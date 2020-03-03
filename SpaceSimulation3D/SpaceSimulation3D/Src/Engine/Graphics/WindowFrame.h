#pragma once
#include <glm/glm.hpp>

struct GLFWwindow;
typedef unsigned int uint32_t;

class WindowFrame
{
private:
	GLFWwindow* m_windowPtr;
	const uint32_t m_width, m_height;

	mutable bool m_depthTestEnabled;
private:
	void initGLFWLibrary() const; // Initializes the GLFW library
public:
	WindowFrame(const char* title, uint32_t width, uint32_t height, bool fullscreen = false);
	~WindowFrame();

	void requestClose() const; // Requests for the window to close
	void setCursorState(bool enabled) const; // Sets whether the cursor is enabled or disabled (locked to window)
	void setDepthTestState(bool enabled) const; // Sets whether depth testing is enabled or not

	void updateTick() const; // Listens and polls events and swaps the rendering buffers
	void clearScreen(const glm::vec3& clear_color) const; // Clears the window and fills with given color
public:
	GLFWwindow* getFramePtr() const; // Returns a pointer to the GLFWwindow object
	const uint32_t& getWidth() const; // Returns the width of the window
	const uint32_t& getHeight() const; // Returns the height of the window

	bool wasRequestedClose() const; // Returns whether the window was requested to close or not
	bool wasKeyPressed(int key) const; // Returns whether the key given was pressed or not
};