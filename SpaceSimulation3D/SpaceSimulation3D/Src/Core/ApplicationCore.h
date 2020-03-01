#pragma once
#include "Engine/Graphics/WindowFrame.h"
#include <memory>

class ApplicationCore
{
private:
	const std::shared_ptr<WindowFrame> m_window;
private:
	void initResources(); // Initializes the resources needed for the application
	void mainLoop(); // Contains the main loop of the application

	void updateTick(const float& DELTA_TIME); // Updates the application logic per loop/tick
	void render() const; // Renders objects to the scene
public:
	ApplicationCore();
	~ApplicationCore();
};