// © 2020 Joseph Cameron - All Rights Reserved
#ifndef FLAPPY_SCREEN_H
#define FLAPPY_SCREEN_H

#include <gdk/screen.h>
#include <gdk/text_renderer.h>

#include <memory>

namespace flappy
{
	/// \brief base screen type for this game
	///
	/// adds functionality for blinking the currently
	/// focused menu item
	///
	/// TODO: consider moving this work to a flappy_menu, which wraps gdk::menu
	/// since this does menu stuff and menus are not exclusive to screens
    class screen : public gdk::screen
    {
    private:
		static constexpr int BLINK_RATE = 26;

        //! used to flash current selection
        bool m_BlinkStatus = true;

		//! Used to determine which text item should blink
		std::shared_ptr<gdk::text_renderer> m_pCurrentText;

		//! blink counter
		int m_PrompCounter = 0;

	protected:
		//! sets the text to blink (current user selection)
		void set_current_text(std::shared_ptr<gdk::text_renderer> aText);

		//! shows the current text if it is not nullptr
		void show_current_text();

    public:
		virtual void update(float delta, 
        float aspectRatio, 
        std::pair<int, int> windowSize) override;
       
        virtual ~screen() = default;
    };
}

#endif
