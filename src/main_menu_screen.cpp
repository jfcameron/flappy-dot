#include <jfc/main_menu_screen.h>

#include <gdk/texture.h>
#include <gdk/text_map.h>

#include <jfc/Text_Sheet.png.h>

using namespace gdk;

class box
{
	gdk::Vector2<float> m_BottomLeft;
	gdk::Vector2<float> m_UpperRight;
};

class element
{
public:

private:
	box m_TouchBox;

public:

};

class pane
{
public:
	std::vector<element> m_elements;

private:

public:

};

std::stack<pane>;


audio::context::context_shared_ptr_type pAudioContext;

main_menu_screen::main_menu_screen(graphics::context::context_shared_ptr_type aGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudioContext,
	screen_stack_ptr_type aScreens,
	screen_ptr_type aGameScreen)
	: m_pInput(aInputContext)
	, m_Screens(aScreens)
	, m_GameScreen(aGameScreen)
	, m_pMainScene(graphics::context::scene_shared_ptr_type(std::move(aGraphicsContext->make_scene())))
	, m_pMainCamera(std::shared_ptr<gdk::camera>(std::move(aGraphicsContext->make_camera())))
	, scenery(flappy::scenery(aGraphicsContext, aGraphicsContext->get_alpha_cutoff_shader(), m_pMainScene))
{
	pAudioContext = aAudioContext;
	
	m_pMainScene->add_camera(m_pMainCamera);

	auto pTextTexture = std::shared_ptr<gdk::texture>(std::shared_ptr<texture>(std::move(aGraphicsContext->make_texture(
		{ Text_Sheet_png, Text_Sheet_png + sizeof Text_Sheet_png / sizeof Text_Sheet_png[0] }))));

	text_map map(
		pTextTexture,
		{ 8, 8 },
		{
			{'a', {0,0}},
			{'b', {1,0}},
			{'c', {2,0}},
			{'d', {3,0}},
			{'e', {4,0}},
			{'f', {5,0}},
			{'g', {6,0}},
			{'h', {7,0}},

			{'i', {0,1}},
			{'j', {1,1}},
			{'k', {2,1}},
			{'l', {3,1}},
			{'m', {4,1}},
			{'n', {5,1}},
			{'o', {6,1}},
			{'p', {7,1}},

			{'q', {0,2}},
			{'r', {1,2}},
			{'s', {2,2}},
			{'t', {3,2}},
			{'u', {4,2}},
			{'v', {5,2}},
			{'w', {6,2}},
			{'x', {7,2}},

			{'y', {0,3}},
			{'z', {1,3}},
			{'!', {2,3}},

			{'0', {3,3}},
			{'1', {4,3}},
			{'2', {5,3}},
			{'3', {6,3}},
			{'4', {7,3}},

			{'5', {0,4}},
			{'6', {1,4}},
			{'7', {2,4}},
			{'8', {3,4}},
			{'9', {4,4}},

			{'.', {5,4}},
			{':', {6,4}},
			{'?', {7,3}},
			{' ', {7,6}},
			{'/', {0,5}},
			{'-', {1,5}},
		});

	m_TitleText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::upper_edge, 
		L"flappy::bird"));
	m_TitleText->set_model_matrix({ 0, 0.5f, 0 }, {}, { 0.1f });
	m_TitleText->add_to_scene(m_pMainScene);

	m_VersionText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::left_lower_corner, 
		L"artwork by ld:\ropengameart.org/users/ld\r\rcode by jfcameron:\rgithub.com/jfcameron/gdk-graphics"
	));
	m_VersionText->add_to_scene(m_pMainScene);

	m_PromptText = std::make_shared<static_text_renderer>(static_text_renderer(aGraphicsContext,
		map,
		text_renderer::alignment::center,
		L"space to start"
	));
	m_PromptText->set_model_matrix({ 0, 0.0f, 0 }, {}, { 0.075f });
	m_PromptText->add_to_scene(m_pMainScene);
}

void main_menu_screen::update(float delta, float aspectRatio, std::pair<int, int> windowSize)
{
	m_VersionText->set_model_matrix({ -0.5f * aspectRatio, -0.5, 0 }, {}, { 0.04 });

	if (++m_PrompCounter % 32 == 0)
	{
		if (m_PromptText->isHidden()) m_PromptText->show();
		else m_PromptText->hide();
	}

	m_pMainCamera->set_orthographic_projection(2, 2, 0.0075, 10, aspectRatio);

	m_pMainScene->draw(windowSize);

	scenery.update(delta);

	if (m_pInput->get_key_just_pressed(gdk::keyboard::Key::Space))
	{
		m_Screens->push(m_GameScreen);
	}
}
