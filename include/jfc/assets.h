// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_ASSETS_H
#define FLAPPY_ASSETS_H

#include <gdk/audio/context.h>
#include <gdk/graphics_context.h>
#include <gdk/text_map.h>

#include <memory>
#include <array>

namespace flappy
{
	using namespace gdk;

	/// \brief loads all resources (sounds, textures), provides const getters to them
	/// for use throughout the program.
	class assets
	{
	public:
		using shared_ptr = std::shared_ptr<flappy::assets>;

	private:
		/// \brief ptr to the graphics context used throughout flappy
		graphics::context::context_shared_ptr_type m_pGraphics;

		/// \brief ptr to the audio context used throughout flappy
		audio::context::context_shared_ptr_type m_pAudio;

		/// \brief coin sound effect
		audio::context::sound_shared_ptr_type m_CoinSound;

		/// \brief the texture containing the rasterized alphanumeric characters
		std::shared_ptr<gdk::texture> m_TextTexture;

		/// \brief text map used throughout the program
		text_map m_TextMap;

		/// \brief textures used for the background layer of the game/menu scenes
		std::array<std::shared_ptr<texture>, 8> m_BGLayerTextures;

		/// \brief texture sheet containing bird graphics etc.
		std::shared_ptr<gdk::texture> m_SpriteSheet;

		/// \brief bird flap sound effect
		audio::context::sound_shared_ptr_type m_FlapSound;

	public:
		decltype(m_CoinSound) get_coin_sound() const;

		decltype(m_TextMap) get_textmap() const;

		decltype(m_BGLayerTextures) get_bglayertextures() const;

		decltype(m_SpriteSheet) get_spritesheet() const;

		decltype(m_FlapSound) get_flapsound() const;

		assets(decltype(m_pGraphics) aGraphics,
			decltype(m_pAudio) pAudio);
	};
}

#endif
