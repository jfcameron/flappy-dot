// © Joseph Cameron - All Rights Reserved

#ifndef GDK_DYNAMIC_TEXT_RENDERER_H
#define GDK_DYNAMIC_TEXT_RENDERER_H

#include <gdk/text_renderer.h>

namespace gdk
{
	/// \brief renders text that can be changed
	class dynamic_text_renderer final : public text_renderer
	{
		/// \brief text buffer, used to prevent unnecessary vertex data reconstruction
		std::wstring m_Text;

	public:
		void update_text(const std::wstring &aText);

		dynamic_text_renderer(graphics::context_ptr_type pContext,
			text_map aTextMap,
			const text_renderer::alignment aAlignment,
			const std::wstring &aText = L" ");

		~dynamic_text_renderer() = default;
	};
}

#endif
