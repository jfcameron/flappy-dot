// © 2020 Joseph Cameron - All Rights Reserved

#ifndef FLAPPY_GAME_H
#define FLAPPY_GAME_H

#include <gdk/input_context.h>
#include <gdk/intvector2.h>

namespace flappy
{
	using namespace gdk;

	class game final
	{


	public:
		game(const IntVector2<short>& splitScreenSize);
	};
}

#endif
