#include "ApplicationCore.h"
#include <GLFW/glfw3.h>

ApplicationCore::ApplicationCore() :
	m_window(std::make_shared<WindowFrame>("Space Simulation 3D", 1600, 900))
{
	this->initResources();
	this->mainLoop();
}

ApplicationCore::~ApplicationCore() {}

void ApplicationCore::initResources() 
{
	// Just configure the cursor state and enable depth test
	m_window->setCursorState(false);
	m_window->setDepthTestState(true);
}

void ApplicationCore::mainLoop()
{
	float previousTime = 0.0f;
	while (!m_window->wasRequestedClose())
	{
		// Calculate the delta time of each loop
		const float CURRENT_TIME = static_cast<float>(glfwGetTime());
		const float DELTA_TIME = CURRENT_TIME - previousTime;
		previousTime = CURRENT_TIME;

		this->updateTick(DELTA_TIME);
		this->render();
	}
}

void ApplicationCore::updateTick(const float& DELTA_TIME)
{
	if (m_window->wasKeyPressed(GLFW_KEY_ESCAPE))
		m_window->requestClose();

	m_window->updateTick();
}

void ApplicationCore::render() const
{
	m_window->clearScreen(glm::vec3(0.2f, 0.2f, 0.2f));
}