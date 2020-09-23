#include <jfc/game_screen.h>

#include <memory>
#include <chrono>
#include <random>

using namespace gdk;

static size_t increment_pipeCounter(size_t &pipeCounter, size_t size)
{
	if (pipeCounter++; pipeCounter >= size) pipeCounter = 0;

	return pipeCounter;
}

game_screen::game_screen(graphics::context::context_shared_ptr_type pGraphicsContext,
	input::context::context_shared_ptr_type aInputContext)
	: pInputContext(aInputContext)
	, pScene(gdk::graphics::context::scene_shared_ptr_type(std::move(pGraphicsContext->make_scene())))
	, pMainCamera(std::shared_ptr<gdk::camera>(std::move(pGraphicsContext->make_camera())))
	, scenery(flappy::scenery(pGraphicsContext, pGraphicsContext->get_alpha_cutoff_shader(), pScene))
	, bird(flappy::bird(pGraphicsContext, pScene, pInputContext))
{
	m_Random.seed(std::chrono::system_clock::now().time_since_epoch().count());

	pScene->add_camera(pMainCamera);

	for (int i(0); i < 10; ++i) clouds.push_back(flappy::cloud(pGraphicsContext, pScene));

	for (int i(0); i < 20; ++i) pipes.push_back(flappy::pipe(pGraphicsContext, pScene));

	std::cout << pipes.size() << "\n";
	
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

void game_screen::update(float deltaTime, float aspectRatio, std::pair<int, int> windowSize)
{
	pMainCamera->set_orthographic_projection(2, 2, 0.01, 10, aspectRatio);

	pScene->draw(windowSize);

	scenery.update(deltaTime);

	bird.update(deltaTime, pipes);

	for (auto &pipe : pipes) pipe.update(deltaTime, pInputContext);

	for (auto &cloud : clouds) cloud.update(deltaTime);

	pipeDelay -= deltaTime * 1;
	if ( pipeDelay <= 0)
	{
		//float pipeDelay = 0;
		m_PipeBehaviours[m_Random() % m_PipeBehaviours.size()](pipes, pipeCounter, pipeDelay, m_Random);
	}
}
