// � 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_SCREEN_H
#define GDK_SCREEN_H

#include <map>

namespace gdk
{
	class screen
	{
	public:
		virtual void update(float delta, float windowAspectRatio, std::pair<int, int> windowSize) = 0;

		//screen() = default;

		//screen(screen&&) = default;

		screen& operator=(screen&&) = default;

		virtual ~screen() = default;
	};
}

#endif