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

using namespace flappy;
using namespace gdk;

assets::assets(decltype(m_pGraphics) aGraphics, 
	decltype(m_pAudio) aAudio,
	input::context::context_shared_ptr_type pInput)
	: m_pGraphics(aGraphics)
	, m_pAudio(aAudio)
	, m_CoinSound(m_pAudio->make_sound(audio::sound::encoding_type::vorbis, std::vector<unsigned char>(
		Coins_ogg, Coins_ogg + sizeof(Coins_ogg) / sizeof(Coins_ogg[0]))))
	, m_TextTexture(std::shared_ptr<gdk::texture>(std::shared_ptr<texture>(std::move(m_pGraphics->make_texture(
		{ Text_Sheet_png, Text_Sheet_png + sizeof Text_Sheet_png / sizeof Text_Sheet_png[0] })))))
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
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_0_png, Background_0_png + sizeof Background_0_png / sizeof Background_0_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_1_png, Background_1_png + sizeof Background_1_png / sizeof Background_1_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_2_png, Background_2_png + sizeof Background_2_png / sizeof Background_2_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_3_png, Background_3_png + sizeof Background_3_png / sizeof Background_3_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_4_png, Background_4_png + sizeof Background_4_png / sizeof Background_4_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_5_png, Background_5_png + sizeof Background_5_png / sizeof Background_5_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Background_6_png, Background_6_png + sizeof Background_6_png / sizeof Background_6_png[0] }))),
		std::shared_ptr<texture>(std::move(m_pGraphics->make_texture({ Floor_png, Floor_png + sizeof Floor_png / sizeof Floor_png[0] })))
		})
	, m_SpriteSheet(std::shared_ptr<texture>(std::move(m_pGraphics->make_texture(
		{ Sprite_Sheet_png, Sprite_Sheet_png + sizeof Sprite_Sheet_png / sizeof Sprite_Sheet_png[0] }))))
	, m_FlapSound(m_pAudio->make_sound(audio::sound::encoding_type::vorbis, std::vector<unsigned char>(
		jump_ogg, jump_ogg + sizeof(jump_ogg) / sizeof(jump_ogg[0]))))
{}

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
