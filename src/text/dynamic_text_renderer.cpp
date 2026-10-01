// © Joseph Cameron - All Rights Reserved

#include <gdk/dynamic_text_renderer.h>

using namespace gdk;

void dynamic_text_renderer::update_text(const std::wstring &aText)
{
	if (aText == m_Text) return;

	build_string_model(graphics::model::usage_hint::dynamic, aText);

	m_Text = aText;
}

dynamic_text_renderer::dynamic_text_renderer(graphics::context_ptr_type pContext,
	text_map aTextMap,
	const text_renderer::alignment aAlignment,
	const std::wstring &aText)
	: text_renderer(pContext, aTextMap, aAlignment)
	, m_Text(aText)
{
	build_string_model(graphics::model::usage_hint::dynamic, aText);
}
