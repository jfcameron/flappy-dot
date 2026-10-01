// © 2020 Joseph Cameron - All Rights Reserved

#ifndef JFC_BACKGROUND_MUSIC_PLAYER_H
#define JFC_BACKGROUND_MUSIC_PLAYER_H

#include <jfc/flappy_event_bus.h>
#include <jfc/screen_stack.h>
#include <gdk/audio/emitter.h>
#include <gdk/audio/scene.h>
#include <gdk/audio/sound.h>

#include <map>
#include <string>

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
		std::map<std::string, gdk::audio::emitter_shared_ptr_type> m_ScreenNameToBGM;

		screen_push_observer_type m_pScreenPushObserver;

		screen_popped_observer_type m_pScreenPopObserver;

		gdk::audio::emitter_shared_ptr_type m_pCurrentBGM;

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
		background_music_player(std::shared_ptr<flappy::event_bus> pEventBus, gdk::audio::scene_shared_ptr_type aAudio)
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
			m_ScreenNameToBGM["MainMenu"] = aAudio->make_emitter(gdk::audio::make_vorbis_sound(
				POL_chubby_cat_short_ogg, sizeof POL_chubby_cat_short_ogg));

			m_ScreenNameToBGM["GameScreen"] = aAudio->make_emitter(gdk::audio::make_vorbis_sound(
				Very_Short_Work_ogg, sizeof Very_Short_Work_ogg));
			m_ScreenNameToBGM["GameScreen"]->set_pitch(1.f);

			pEventBus->add_screen_pushed_event_observer(m_pScreenPushObserver);
			pEventBus->add_screen_popped_event_observer(m_pScreenPopObserver);
		}

		void update()
		{
			if (m_pCurrentBGM && !m_pCurrentBGM->is_playing()) m_pCurrentBGM->play();
		}
	};
}

#endif
