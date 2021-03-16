#pragma once

#include "PlatformBackend.hpp"

#include <GLFW/glfw3.h>

#include <string>

// NOTE: dependency to GLFW
class PlatformBackendGLFW : public PlatformBackend
{
private:
	GLFWwindow *window = nullptr;
	uint windowWidth = 0;
	uint windowHeight = 0;
	std::string windowName;

	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

public:
	PlatformBackendGLFW(uint _windowWidth, uint _windowHeight, const std::string &_windowName);
	~PlatformBackendGLFW();

	virtual bool init() override;
	virtual bool end() override;
	virtual bool isLive() const override;
	virtual bool update() override;
	virtual void close() override;
	virtual void getFramebufferSize(uint &widthOut, uint &heightOut) override;
};

void PlatformBackendGLFW::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
}

PlatformBackendGLFW::PlatformBackendGLFW(uint _windowWidth, uint _windowHeight, const std::string &_windowName)
: windowWidth(_windowWidth)
, windowHeight(_windowHeight)
, windowName(_windowName)
{}

PlatformBackendGLFW::~PlatformBackendGLFW()
{
	if (window)
		end();
}

bool PlatformBackendGLFW::init()
{
	if (!glfwInit())
		return false;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	window = glfwCreateWindow(windowWidth, windowHeight, windowName.c_str(), nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);
	// Enable vsync
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, keyCallback);

	return true;
}

bool PlatformBackendGLFW::end()
{
	glfwDestroyWindow(window);
	glfwTerminate();

	return true;
}

bool PlatformBackendGLFW::isLive() const
{
	return glfwWindowShouldClose(window) == 0;
}

bool PlatformBackendGLFW::update()
{
	glfwSwapBuffers(window);
	glfwPollEvents();

	return true;
}

void PlatformBackendGLFW::close()
{
	glfwSetWindowShouldClose(window, 1);
}

void PlatformBackendGLFW::getFramebufferSize(uint &widthOut, uint &heightOut)
{
	widthOut = 0;
	heightOut = 0;
	int width = 0;
	int height = 0;
	glfwGetFramebufferSize(window, &width, &height);	
	widthOut = uint(width > 0 ? width : 0);
	heightOut = uint(height > 0 ? height : 0);
}
