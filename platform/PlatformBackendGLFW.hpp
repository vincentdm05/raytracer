#pragma once

#include "PlatformBackend.hpp"

#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

// NOTE: dependency to GLFW
class PlatformBackendGLFW : public PlatformBackend
{
private:
	GLFWwindow *window = nullptr;
	uint windowWidth = 0;
	uint windowHeight = 0;
	std::string windowName;

	static void errorCallback(int errorCode, const char *errorMessage);
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

void PlatformBackendGLFW::errorCallback(int errorCode, const char *errorMessage)
{
	std::string errorName;
	switch (errorCode)
	{
		case GLFW_NOT_INITIALIZED:
		{
			errorName = "GLFW_NOT_INITIALIZED";
			break;
		}
		case GLFW_NO_CURRENT_CONTEXT:
		{
			errorName = "GLFW_NO_CURRENT_CONTEXT";
			break;
		}
		case GLFW_INVALID_ENUM :
		{
			errorName = "GLFW_INVALID_ENUM ";
			break;
		}
		case GLFW_INVALID_VALUE:
		{
			errorName = "GLFW_INVALID_VALUE";
			break;
		}
		case GLFW_OUT_OF_MEMORY:
		{
			errorName = "GLFW_OUT_OF_MEMORY";
			break;
		}
		case GLFW_API_UNAVAILABLE:
		{
			errorName = "GLFW_API_UNAVAILABLE";
			break;
		}
		case GLFW_VERSION_UNAVAILABLE:
		{
			errorName = "GLFW_VERSION_UNAVAILABLE";
			break;
		}
		case GLFW_PLATFORM_ERROR:
		{
			errorName = "GLFW_PLATFORM_ERROR";
			break;
		}
		case GLFW_FORMAT_UNAVAILABLE:
		{
			errorName = "GLFW_FORMAT_UNAVAILABLE";
			break;
		}
		default:
		{
			errorName = "Unrecognized error";
		}
	};

	std::cerr << "[GLFW Platform Backend] ERROR (" << errorName << "): " << errorMessage << std::endl;
}

void PlatformBackendGLFW::keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE)
		glfwSetWindowShouldClose(window, 1);
}

PlatformBackendGLFW::PlatformBackendGLFW(uint _windowWidth, uint _windowHeight, const std::string &_windowName)
: windowWidth(_windowWidth)
, windowHeight(_windowHeight)
, windowName(_windowName)
{
	glfwSetErrorCallback(errorCallback);
}

PlatformBackendGLFW::~PlatformBackendGLFW()
{
	if (window)
		end();
}

bool PlatformBackendGLFW::init()
{
	if (!glfwInit())
	{
		std::cerr << "Unable to init GLFW." << std::endl;
		return false;
	}

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
		std::cerr << "Unable to create window." << std::endl;
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
