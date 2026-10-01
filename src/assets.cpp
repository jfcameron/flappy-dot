// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/assets.h>

#include <jfc/Coins.ogg.h>
#include <jfc/Text_Sheet.png.h>

#include <jfc/Background_0.png.h>
#include <jfc/Background_1.png.h>
#include <jfc/Background_2.png.h>
#include <jfc/Background_3.png.h>
#include <jfc/Background_4.png.h>
#include <jfc/Background_5.png.h>
#include <jfc/Background_6.png.h>
#include <jfc/jump.ogg.h>
#include <jfc/Floor.png.h>
#include <jfc/Sprite_Sheet.png.h>

#include <gdk/graphics/ext/png.h>
#include <gdk/graphics/model_data.h>

using namespace flappy;
using namespace gdk;

/// \brief decodes a png and uploads it to a texture
static graphics::texture_ptr_type make_texture(graphics::context_ptr_type pGraphics,
	const unsigned char *const aPNG, const size_t aSize)
{
	auto [view, pData] = graphics::ext::make_from_png({ aPNG, aSize });

	return pGraphics->make_texture(view);
}

#define FLAPPY_MAKE_TEXTURE(png) make_texture(m_pGraphics, png, sizeof png)

assets::assets(decltype(m_pGraphics) aGraphics)
	: m_pGraphics(aGraphics)
	, m_AlphaCutoffShader(m_pGraphics->make_alpha_cutoff_shader())
	, m_QuadModel([&]()
	{
		auto quad = graphics::model_data::make_quad();
		quad.transform("a_Position", { -0.5f, -0.5f, 0.f });

		return m_pGraphics->make_model(graphics::model::usage_hint::upload_once, quad);
	}())
	, m_CoinSound(audio::make_vorbis_sound(Coins_ogg, sizeof Coins_ogg))
	, m_TextTexture(FLAPPY_MAKE_TEXTURE(Text_Sheet_png))
	, m_TextMap(m_TextTexture, { 8, 8 },
	{
		{'a', {0,0}},
		{'b', {1,0}},
		{'c', {2,0}},
		{'d', {3,0}},
		{'e', {4,0}},
		{'f', {5,0}},
		{'g', {6,0}},
		{'h', {7,0}},
		{'i', {0,1}},
		{'j', {1,1}},
		{'k', {2,1}},
		{'l', {3,1}},
		{'m', {4,1}},
		{'n', {5,1}},
		{'o', {6,1}},
		{'p', {7,1}},
		{'q', {0,2}},
		{'r', {1,2}},
		{'s', {2,2}},
		{'t', {3,2}},
		{'u', {4,2}},
		{'v', {5,2}},
		{'w', {6,2}},
		{'x', {7,2}},
		{'y', {0,3}},
		{'z', {1,3}},
		{'A', {0,0}},
		{'B', {1,0}},
		{'C', {2,0}},
		{'D', {3,0}},
		{'E', {4,0}},
		{'F', {5,0}},
		{'G', {6,0}},
		{'H', {7,0}},
		{'I', {0,1}},
		{'J', {1,1}},
		{'K', {2,1}},
		{'L', {3,1}},
		{'M', {4,1}},
		{'N', {5,1}},
		{'O', {6,1}},
		{'P', {7,1}},
		{'Q', {0,2}},
		{'R', {1,2}},
		{'S', {2,2}},
		{'T', {3,2}},
		{'U', {4,2}},
		{'V', {5,2}},
		{'W', {6,2}},
		{'X', {7,2}},
		{'Y', {0,3}},
		{'Z', {1,3}},
		{'0', {3,3}},
		{'1', {4,3}},
		{'2', {5,3}},
		{'3', {6,3}},
		{'4', {7,3}},
		{'5', {0,4}},
		{'6', {1,4}},
		{'7', {2,4}},
		{'8', {3,4}},
		{'9', {4,4}},
		{'!', {2,3}},
		{'.', {5,4}},
		{':', {6,4}},
		{'?', {7,3}},
		{' ', {7,6}},
		{'/', {0,5}},
		{'-', {1,5}},
	})
	, m_BGLayerTextures(decltype(m_BGLayerTextures){
		FLAPPY_MAKE_TEXTURE(Background_0_png),
		FLAPPY_MAKE_TEXTURE(Background_1_png),
		FLAPPY_MAKE_TEXTURE(Background_2_png),
		FLAPPY_MAKE_TEXTURE(Background_3_png),
		FLAPPY_MAKE_TEXTURE(Background_4_png),
		FLAPPY_MAKE_TEXTURE(Background_5_png),
		FLAPPY_MAKE_TEXTURE(Background_6_png),
		FLAPPY_MAKE_TEXTURE(Floor_png)
		})
	, m_SpriteSheet(FLAPPY_MAKE_TEXTURE(Sprite_Sheet_png))
	, m_FlapSound(audio::make_vorbis_sound(jump_ogg, sizeof jump_ogg))
{}

#undef FLAPPY_MAKE_TEXTURE

decltype(assets::m_AlphaCutoffShader) assets::get_alpha_cutoff_shader() const
{
	return m_AlphaCutoffShader;
}

decltype(assets::m_QuadModel) assets::get_quad_model() const
{
	return m_QuadModel;
}

decltype(assets::m_FlapSound) assets::get_flapsound() const
{
	return m_FlapSound;
}

decltype(assets::m_BGLayerTextures) assets::get_bglayertextures() const
{
	return m_BGLayerTextures;
}

decltype(assets::m_CoinSound) assets::get_coin_sound() const
{
	return m_CoinSound;
}

decltype(assets::m_TextMap) assets::get_textmap() const
{
	return m_TextMap;
}

decltype(assets::m_SpriteSheet) assets::get_spritesheet() const
{
	return m_SpriteSheet;
}
