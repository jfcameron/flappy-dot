// © Joseph Cameron - All Rights Reserved

#include <gdk/static_text_renderer.h>

using namespace gdk;

static_text_renderer::static_text_renderer(graphics::context_ptr_type pContext,
	text_map aTextMap,
	const text_renderer::alignment aAlignment,
	const std::wstring &aText)
	: text_renderer(pContext, aTextMap, aAlignment)
{
	build_string_model(graphics::model::usage_hint::upload_once, aText);
}
