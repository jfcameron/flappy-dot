#include<gdk/text_renderer.h>

using namespace gdk;

text_renderer::text_renderer(gdk::graphics::context::context_shared_ptr_type aContext,
	gdk::text_map aTextMap)
	: pContext(aContext)
	, m_TextMap(aTextMap)
{
	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));

	m_Material->setTexture("_Texture", aTextMap.getTexture());
	m_Material->setVector2("_UVScale", { 1, 1 });
	m_Material->setVector2("_UVOffset", { 0, 0 });
}

void text_renderer::build_character_quad(vertex_attribute_array_type& rPosData,
	vertex_attribute_array_type& rUVData,
	const wchar_t character,
	gdk::IntVector2<size_t> aCharacterOffsetInCells)
{
	vertex_attribute_type size = 1;
	decltype(size) hsize = size / 2.;

	auto rasterCoord = m_TextMap.get_raster_coordinate(character); // L'あ'works here too. wchar_t is fine

	auto textureSizeInCells = m_TextMap.getTextureSizeInCells();

	gdk::Vector2<float> offset = aCharacterOffsetInCells.toVector2();

	vertex_attribute_array_type posData({ //clean this up
		offset.x + size - hsize, offset.y + size - hsize, 0.0f,
		offset.x + 0.0f - hsize, offset.y + size - hsize, 0.0f,
		offset.x + 0.0f - hsize, offset.y + 0.0f - hsize, 0.0f,
		offset.x + size - hsize, offset.y + size - hsize, 0.0f,
		offset.x + 0.0f - hsize, offset.y + 0.0f - hsize, 0.0f,
		offset.x + size - hsize, offset.y + 0.0f - hsize, 0.0f
		});

	for (int i(0); i < posData.size() / 3; ++i)
	{
		posData[i + 0] += 0.0f;//offset.x;
		posData[i + 1] += 0.0f;//offset.y;
	}

	rPosData.insert(rPosData.end(), posData.begin(), posData.end());

	gdk::Vector2<float> cellSize(1.f / textureSizeInCells.x, 1.f / textureSizeInCells.y);

	const float sampleBias = 0.001f;

	float bottom_xl = rasterCoord.x * cellSize.x + sampleBias;
	float bottom_yl = rasterCoord.y * cellSize.y + sampleBias;
	float bottom_xh = rasterCoord.x * cellSize.x + cellSize.x - sampleBias;
	float bottom_yh = rasterCoord.y * cellSize.y + cellSize.y - sampleBias;

	vertex_attribute_array_type uvData({
		bottom_xh, bottom_yl,
		bottom_xl, bottom_yl,
		bottom_xl, bottom_yh,
		bottom_xh, bottom_yl,
		bottom_xl, bottom_yh,
		bottom_xh, bottom_yh,
		});

	rUVData.insert(rUVData.end(), uvData.begin(), uvData.end());
}

void text_renderer::add_to_scene(gdk::graphics::context::scene_shared_ptr_type pScene)
{
	pScene->add_entity(m_Entity);
}

void text_renderer::build_string_model(vertex_data_view::UsageHint hint, const std::wstring& aText)
{
	text_map aTextMap = m_TextMap;

	text_renderer::vertex_attribute_array_type posData;
	text_renderer::vertex_attribute_array_type uvData;

	//std::wstring aText = L"あasdf123.?";

	for (int i(0); i < aText.size(); ++i)
	{
		build_character_quad(posData, uvData, aText[i], { i, 0 }); //L'あ' works here
	}

	vertex_data_view data = { hint,
	{
		{
			"a_Position",
			{
				&posData.front(),
				posData.size(),
				3
			}
		},
		{
			"a_UV",
			{
				&uvData.front(),
				uvData.size(),
				2
			}
		}
	}};

	if (m_Model == nullptr)
	{
		m_Model = std::shared_ptr<model>(std::move(
			pContext->make_model(data)));
	}
	else
	{
		m_Model->update_vertex_data(data);
	}

	if (m_Entity == nullptr)
	{
		m_Entity = pContext->make_entity(m_Model, m_Material);

		m_Entity->set_model_matrix({ 0, 0, 0 }, { {0, 0, 0} }, { 0.1, 0.1, 1 });
	}
}
