// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/pipe.h>

#include <jfc/Sprite_Sheet.png.h>

#include <chrono>

using namespace flappy;
using namespace gdk;

static const graphics_vector2_type PIPE_MOUTH_UP_GRAPHIC(0, 1);
static const graphics_vector2_type PIPE_MOUTH_DOWN_GRAPHIC(1, 1);
static const graphics_vector2_type PIPE_TRUNK_GRAPHIC(3, 0);

static std::shared_ptr<model> generatePipeModel(graphics_vector2_type aBottomTileCell, 
	graphics_vector2_type aMiddleTileCell,
	graphics_vector2_type aTopTileCell,
	gdk::graphics::context::context_shared_ptr_type pContext)
{
	using vertex_attribute_type = float;
	using vertex_attribute_array_type = std::vector<vertex_attribute_type>;

	vertex_attribute_type size = 1;
	decltype(size) hsize = size / 2.;

	vertex_attribute_array_type posData({ //"tiles", used to render the two pieces of pipe (mouth and trunk)
		size - hsize, size - hsize, 0.0f,
		0.0f - hsize, size - hsize, 0.0f,
		0.0f - hsize, 0.0f - hsize, 0.0f,
		size - hsize, size - hsize, 0.0f,
		0.0f - hsize, 0.0f - hsize, 0.0f,
		size - hsize, 0.0f - hsize, 0.0f,

		size - hsize, 2 * size - hsize, 0.0f,
		0.0f - hsize, 2 * size - hsize, 0.0f,
		0.0f - hsize, 1 * size - hsize, 0.0f,
		size - hsize, 2 * size - hsize, 0.0f,
		0.0f - hsize, 1 * size - hsize, 0.0f,
		size - hsize, 1 * size - hsize, 0.0f, 

		size - hsize, 3 * size - hsize, 0.0f,
		0.0f - hsize, 3 * size - hsize, 0.0f,
		0.0f - hsize, 2 * size - hsize, 0.0f,
		size - hsize, 3 * size - hsize, 0.0f,
		0.0f - hsize, 2 * size - hsize, 0.0f,
		size - hsize, 2 * size - hsize, 0.0f,
	});

	const float cellSize = 1.f / 4.f;
	const float sampleBias = 0.001f; // This prevents seams from occasionally appearing when a 
	// fragment samples a texel from a neighbouring cell. 
	// This is only really an issue for extremely low pixel count games (such as this one)

	aBottomTileCell *= cellSize;
	aTopTileCell *= cellSize;

	float bottom_xl = aBottomTileCell.x + sampleBias;
	float bottom_yl = aBottomTileCell.y + sampleBias;
	float bottom_xh = aBottomTileCell.x + cellSize - sampleBias;
	float bottom_yh = aBottomTileCell.y + cellSize - sampleBias;

	float middle_xl = aBottomTileCell.x + sampleBias;
	float middle_yl = aBottomTileCell.y + sampleBias;
	float middle_xh = aBottomTileCell.x + cellSize - sampleBias;
	float middle_yh = aBottomTileCell.y + cellSize - sampleBias;

	float top_xl = aTopTileCell.x + sampleBias;
	float top_yl = aTopTileCell.y + sampleBias;
	float top_xh = aTopTileCell.x + cellSize - sampleBias;
	float top_yh = aTopTileCell.y + cellSize - sampleBias;

	vertex_attribute_array_type uvData({ //Quad data: uvs
		bottom_xh, bottom_yl,
		bottom_xl, bottom_yl,
		bottom_xl, bottom_yh,
		bottom_xh, bottom_yl,
		bottom_xl, bottom_yh,
		bottom_xh, bottom_yh,

		middle_xh, middle_yl,
		middle_xl, middle_yl,
		middle_xl, middle_yh,
		middle_xh, middle_yl,
		middle_xl, middle_yh,
		middle_xh, middle_yh,

		top_xh, top_yl,
		top_xl, top_yl,
		top_xl, top_yh,
		top_xh, top_yl,
		top_xl, top_yh,
		top_xh, top_yh,
	});

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

	return pModel;
}

pipe::pipe(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::graphics::context::scene_shared_ptr_type pScene)
: m_Position(0.5, -0.5, -0.435)
{
	auto pTexture = std::shared_ptr<texture>(std::move(pContext->make_texture(
		{ Sprite_Sheet_png, Sprite_Sheet_png + sizeof Sprite_Sheet_png / sizeof Sprite_Sheet_png[0] })));

	m_Material = std::shared_ptr<material>(std::move(pContext->make_material(pContext->get_alpha_cutoff_shader())));
	m_Material->setTexture("_Texture", pTexture);
	m_Material->setVector2("_UVScale", { 1, 1 });

	m_UpPipeModel = generatePipeModel(PIPE_TRUNK_GRAPHIC, PIPE_TRUNK_GRAPHIC, PIPE_MOUTH_UP_GRAPHIC, pContext);
	m_DownPipeModel = generatePipeModel(PIPE_MOUTH_DOWN_GRAPHIC, PIPE_TRUNK_GRAPHIC, PIPE_TRUNK_GRAPHIC, pContext);

	m_Entity = pContext->make_entity(m_UpPipeModel, m_Material);

	pScene->add_entity(m_Entity);
}

float total_time(0);

void pipe::update(const float delta)
{
	total_time += delta * 0.999f;

	m_Position.x -= delta;

	m_Entity->set_model_matrix(m_Position, {{0, 0, 45}}, { 0.25, 0.25, 1 });

	if (m_Position.x < -2)
	{
		m_Position.x = 2;
	}
}
