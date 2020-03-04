#pragma once
#include "Engine/Graphics/WindowFrame.h"
#include "Engine/Graphics/ShaderPrograms.h"
#include "Engine/Graphics/SceneCamera.h"
#include "Engine/Graphics/SceneSkybox.h"
#include "Engine/Graphics/SceneModel.h"

#include <memory>

class ApplicationCore
{
private:
	const std::shared_ptr<WindowFrame> m_window;
	
	std::shared_ptr<FrameBuffer> m_multisampleFBO;
	std::shared_ptr<VertexArray> m_quadVAO;

	std::shared_ptr<ShaderProgram> m_planetShader;
	std::shared_ptr<ShaderProgram> m_asteroidShader;
	std::shared_ptr<ShaderProgram> m_skyboxShader;
	std::shared_ptr<ShaderProgram> m_multisamplingShader;

	std::shared_ptr<UniformBuffer> m_matricesUBO;

	std::shared_ptr<Skybox> m_sceneSkybox;
	std::shared_ptr<SceneModel> m_planet;
	std::shared_ptr<SceneModel> m_asteroid;

	SceneCamera m_camera;
private:
	void initResources(); // Initializes the resources needed for the application
	void mainLoop(); // Contains the main loop of the application

	void updateTick(const float& DELTA_TIME); // Updates the application logic per loop/tick
	void render() const; // Renders objects to the scene
public:
	ApplicationCore();
	~ApplicationCore();
};