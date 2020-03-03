#include "ApplicationCore.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

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

	// Setup the shaders needed
	m_modelShader = std::make_shared<ShaderProgram>("Resources/Shaders/SceneModel.glsl.vsh",
		"Resources/Shaders/SceneModel.glsl.fsh");
	m_modelShader->bindUniformBlock("Matrices", 0);

	m_skyboxShader = std::make_shared<ShaderProgram>("Resources/Shaders/Skybox.glsl.vsh",
		"Resources/Shaders/Skybox.glsl.fsh");
	m_skyboxShader->bindUniformBlock("Matrices", 0);

	// Initialize the matrices ubo
	m_matricesUBO = std::make_shared<UniformBuffer>(nullptr, 2 * sizeof(glm::mat4), GL_STATIC_DRAW);
	m_matricesUBO->setBindingPoint(0, 0, 2 * sizeof(glm::mat4));

	// Create the skybox for the scene
	std::array<std::string, 6> texturePaths
	{
		"Resources/Textures/SpaceSkybox/right.png",
		"Resources/Textures/SpaceSkybox/left.png",
		"Resources/Textures/SpaceSkybox/top.png",
		"Resources/Textures/SpaceSkybox/bottom.png",
		"Resources/Textures/SpaceSkybox/back.png",
		"Resources/Textures/SpaceSkybox/front.png"
	};

	m_sceneSkybox = std::make_shared<Skybox>(texturePaths);

	// Setup the models for the scene
	m_planet = std::make_shared<SceneModel>("Resources/Models/MarsPlanet/mars_planet.obj",
		"Resources/Textures/MarsPlanet");
	m_planet->setPosition(glm::vec3(-20.0f, 0.0f, 10.0f));
	m_planet->setScale(glm::vec3(0.01f));

	// Create the perspective camera for the scene
	m_camera = SceneCamera(m_window, glm::vec3(0.0f, 0.0f, 3.0f));
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

	m_camera.updateMovement(DELTA_TIME);
	m_camera.updateView();
}

void ApplicationCore::render() const
{
	m_window->clearScreen(glm::vec3(0.2f, 0.2f, 0.2f));

	// Update the contents of the matrices UBO
	m_matricesUBO->modifyData(&m_camera.getViewMatrix()[0][0], 0, sizeof(glm::mat4));
	m_matricesUBO->modifyData(&m_camera.getProjectionMatrix()[0][0], sizeof(glm::mat4), sizeof(glm::mat4));

	// Render the scene
	m_sceneSkybox->render(m_skyboxShader);
	m_planet->render(m_modelShader, m_camera);
}