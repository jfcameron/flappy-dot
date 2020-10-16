// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_BACKGROUND_MUSIC_PLAYER_H
#define JFC_BACKGROUND_MUSIC_PLAYER_H

#include <jfc/flappy_event_bus.h>
#include <jfc/screen_stack.h>
#include <gdk/audio/context.h>

#include <jfc/POL_chubby_cat_short.ogg.h>
#include <jfc/Very_Short_Work.ogg.h>

namespace flappy
{
	class background_music_player
	{
	public:
		using screen_push_observer_type = jfc::event_bus<flappy::screen_pushed_event>::observer_shared_ptr_type;

		using screen_popped_observer_type = jfc::event_bus<flappy::screen_popped_event>::observer_shared_ptr_type;

	private:
		std::map<std::string, gdk::audio::context::emitter_shared_ptr_type> m_ScreenNameToBGM;

		screen_push_observer_type m_pScreenPushObserver;

		screen_popped_observer_type m_pScreenPopObserver;

		gdk::audio::context::emitter_shared_ptr_type m_pCurrentBGM;

		void handle_track_change(std::string name)
		{
			if (auto s = m_ScreenNameToBGM.find(name); s != m_ScreenNameToBGM.end())
			{
				m_pCurrentBGM = m_ScreenNameToBGM[name];
			
				for (auto p : m_ScreenNameToBGM)
				{
					if (p.second != m_pCurrentBGM) p.second->stop();
				}

				m_pCurrentBGM->play();
			}
		}

	public:
		background_music_player(std::shared_ptr<flappy::event_bus> pEventBus, gdk::audio::context::context_shared_ptr_type aAudioContext)
			: m_pScreenPushObserver(std::make_shared<screen_push_observer_type::element_type>([&](
				flappy::screen_pushed_event e)
		{
			handle_track_change(e.name);
		}))
			, m_pScreenPopObserver(std::make_shared<screen_popped_observer_type::element_type>([&](
				flappy::screen_popped_event e)
		{
			handle_track_change(e.name);
		}))
		{
			auto pSound = aAudioContext->make_sound(gdk::audio::sound::encoding_type::vorbis, std::vector<unsigned char>(
				POL_chubby_cat_short_ogg, POL_chubby_cat_short_ogg + sizeof(POL_chubby_cat_short_ogg) / sizeof(POL_chubby_cat_short_ogg[0])));

			m_ScreenNameToBGM["MainMenu"] = aAudioContext->make_emitter(pSound);

			pSound = aAudioContext->make_sound(gdk::audio::sound::encoding_type::vorbis, std::vector<unsigned char>(
				Very_Short_Work_ogg, Very_Short_Work_ogg + sizeof(Very_Short_Work_ogg) / sizeof(Very_Short_Work_ogg[0])));

			m_ScreenNameToBGM["GameScreen"] = aAudioContext->make_emitter(pSound);
			m_ScreenNameToBGM["GameScreen"]->set_pitch(1.f);

			pEventBus->add_screen_pushed_event_observer(m_pScreenPushObserver);
			pEventBus->add_screen_popped_event_observer(m_pScreenPopObserver);
		}

		void update()
		{
			if (!m_pCurrentBGM->isPlaying()) m_pCurrentBGM->play();
		}
	};
}

#endif
