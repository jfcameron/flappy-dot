#include<gdk/static_text_renderer.h>

using namespace gdk;

static_text_renderer::static_text_renderer(gdk::graphics::context::context_shared_ptr_type pContext,
	gdk::text_map aTextMap,
	std::wstring aText)
	: text_renderer(pContext, aTextMap)
{
	build_string_model(vertex_data_view::UsageHint::Static, aText);
}
