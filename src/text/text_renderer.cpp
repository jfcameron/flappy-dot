// © Joseph Cameron - All Rights Reserved

#include <gdk/text_renderer.h>

#include <gdk/graphics/material.h>
#include <gdk/graphics/model_data.h>

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

using namespace gdk;

static constexpr char TAG[] = "gdk::text_renderer";

using vertex_attribute_array_type = std::vector<graphics::component_type>;

/// \brief the offset from the text's origin to the upper left corner of the first character
static graphics::vector2_type alignment_to_offset_vector(text_renderer::alignment aAlignment, 
	size_t aLongestLineLength, size_t aLineCount)
{
	const auto width(static_cast<float>(aLongestLineLength));
	const auto half_width(width / 2.f + 0.5f);
	
	const auto height(static_cast<float>(aLineCount));
	const auto half_height(height / 2.f);

	switch (aAlignment)
	{
		case text_renderer::alignment::left_edge: return         { 0, 0.5f - half_height };
		case text_renderer::alignment::left_lower_corner: return { 0, 0.0f - height };
		case text_renderer::alignment::left_upper_corner: return { 0, 1.0f };

		case text_renderer::alignment::right_edge: return         { -width, 0.5f - half_height };
		case text_renderer::alignment::right_lower_corner: return { -width, 0.0f - height };
		case text_renderer::alignment::right_upper_corner: return { -width, 1.0f };

		case text_renderer::alignment::center: return     { 0.5f - half_width, 0.5f - half_height };
		case text_renderer::alignment::upper_edge: return { 0.5f - half_width, 1.0f };
		case text_renderer::alignment::lower_edge: return { 0.5f - half_width, 0.0f - height };
	}

	throw std::invalid_argument(std::string(TAG) + ": unhandled text alignment");
}

/// \brief builds a quad for a single character, appends it to the back of the buffers
static void build_character_quad(vertex_attribute_array_type &rPosData,
	vertex_attribute_array_type &rUVData,
	const text_map &aTextMap,
	const wchar_t aCharacter,
	const graphics::vector2_type &aCharacterOffsetInCells,
	const graphics::vector2_type &aAlignmentOffset)
{
	vertex_attribute_array_type posData({
		1, 1, 0,
		0, 1, 0,
		0, 0, 0,
		1, 1, 0,
		0, 0, 0,
		1, 0, 0
	});

	for (size_t i(0), s(posData.size()); i < s; i += 3)
	{
		posData[i + 0] += aAlignmentOffset.x + aCharacterOffsetInCells.x;
		posData[i + 1] -= aAlignmentOffset.y + aCharacterOffsetInCells.y;
	}

	rPosData.insert(rPosData.end(), posData.begin(), posData.end());

	const auto &rasterCoord = aTextMap.raster_coordinate(aCharacter);
	const auto &textureSizeInCells = aTextMap.texture_size_in_cells();

	const graphics::vector2_type cellSize(1.f / static_cast<float>(textureSizeInCells.x), 
		1.f / static_cast<float>(textureSizeInCells.y));

	// prevents fragments from sampling texels belonging to a neighbouring cell
	const float sampleBias = 0.001f;

	const float xl = static_cast<float>(rasterCoord.x) * cellSize.x + sampleBias;
	const float yl = static_cast<float>(rasterCoord.y) * cellSize.y + sampleBias;
	const float xh = static_cast<float>(rasterCoord.x) * cellSize.x + cellSize.x - sampleBias;
	const float yh = static_cast<float>(rasterCoord.y) * cellSize.y + cellSize.y - sampleBias;

	rUVData.insert(rUVData.end(), {
		xh, yl,
		xl, yl,
		xl, yh,
		xh, yl,
		xl, yh,
		xh, yh,
	});
}

text_renderer::text_renderer(graphics::context_ptr_type aContext,
	text_map aTextMap,
	const alignment aAlignment,
	graphics::material_ptr_type aMaterial)
	: m_pContext(aContext)
	, m_TextMap(aTextMap)
	, m_Alignment(aAlignment)
	, m_Material([&]()
	{
		if (aMaterial) return aMaterial;

		// the shader is the same for every text renderer, so it is compiled once and shared
		static std::weak_ptr<graphics::shader_program> wpShader;

		auto pShader = wpShader.lock();

		if (!pShader) wpShader = pShader = aContext->make_alpha_cutoff_shader();

		auto p = aContext->make_material(pShader);
		p->set_texture("_Texture", aTextMap.texture());
		p->set_vector2("_UVScale", { 1, 1 });
		p->set_vector2("_UVOffset", { 0, 0 });

		return p;
	}())
	, m_Model(aContext->make_model())
	, m_Entity(aContext->make_entity(m_Model, m_Material))
{}

void text_renderer::build_string_model(const graphics::model::usage_hint aHint, const std::wstring &aText)
{
	size_t longestLineLength(0), lineCount(0);

	for (size_t i(0), currentLineLength(0); i < aText.size(); ++i)
	{
		if (aText[i] == L'\n' || aText[i] == L'\r')
		{
			currentLineLength = 0;

			++lineCount;
		}
		else if (++currentLineLength > longestLineLength) longestLineLength = currentLineLength;
	}

	const auto alignmentOffset = alignment_to_offset_vector(m_Alignment, longestLineLength, lineCount);

	vertex_attribute_array_type posData;
	vertex_attribute_array_type uvData;

	for (size_t i(0), lineCharacterCounter(0), lineCounter(0); i < aText.size(); ++i)
	{
		const auto character = aText[i];

		if (character == L'\n' || character == L'\r')
		{
			lineCharacterCounter = 0;

			++lineCounter;
		}
		else
		{
			build_character_quad(posData, uvData, m_TextMap, character, 
				{ static_cast<float>(lineCharacterCounter), static_cast<float>(lineCounter) },
				alignmentOffset);

			++lineCharacterCounter;
		}
	}

	if (posData.empty())
	{
		m_Model->upload(aHint, {});

		return;
	}

	m_Model->upload(aHint, graphics::model_data({
		{ "a_Position", { posData, 3 } },
		{ "a_UV", { uvData, 2 } }
	}));
}

void text_renderer::set_transform(const graphics::vector3_type &aWorldPos,
	const graphics::quaternion_type &aRotation,
	const graphics::vector3_type &aScale)
{
	m_Entity->set_transform(aWorldPos, aRotation, aScale);
}

void text_renderer::add_to_scene(graphics::scene_ptr_type pScene)
{
	pScene->add(m_Entity);
}

void text_renderer::remove_from_scene(graphics::scene_ptr_type pScene)
{
	pScene->remove(m_Entity);
}

void text_renderer::hide()
{
	m_Entity->hide();
}

void text_renderer::show()
{
	m_Entity->show();
}

bool text_renderer::is_hidden() const
{
	return m_Entity->is_hidden();
}
