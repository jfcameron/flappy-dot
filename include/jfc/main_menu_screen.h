// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_MAIN_MENU_SCREEN_H
#define FLAPPY_MAIN_MENU_SCREEN_H

#include <jfc/screen.h>

namespace gdk
{
	class main_menu_screen final : public screen
	{
	public:
		virtual void update(float delta, float aspectRatio, std::pair<int, int> windowSize) override;

		virtual ~main_menu_screen() = default;
	};
}

#endif
