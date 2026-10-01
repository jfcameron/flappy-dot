// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_ASSETS_H
#define FLAPPY_ASSETS_H

#include <gdk/audio/scene.h>
#include <gdk/audio/sound.h>
#include <gdk/graphics/context.h>
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
		graphics::context_ptr_type m_pGraphics;

		/// \brief shader used by all of the sprites in the game
		graphics::shader_ptr_type m_AlphaCutoffShader;

		/// \brief 1x1 quad, centered on the origin
		graphics::model_ptr_type m_QuadModel;

		/// \brief coin sound effect
		audio::sound_shared_ptr_type m_CoinSound;

		/// \brief the texture containing the rasterized alphanumeric characters
		graphics::texture_ptr_type m_TextTexture;

		/// \brief text map used throughout the program
		text_map m_TextMap;

		/// \brief textures used for the background layer of the game/menu scenes
		std::array<graphics::texture_ptr_type, 8> m_BGLayerTextures;

		/// \brief texture sheet containing bird graphics etc.
		graphics::texture_ptr_type m_SpriteSheet;

		/// \brief bird flap sound effect
		audio::sound_shared_ptr_type m_FlapSound;

	public:
		decltype(m_AlphaCutoffShader) get_alpha_cutoff_shader() const;

		decltype(m_QuadModel) get_quad_model() const;

		decltype(m_CoinSound) get_coin_sound() const;

		decltype(m_TextMap) get_textmap() const;

		decltype(m_BGLayerTextures) get_bglayertextures() const;

		decltype(m_SpriteSheet) get_spritesheet() const;

		decltype(m_FlapSound) get_flapsound() const;
		
		assets(decltype(m_pGraphics) aGraphics);
	};
}

#endif
