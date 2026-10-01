// © Joseph Cameron - All Rights Reserved

#ifndef GDK_STATIC_TEXT_RENDERER_H
#define GDK_STATIC_TEXT_RENDERER_H

#include <gdk/text_renderer.h>

namespace gdk
{
	/// \brief renders text that cannot be updated
	class static_text_renderer final : public text_renderer
	{
	public:
		static_text_renderer(graphics::context_ptr_type pContext,
			text_map aTextMap,
			const text_renderer::alignment aAlignment,
			const std::wstring &aText);

		~static_text_renderer() = default;
	};
}

#endif
