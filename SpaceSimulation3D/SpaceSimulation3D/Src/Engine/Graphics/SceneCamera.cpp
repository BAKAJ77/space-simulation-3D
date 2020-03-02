#include "SceneCamera.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

namespace
{
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	glm::vec3 front = { 0.0f, 0.0f, -1.0f };

	float prevX = 0.0f, prevY = 0.0f;
	float pitch = 0.0f, yaw = 90 * front.z;
	float sensitivity = 0.12f;

	bool mouseFocused = false;
}

void cursorPosCallbackFunc(GLFWwindow* window, double xpos, double ypos)
{
	if (!mouseFocused)
	{
		prevX = static_cast<float>(xpos);
		prevY = static_cast<float>(ypos);
		mouseFocused = true;
	}

	// Calculate the offset of the cursor
	float xOffset = static_cast<float>(xpos) - prevX;
	float yOffset = prevY - static_cast<float>(ypos);
	prevX = static_cast<float>(xpos);
	prevY = static_cast<float>(ypos);

	xOffset *= sensitivity;
	yOffset *= sensitivity;
	yaw += xOffset;
	pitch += yOffset;

	// Apply constraints on the pitch value
	if (pitch >= 89.0f)
		pitch = 89.0f;
	else if (pitch <= -89.0f)
		pitch = -89.0f;

	// Finally calculate the new front vector
	glm::vec3 newFront;
	newFront.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
	newFront.y = sin(glm::radians(pitch));
	newFront.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	front = glm::normalize(newFront);
}

SceneCamera::SceneCamera() :
	m_speed(1.0f), m_fov(45.0f), m_aspect(1.0f)
{}

SceneCamera::SceneCamera(std::shared_ptr<WindowFrame> window, const glm::vec3& pos, float speed) :
	m_position(pos), m_speed(speed), m_fov(45.0f), m_aspect((float)window->getWidth() / (float)window->getHeight()),
	m_window(window)
{
	glfwSetCursorPosCallback(window->getFramePtr(), cursorPosCallbackFunc);
}

SceneCamera::~SceneCamera() {}

void SceneCamera::setPosition(const glm::vec3& pos)
{
	m_position = pos;
}

void SceneCamera::setAspect(float aspect)
{
	m_aspect = aspect;
}

void SceneCamera::setSpeed(float speed)
{
	m_speed = speed;
}

void SceneCamera::SetSensitivity(float value)
{
	sensitivity = value;
}

void SceneCamera::setFOV(float fov)
{
	m_fov = fov;
}

void SceneCamera::updateMovement(const float& DELTA_TIME)
{
	const float FINAL_SPEED = m_speed * DELTA_TIME;

	// Movement along 'X' axis
	if (m_window->wasKeyPressed(GLFW_KEY_A))
	{
		m_position.x -= glm::normalize(glm::cross(front, up)).x * FINAL_SPEED;
		m_position.z -= glm::normalize(glm::cross(front, up)).z * FINAL_SPEED;
	}
	else if (m_window->wasKeyPressed(GLFW_KEY_D))
	{
		m_position.x += glm::normalize(glm::cross(front, up)).x * FINAL_SPEED;
		m_position.z += glm::normalize(glm::cross(front, up)).z * FINAL_SPEED;
	}

	// Movement along 'Z' axis
	if (m_window->wasKeyPressed(GLFW_KEY_W))
	{
		m_position.x += front.x * FINAL_SPEED;
		m_position.z += front.z * FINAL_SPEED;
	}
	else if (m_window->wasKeyPressed(GLFW_KEY_S))
	{
		m_position.x -= front.x * FINAL_SPEED;
		m_position.z -= front.z * FINAL_SPEED;
	}

	// Movement along 'Y' axis
	if (m_window->wasKeyPressed(GLFW_KEY_SPACE))
		m_position += up * FINAL_SPEED;
	else if (m_window->wasKeyPressed(GLFW_KEY_LEFT_SHIFT))
		m_position -= up * FINAL_SPEED;
}

void SceneCamera::updateView()
{
	m_view = glm::lookAt(m_position, m_position + front, up);
	m_projection = glm::perspective(glm::radians(m_fov), m_aspect, 0.1f, 100000.0f);
}

const glm::vec3& SceneCamera::getPosition() const
{
	return m_position;
}

const glm::mat4& SceneCamera::getViewMatrix() const
{
	return m_view;
}

const glm::mat4& SceneCamera::getProjectionMatrix() const
{
	return m_projection;
}