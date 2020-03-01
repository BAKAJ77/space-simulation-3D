#include "WindowFrame.h"
#include "Engine/Utils/LoggingManager.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

WindowFrame::WindowFrame(const char* title, uint32_t width, uint32_t height, bool fullscreen) :
	m_width(width), m_height(height), m_depthTestEnabled(false)
{
	this->initGLFWLibrary();

	// Create the window and make sure its successfully created
	if (fullscreen)
		m_windowPtr = glfwCreateWindow(width, height, title, glfwGetPrimaryMonitor(), nullptr);
	else
		m_windowPtr = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!m_windowPtr)
		OutputLog("Failed to create window successfully!", Logging::Severity::FATAL);

	glfwMakeContextCurrent(m_windowPtr);

	// Center the window's position on the monitor
	const GLFWvidmode* videoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	const uint32_t& WIDTH = videoMode->width;
	const uint32_t& HEIGHT = videoMode->height;

	glfwSetWindowPos(m_windowPtr, (WIDTH / 2) - (width / 2), (HEIGHT / 2) - (height / 2));

	// Finally initialize GLAD
	if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
		OutputLog("A problem occurred while initializing GLAD!", Logging::Severity::FATAL);

	glDisable(GL_DEPTH_TEST);
}

WindowFrame::~WindowFrame()
{
	glfwTerminate();
}

void WindowFrame::initGLFWLibrary() const
{
	if (glfwInit() < 0)
		OutputLog("A problem occurred while initializing GLFW!", Logging::Severity::FATAL);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, false);
}

void WindowFrame::requestClose() const
{
	glfwSetWindowShouldClose(m_windowPtr, true);
}

void WindowFrame::setCursorState(bool enabled) const
{
	if (enabled)
		glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	else
		glfwSetInputMode(m_windowPtr, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void WindowFrame::setDepthTestState(bool enabled) const
{
	if (enabled)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
}

void WindowFrame::updateTick() const
{
	glfwPollEvents();
	glfwSwapBuffers(m_windowPtr);
}

void WindowFrame::clearScreen(const glm::vec3& clear_color) const
{
	glClearColor(clear_color.r, clear_color.g, clear_color.b, 1.0f);

	if (m_depthTestEnabled)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	else
		glClear(GL_COLOR_BUFFER_BIT);
}

GLFWwindow* WindowFrame::getFramePtr() const
{
	return m_windowPtr;
}

const uint32_t& WindowFrame::getWidth() const
{
	return m_width;
}

const uint32_t& WindowFrame::getHeight() const
{
	return m_height;
}

bool WindowFrame::wasRequestedClose() const
{
	return glfwWindowShouldClose(m_windowPtr);
}

bool WindowFrame::wasKeyPressed(int key) const
{
	return glfwGetKey(m_windowPtr, key) == GLFW_PRESS;
}