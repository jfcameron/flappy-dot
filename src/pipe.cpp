// © 2020 Joseph Cameron - All Rights Reserved
#include <jfc/pipe.h>
#include <jfc/bird.h>

#include <cmath>

using namespace flappy;
using namespace gdk;

static const graphics::vector2_type PIPE_MOUTH_GRAPHIC(0, 1);
static const graphics::vector2_type PIPE_TRUNK_GRAPHIC(3, 0);

static graphics::model_data generatePipeModel(graphics::vector2_type aBottomTileCell, 
	graphics::vector2_type aMiddleTileCell,
	graphics::vector2_type aTopTileCell)
{
	using vertex_attribute_type = graphics::component_type;
	using vertex_attribute_array_type = std::vector<vertex_attribute_type>;

	vertex_attribute_type size = 1;
	decltype(size) hsize = size / 2.f;

	vertex_attribute_array_type posData({ //quads used to render the pieces of pipe (mouth and trunk)
		size - hsize, size - hsize, 0.0f, // NOTE: This work should be abstracted away into a tiled mesh generator.
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
	aMiddleTileCell *= cellSize;
	aTopTileCell *= cellSize;

	float bottom_xl = aBottomTileCell.x + sampleBias;
	float bottom_yl = aBottomTileCell.y + sampleBias;
	float bottom_xh = aBottomTileCell.x + cellSize - sampleBias;
	float bottom_yh = aBottomTileCell.y + cellSize - sampleBias;

	float middle_xl = aMiddleTileCell.x + sampleBias;
	float middle_yl = aMiddleTileCell.y + sampleBias;
	float middle_xh = aMiddleTileCell.x + cellSize - sampleBias;
	float middle_yh = aMiddleTileCell.y + cellSize - sampleBias;

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

	return graphics::model_data({
		{ "a_Position", { posData, 3 } },
		{ "a_UV", { uvData, 2 } }
	});
}

/// \brief mouth at the top, so a pipe rotated by pi hangs with its mouth at the bottom
static const graphics::model_data &pipe_model_data()
{
	static const auto data = generatePipeModel(PIPE_TRUNK_GRAPHIC, PIPE_TRUNK_GRAPHIC, PIPE_MOUTH_GRAPHIC);

	return data;
}

pipe::pipe(gdk::graphics::context_ptr_type pContext,
	gdk::graphics::scene_ptr_type pScene,
	flappy::assets::shared_ptr aAssets)
: m_Position(-5.0, 0.0)
, m_Scale(0.25, 0.25)
{
	m_Material = pContext->make_material(aAssets->get_alpha_cutoff_shader());
	m_Material->set_texture("_Texture", aAssets->get_spritesheet());
	m_Material->set_vector2("_UVScale", { 1, 1 });
	m_Material->set_vector2("_UVOffset", { 0, 0 });

	m_Entity = pContext->make_entity(
		pContext->make_model(graphics::model::usage_hint::upload_once, pipe_model_data()), m_Material);
	
	pScene->add(m_Entity);
}

void pipe::update(const float delta)
{
	m_Position.x -= delta * 0.65f;

	m_Entity->set_transform({ m_Position.x, m_Position.y, -0.435f }, 
		graphics::quaternion_type::from_euler({ 0, 0, m_Rotation }), 
		{ m_Scale.x, m_Scale.y, 1 });
}

decltype(pipe::m_Position) pipe::getPosition() const
{
	return {m_Position.x, m_Position.y};
}

decltype(pipe::m_Scale) pipe::getScale() const
{
	return m_Scale;
}

decltype(pipe::m_Rotation) pipe::getRotation() const
{
	return m_Rotation;
}

void pipe::set_up(const decltype(m_Position)& aPosition, 
	const decltype(m_Rotation) aRotation)
{
	m_Position = aPosition;
	m_Rotation = aRotation;
}

bool pipe::check_collision(const graphics::vector2_type &aWorldPosition) const
{
	// bail early if collision is not possible
	if (std::abs(m_Position.x) > 0.5f) return false;

	// Moving the bird's world position into the pipe's local space
	// for rotation friendly point vs box collision detection.
	// NOTE: This should not be the pipe's responsibility. This should be performed by a separate system but
	// this implementation is OK given how simple the game is.
	// The thresholds below are tuned against translation and rotation only; scale is not removed.
	const graphics::vector2_type offset(aWorldPosition.x - m_Position.x, aWorldPosition.y - m_Position.y);

	const auto c = std::cos(-m_Rotation);
	const auto s = std::sin(-m_Rotation);

	const graphics::vector2_type localBirdPos(offset.x * c - offset.y * s, 
		offset.x * s + offset.y * c);

	return 
		//Horizontal check
		std::abs(localBirdPos.x) < 0.15f
		//Vertical checks
		&& localBirdPos.y > -0.1f
		&& localBirdPos.y < 0.5f;
}
