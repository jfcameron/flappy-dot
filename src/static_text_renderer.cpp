#include<gdk/static_text_renderer.h>

using namespace gdk;

void static_text_renderer::build_character_quad(vertex_attribute_array_type& rPosData, 
	vertex_attribute_array_type& rUVData,
	const char character,
	gdk::IntVector2<size_t> aCharacterOffsetInCells)
{
	vertex_attribute_type size = 1;
	decltype(size) hsize = size / 2.;

	auto rasterCoord = m_TextMap.get_raster_coordinate(character);

	auto textureSizeInCells = m_TextMap.getTextureSizeInCells();

	gdk::Vector2<float> offset = aCharacterOffsetInCells.toVector2();

	vertex_attribute_array_type posData({ //clean this up good lord
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

static_text_renderer::static_text_renderer(std::string aText,
	gdk::text_map aTextMap,
	gdk::graphics::context::context_shared_ptr_type pContext)
	: m_TextMap(aTextMap)
{
	vertex_attribute_array_type posData;
	vertex_attribute_array_type uvData;

	for (int i(0); i < aText.size(); ++i)
	{
		build_character_quad(posData, uvData, aText[i], { i, 0 });
	}

	auto pModel = std::shared_ptr<model>(std::move(
		pContext->make_model({ vertex_data_view::UsageHint::Static,
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
		} })));

	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));
	m_Material->setTexture("_Texture", aTextMap.getTexture());
	m_Material->setVector2("_UVScale", { 1, 1 });
	m_Material->setVector2("_UVOffset", { 0, 0 });

	m_Entity = pContext->make_entity(pModel, m_Material);

	m_Entity->set_model_matrix({ 0, 0, 0 }, { {0, 0, 0} }, { 0.1, 0.1, 1 });
}

void static_text_renderer::add_to_scene(gdk::graphics::context::scene_shared_ptr_type pScene)
{
	pScene->add_entity(m_Entity);
}
