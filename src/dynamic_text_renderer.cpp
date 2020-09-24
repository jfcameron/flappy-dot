#include<gdk/dynamic_text_renderer.h>

using namespace gdk;

void dynamic_text_renderer::update_text(const std::wstring& aText)
{
	build_string_model(vertex_data_view::UsageHint::Dynamic, aText);
}

dynamic_text_renderer::dynamic_text_renderer(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::text_map aTextMap,
	std::wstring aText)
	: text_renderer(pContext, aTextMap)
{
	build_string_model(vertex_data_view::UsageHint::Dynamic, aText);
}
