#include "ApplicationCore.h"
#include "Engine/Utils/RandomGenerator.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>

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

	// Setup the shaders needed
	m_planetShader = std::make_shared<ShaderProgram>("Resources/Shaders/Planet.glsl.vsh",
		"Resources/Shaders/Planet.glsl.fsh");
	m_planetShader->bindUniformBlock("Matrices", 0);

	m_asteroidShader = std::make_shared<ShaderProgram>("Resources/Shaders/AsteroidInstancing.glsl.vsh",
		"Resources/Shaders/AsteroidInstancing.glsl.fsh");
	m_asteroidShader->bindUniformBlock("Matrices", 0);

	m_skyboxShader = std::make_shared<ShaderProgram>("Resources/Shaders/Skybox.glsl.vsh",
		"Resources/Shaders/Skybox.glsl.fsh");
	m_skyboxShader->bindUniformBlock("Matrices", 0);

	m_multisamplingShader = std::make_shared<ShaderProgram>("Resources/Shaders/Multisampling.glsl.vsh",
		"Resources/Shaders/Multisampling.glsl.fsh");

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

	// Setup the planet model
	m_planet = std::make_shared<SceneModel>("Resources/Models/MarsPlanet/mars_planet.obj",
		"Resources/Textures/MarsPlanet");
	m_planet->setPosition(glm::vec3(-20.0f, 0.0f, 10.0f));
	m_planet->setScale(glm::vec3(0.01f));

	// Setup the asteroid instances on the scene
	constexpr int NUM_ASTEROIDS = 10000;
	glm::mat4* instancedArray = new glm::mat4[NUM_ASTEROIDS];
	
	for (int index = 0; index < NUM_ASTEROIDS; index++)
	{
		// Generate a position for the asteroid
		const float RADIUS = 14.0f;
		float angle = static_cast<float>(index) / static_cast<float>(NUM_ASTEROIDS) * 360.0f;
		float xOffset = Random::generateFloat(-2.5f, 4.0f);
		float yOffset = Random::generateFloat(-0.5f, 0.5f);
		float zOffset = Random::generateFloat(-2.5f, 4.0f);

		glm::vec3 position;
		position.x = m_planet->getPosition().x + (cos(angle) * RADIUS) + xOffset;
		position.y = yOffset;
		position.z = m_planet->getPosition().z + (sin(angle) * RADIUS) + zOffset;

		// Generate a scale for the asteroid
		float scale = Random::generateFloat(0.03f, 0.1f);

		// Generate rotation for the asteroid
		glm::vec3 rotationAxis = glm::vec3(Random::generateFloat(0.0f, 1.0f), Random::generateFloat(0.0f, 1.0f),
			Random::generateFloat(0.0f, 1.0f));
		float rotationAngle = Random::generateFloat(0.0f, 360.0f);

		glm::mat4 model;
		model = glm::translate(model, position);
		model = glm::scale(model, glm::vec3(scale));
		model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);

		instancedArray[index] = model;
	}

	m_asteroid = std::make_shared<SceneModel>("Resources/Models/Asteroid/rock.obj", "Resources/Textures/Asteroid", 
		32.0f, instancedArray, NUM_ASTEROIDS);

	// Create the perspective camera for the scene
	m_camera = SceneCamera(m_window, glm::vec3(0.0f, 0.0f, 3.0f));

	// Setup the VAO for the quad
	float vertices[]
	{
	  // Vertex pos  // Texture coord
		0.5f, -0.5f, 1.0f, 0.0f,
	   -0.5f, -0.5f, 0.0f, 0.0f,
	    0.5f,  0.5f, 1.0f, 1.0f,
	   -0.5f,  0.5f, 0.0f, 1.0f
	};

	auto quadVBO = std::make_shared<VertexBuffer>(vertices, sizeof(vertices), GL_STATIC_DRAW);
	m_quadVAO = std::make_shared<VertexArray>();
	m_quadVAO->pushLayout<float>(0, 2, 4 * sizeof(float));
	m_quadVAO->pushLayout<float>(1, 2, 4 * sizeof(float), 2 * sizeof(float));
	m_quadVAO->attachBufferObjects(quadVBO);

	// Lastly setup the multisampling FBO
	m_multisampleFBO = std::make_shared<FrameBuffer>();
	m_multisampleFBO->attachColorBuffer(GL_TEXTURE_2D_MULTISAMPLE, static_cast<int>(m_window->getWidth()), 
		static_cast<int>(m_window->getHeight()));
	m_multisampleFBO->attachDepthStencilRBO(static_cast<int>(m_window->getWidth()), 
		static_cast<int>(m_window->getHeight()));
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
	///////////////////// RENDER THE SCENE /////////////////////
	m_multisampleFBO->bindBuffer();

	m_window->setDepthTestState(true);
	m_window->clearScreen(glm::vec3(0.2f, 0.2f, 0.2f));

	// Update the contents of the matrices UBO
	m_matricesUBO->modifyData(&m_camera.getViewMatrix()[0][0], 0, sizeof(glm::mat4));
	m_matricesUBO->modifyData(&m_camera.getProjectionMatrix()[0][0], sizeof(glm::mat4), sizeof(glm::mat4));

	// Update rotation of the planet
	m_planet->setRotation(glm::vec3(1.0, 1.0f, 0.0f), static_cast<float>(glfwGetTime() * 1.5f));

	// Render the scene
	m_sceneSkybox->render(m_skyboxShader);
	m_planet->render(m_planetShader, m_camera);
	m_asteroid->render(m_asteroidShader, m_camera);

	///////////////////// RENDER THE QUAD FOR THE SCENE TO BE DISPLAYED ON /////////////////////
	m_multisampleFBO->unbindBuffer();

	m_window->setDepthTestState(false);
	m_window->clearScreen(glm::vec3(0.0f));

	m_multisamplingShader->bindProgram();
	m_multisamplingShader->setUniform("numSamples", 4);
	m_multisamplingShader->setUniform("colorAttachment0", 0);

	m_multisampleFBO->bindColorAttachment(0);
	m_quadVAO->bind();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
}