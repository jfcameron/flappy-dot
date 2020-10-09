// © 2020 Joseph Cameron - All Rights Reserved

#include <jfc/game.h>
#include <jfc/Text_Sheet.png.h>

#include <memory>
#include <chrono>
#include <random>

using namespace flappy;
using namespace gdk;

static size_t g_HighScore(0);

static size_t increment_pipeCounter(size_t& pipeCounter, size_t size)
{
	if (pipeCounter++; pipeCounter >= size) pipeCounter = 0;

	return pipeCounter;
}

game::game(graphics::context::context_shared_ptr_type pGraphicsContext,
	input::context::context_shared_ptr_type aInputContext,
	audio::context::context_shared_ptr_type aAudio,
	screen_stack_ptr_type aScreens,
	std::shared_ptr<flappy::event_bus> aEventBus,
	flappy::assets::shared_ptr aAssets)
	: pInputContext(aInputContext)
	, pGameScene(gdk::graphics::context::scene_shared_ptr_type(std::move(pGraphicsContext->make_scene())))
	, pMainCamera(std::shared_ptr<gdk::camera>(std::move(pGraphicsContext->make_camera())))
	, scenery(flappy::scenery(pGraphicsContext, pGraphicsContext->get_alpha_cutoff_shader(), pGameScene, aAssets))
	, bird(flappy::bird(pGraphicsContext, pGameScene, pInputContext, aAudio, aAssets))
	, m_screens(aScreens)
	, m_EventBus(aEventBus)
	, m_menu(std::make_shared<decltype(m_menu)::element_type>(gdk::menu(
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::UpArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::DownArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::LeftArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::RightArrow);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::Enter);},
		[&]() {return pInputContext->get_key_just_pressed(keyboard::Key::Escape);})))
	, m_BirdObserver(std::make_shared<bird::state_machine_type::observer_type>(
		[&](bird::state aOld, bird::state aNew)
		{
			if (aOld == bird::state::alive && aNew == bird::state::dead)
			{
				updateHighScore();

				m_EventBus->propagate_player_died_event({ m_Score });

				m_menu->push(m_game_over_pane);

				m_Mode = decltype(m_Mode)::dead;
			}
		}))
{
	bird.add_observer(m_BirdObserver);
	
	pGameScene->add_camera(pMainCamera);

	for (size_t i(0); i < 10; ++i) clouds.push_back(flappy::cloud(pGraphicsContext, pGameScene, aAssets));

	for (size_t i(0); i < 1; ++i) cities.push_back(flappy::city(pGraphicsContext, pGameScene, aAssets));

	for (size_t i(0); i < 30; ++i) pipes.push_back(flappy::pipe(pGraphicsContext, pGameScene, aAssets));

	// text stuff
	auto pTextTexture = aAssets->get_textmap();

	text_map map = aAssets->get_textmap();

	pScoreText = std::make_shared<dynamic_text_renderer>(dynamic_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge));
	pScoreText->set_model_matrix({ 0, 0.5f, 0 }, {}, { 0.1f });
	pScoreText->add_to_scene(pGameScene);

	pHighScoreText = std::make_shared<dynamic_text_renderer>(dynamic_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge));
	pHighScoreText->set_model_matrix({ 0, 0.25f, 0 }, {}, { 0.05f });
	pHighScoreText->hide();
	pHighScoreText->add_to_scene(pGameScene);
	
	updateHighScore();

	pRetryText = std::make_shared<static_text_renderer>(static_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge,
		L"retry"));
	pRetryText->set_model_matrix({ 0.2f, 0.1f, 0 }, {}, { 0.05f });
	pRetryText->hide();
	pRetryText->add_to_scene(pGameScene);

	pQuitText = std::make_shared<static_text_renderer>(static_text_renderer(pGraphicsContext,
		map,
		text_renderer::alignment::upper_edge,
		L"quit"));
	pQuitText->set_model_matrix({ -0.2f, 0.1f, 0 }, {}, { 0.05f });
	pQuitText->hide();
	pQuitText->add_to_scene(pGameScene);

	m_game_over_pane = pane::make_pane();
	{
		auto retry = m_game_over_pane->make_element();
		auto quit = m_game_over_pane->make_element();

		auto gainedFocus = [&](std::shared_ptr<static_text_renderer> p)
		{
			if (m_pCurrentText) m_pCurrentText->show();

			m_pCurrentText = p;
		};

		retry->set_west_neighbour(quit);
		retry->set_on_just_gained_focus([=]() {gainedFocus(pRetryText);});
		retry->set_on_activated([this, aEventBus]() 
		{
			aEventBus->propagate_player_wants_to_reset_event({this});
		});

		quit->set_east_neighbour(retry);
		quit->set_on_just_gained_focus([=]() {gainedFocus(pQuitText);});
		quit->set_on_activated([this, aEventBus]()
		{
			aEventBus->propagate_player_wants_to_quit_event({this});
		});
	}

	m_game_over_pane->set_on_just_gained_top([&]()
	{
		pHighScoreText->show();
		pRetryText->show();
		pQuitText->show();
	});

	m_game_over_pane->set_on_just_lost_top([&]()
	{
		pHighScoreText->hide();
		pRetryText->hide();
		pQuitText->hide();
	});

	m_Random.seed(std::chrono::system_clock::now().time_since_epoch().count());

	static const auto standard_horizontal_delay = 0.5f;
	static const auto minimum_height = -0.85f;
	static const auto maximum_height = -0.25f;
	static const auto vertical_interval = 1.305f;

	// the different pipe layouts
	// Classic vertical bars, spaced at 2 intervals
	m_PipeBehaviours[0] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		auto raw_random = random() % 8;
		auto random_element = raw_random / 7.f;

		const auto vertical_range = (maximum_height - minimum_height);

		auto height = minimum_height + (random_element * vertical_range);

		if (raw_random != 0)
		{
			pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,height },
				0,
				flappy::pipe::set_up_model::up_pipe);
		}

		if (raw_random != 7)
		{
			pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,height + vertical_interval },
				3.1415926536f,
				flappy::pipe::set_up_model::down_pipe);
		}

		delay = standard_horizontal_delay * 2;
	};

	// Sinewave
	/*m_PipeBehaviours[0] = [](decltype(pipes)& pipes, decltype(pipeCounter)& counter, decltype(pipeDelay)& delay, decltype(m_Random)& random)
	{
		//shouldnt be static should be a member variable. i think these pipe layouts shoudl be classes.
		static float i = 0;

		auto raw_random = ((1 + (float)std::sin(i+=0.1f)) / 2) * 7;// random() % 8;

		std::cout << raw_random << "\n";

		auto random_element = raw_random / 7.f;

		const auto vertical_range = (maximum_height - minimum_height);// *random_element;

		auto height = minimum_height + (random_element * vertical_range);

		if (raw_random >= 0.5f)
		{
			pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,height },
				0,
				flappy::pipe::set_up_model::up_pipe);
		}

		if (raw_random < 6.5f)
		{
			pipes[increment_pipeCounter(counter, pipes.size())].set_up({ 2,height + vertical_interval },
				3.1415926536f,
				flappy::pipe::set_up_model::down_pipe);
		}

		delay = standard_horizontal_delay * 1;
	};*/
}

void flappy::game::updateHighScore()
{
	if (m_Score > g_HighScore) g_HighScore = m_Score;

	pHighScoreText->update_text(L"high score:\n" + std::to_wstring(g_HighScore));
}

void game::update(float deltaTime, 
	float aspectRatio, 
	std::pair<int, int> windowSize, 
	std::pair<float, float> vpUpperLeft, 
	std::pair<float, float> vpSize)
{
	pMainCamera->set_orthographic_projection(2, 2, 0.01f, 10, aspectRatio);
	pMainCamera->set_viewport(vpUpperLeft.first, vpUpperLeft.second, vpSize.first, vpSize.second);

	pGameScene->draw(windowSize);

	scenery.update(deltaTime);

	for (auto& pipe : pipes) pipe.update(deltaTime, pInputContext);

	for (auto& cloud : clouds) cloud.update(deltaTime);

	for (auto& city : cities) city.update(deltaTime);

	pScoreText->update_text(std::to_wstring(m_Score));

	switch (m_Mode)
	{	
		case decltype(m_Mode)::playing:
		{
			if ((m_ScoreIncrementer += deltaTime) > 0.8f)
			{
				m_Score++;
				m_ScoreIncrementer = 0;
			}

			bird.update(deltaTime, pipes);

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
			if (++m_PrompCounter % BLINK_RATE == 0)
			{
				m_BlinkStatus = !m_BlinkStatus;

				if (m_pCurrentText)
				{
					if (m_BlinkStatus) m_pCurrentText->show();
					else m_pCurrentText->hide();
				}
			}

			m_menu->update();
		} break;

		default: break;
	}
}
