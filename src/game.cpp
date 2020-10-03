// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/game.h>
#include <jfc/Text_Sheet.png.h>

#include <memory>
#include <chrono>
#include <random>

using namespace flappy;
using namespace gdk;

static size_t increment_pipeCounter(size_t& pipeCounter, size_t size)
{
	if (pipeCounter++; pipeCounter >= size) pipeCounter = 0;

	return pipeCounter;
}

bool game::blar()
{
	return pInputContext->get_key_down(keyboard::Key::B);
}

game::game(graphics::context::context_shared_ptr_type pGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudio)
	: pInputContext(aInputContext)
	, pGameScene(gdk::graphics::context::scene_shared_ptr_type(std::move(pGraphicsContext->make_scene())))
	, pMainCamera(std::shared_ptr<gdk::camera>(std::move(pGraphicsContext->make_camera())))
	, scenery(flappy::scenery(pGraphicsContext, pGraphicsContext->get_alpha_cutoff_shader(), pGameScene))
	, bird(flappy::bird(pGraphicsContext, pGameScene, pInputContext, aAudio))
	, m_menu(std::make_shared<decltype(m_menu)::element_type>(gdk::menu(
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::UpArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::DownArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::LeftArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::RightArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::A);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::S);})))
{
	std::cout << "blar: " << pInputContext->get_key_down(keyboard::Key::A) << "\n";

	m_Random.seed(std::chrono::system_clock::now().time_since_epoch().count());

	pGameScene->add_camera(pMainCamera);

	for (int i(0); i < 10; ++i) clouds.push_back(flappy::cloud(pGraphicsContext, pGameScene));

	for (int i(0); i < 1; ++i) cities.push_back(flappy::city(pGraphicsContext, pGameScene));

	for (int i(0); i < 20; ++i) pipes.push_back(flappy::pipe(pGraphicsContext, pGameScene));

	// text stuff
	auto pTextTexture = std::shared_ptr<gdk::texture>(std::shared_ptr<texture>(std::move(pGraphicsContext->make_texture(
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
			{'?', {7,3}},
			{' ', {7,6}},
			{':', {6,4}},
		});

	pScoreText = std::make_shared<dynamic_text_renderer>(dynamic_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge));
	pScoreText->set_model_matrix({ 0, 0.5f, 0 }, {}, { 0.1f });
	pScoreText->add_to_scene(pGameScene);

	pHighScoreText = std::make_shared<dynamic_text_renderer>(dynamic_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge,
		L"high score:\n1234"));
	pHighScoreText->set_model_matrix({ 0, 0.25f, 0 }, {}, { 0.05f });
	pHighScoreText->add_to_scene(pGameScene);
	
	pRetryText = std::make_shared<static_text_renderer>(static_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge,
		L"retry"));
	pRetryText->set_model_matrix({ 0.2, 0.1f, 0 }, {}, { 0.05f });
	pRetryText->add_to_scene(pGameScene);

	pQuitText = std::make_shared<static_text_renderer>(static_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge,
		L"quit"));
	pQuitText->set_model_matrix({ -0.2, 0.1f, 0 }, {}, { 0.05f });
	pQuitText->add_to_scene(pGameScene);

	auto game_over_pane = pane::make_pane();
	{
		auto retry = game_over_pane->make_element();
		auto quit = game_over_pane->make_element();

		retry->set_west_neighbour(quit);
		retry->set_on_activated([]() {std::cout << "retry\n";});

		quit->set_east_neighbour(retry);
		quit->set_on_activated([]() {std::cout << "quit\n";});
	}

	m_menu->push(game_over_pane);

	// the different pipe layouts
	m_PipeBehaviours[0] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,-0.6f + random() % 4 * 0.1f }, 0, flappy::pipe::set_up_model::up_pipe);

		delay = 0.5f + (random() % 2);
	};

	m_PipeBehaviours[1] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,+0.6f + random() % 4 * 0.1f }, 3.1415926536f, flappy::pipe::set_up_model::down_pipe);

		delay = 0.75f + (random() % 2);
	};

	m_PipeBehaviours[2] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		auto offset = random() % 4 * 0.1f;

		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,-0.6f + offset }, 0, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,+0.8f + offset }, 3.1415926536f, flappy::pipe::set_up_model::down_pipe);

		delay = 0.5f + (random() % 2);
	};

	m_PipeBehaviours[3] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		auto offset = 0;//random() % 4 * 0.1f;

		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,-0.5f + offset }, 0.7853981634f, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.3,-0.5f + offset }, 0.7853981634f, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.3,+0.8f + offset }, 3.1415926536f, flappy::pipe::set_up_model::down_pipe);

		delay = 0.9f + (random() % 2);
	};

	m_PipeBehaviours[4] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,-0.6f + random() % 4 * 0.1f }, 0, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.25,-0.5f + random() % 4 * 0.1f }, 0, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.5,-0.4f + random() % 4 * 0.1f }, 0, flappy::pipe::set_up_model::up_pipe);

		delay = 0.75f + (random() % 2);
	};

	m_PipeBehaviours[5] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		auto offset = random() % 4;

		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,-0.6f + offset * 0.04f }, 0, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.25,-0.7f + offset * 0.04f }, 0, flappy::pipe::set_up_model::up_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.5,-0.6f + offset * 0.04f }, 0, flappy::pipe::set_up_model::up_pipe);

		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,+0.8f + offset * 0.04f }, 3.1415926536f, flappy::pipe::set_up_model::down_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.25,+0.7f + offset * 0.04f }, 3.1415926536f, flappy::pipe::set_up_model::down_pipe);
		pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2.5,+0.8f + offset * 0.04f }, 3.1415926536f, flappy::pipe::set_up_model::down_pipe);

		delay = 3.f + (random() % 2);
	};
}

void game::update(float deltaTime, 
	float aspectRatio, 
	std::pair<int, int> windowSize, 
	std::pair<float, float> vpUpperLeft, 
	std::pair<float, float> vpSize)
{
	m_menu->update();

	pMainCamera->set_orthographic_projection(2, 2, 0.01, 10, aspectRatio);
	pMainCamera->set_viewport(vpUpperLeft.first, vpUpperLeft.second, vpSize.first, vpSize.second);

	pGameScene->draw(windowSize);

	scenery.update(deltaTime);

	for (auto& pipe : pipes) pipe.update(deltaTime, pInputContext);

	for (auto& cloud : clouds) cloud.update(deltaTime);

	for (auto& city : cities) city.update(deltaTime);

	pScoreText->update_text(std::to_wstring(deltaTime * 1000));

	switch (m_Mode)
	{	
		case decltype(m_Mode)::playing:
		{
			bird.update(deltaTime, pipes);

			auto wpos = bird.get_world_position();

			for (auto& pipe : pipes)
			{
				if (pipe.check_collision(wpos)) std::cout << "hit\n";
			}

			if (pipeDelay -= deltaTime; pipeDelay <= 0)
			{
				m_PipeBehaviours[m_Random() % m_PipeBehaviours.size()](
					pipes, 
					pipeCounter, 
					pipeDelay, 
					m_Random);
			}
		} break;

		case decltype(m_Mode)::dead:
		{

		} break;
		default: break;
	}
}
