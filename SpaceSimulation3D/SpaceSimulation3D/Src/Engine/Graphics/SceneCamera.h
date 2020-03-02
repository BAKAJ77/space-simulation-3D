#pragma once
#include "Engine/Graphics/WindowFrame.h"

#include <glm/glm.hpp>
#include <memory>

class SceneCamera
{
private:
	std::shared_ptr<WindowFrame> m_window;

	glm::vec3 m_position;
	glm::mat4 m_view, m_projection;

	float m_fov, m_aspect, m_speed;
public:
	SceneCamera();
	SceneCamera(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float speed = 1.0f);
	~SceneCamera();

	void setPosition(const glm::vec3& pos); // Sets the position of the camera
	void setAspect(float aspect); // Sets the aspect ratio of camera
	void setSpeed(float speed); // Sets the movement speed of camera
	void SetSensitivity(float value); // Sets the sensitivity of camera
	void setFOV(float fov); // Sets the FOV angle of the camera

	void updateMovement(const float& DELTA_TIME); // Polls key events for movement
	void updateView(); // Updates the view and projection matrices of the camera
public:
	const glm::vec3& getPosition() const; // Returns the position of the camera

	const glm::mat4& getViewMatrix() const; // Returns the view matrix
	const glm::mat4& getProjectionMatrix() const; // Returns the projection matrix
};