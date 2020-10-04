// © 2020 Joseph Cameron - All Rights Reserved

#ifndef GDK_SCREEN_H
#define GDK_SCREEN_H

#include <map>

namespace gdk
{
	class screen
	{
	public:
		virtual void update(float delta, float windowAspectRatio, std::pair<int, int> windowSize) = 0;

		//! explicit delete copy semantics
		screen(screen&) = delete;

		//! explicit delete move semantics
		screen(screen&&) = delete;

		virtual ~screen() = default;

	protected:
		screen() = default;
	};
}

#endif