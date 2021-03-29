#pragma once

#include "Common.hpp"

#include "GpuBackend.hpp"

#include "Framebuffer.hpp"

#include <GL/glew.h>
#include <iostream>
#include <string>

struct OpengGLTextureDesc
{
	GLenum target;
	GLint level;
	GLint internalformat;
	GLsizei width;
	GLsizei height;
	GLint border;
	GLenum format;
	GLenum type;

	OpengGLTextureDesc(const Framebuffer &image);
};

OpengGLTextureDesc::OpengGLTextureDesc(const Framebuffer &image)
{
	FramebufferDesc framebufferDesc = image.getDesc();

	// Only support a limited subset of textures for the moment
	target = GL_TEXTURE_2D;
	level = 0;
	width = framebufferDesc.width;
	height = framebufferDesc.height;
	border = 0;

	switch (framebufferDesc.format)
	{
		case FBFormat_r32f:
		{
			internalformat = GL_R32F;
			format = GL_RED;
			type = GL_FLOAT;
			break;
		}
		case FBFormat_r32ui:
		{
			internalformat = GL_R32UI;
			format = GL_RED;
			type = GL_UNSIGNED_INT;
			break;
		}
		case FBFormat_r32si:
		{
			internalformat = GL_R32I;
			format = GL_RED;
			type = GL_FLOAT;
			break;
		}
		case FBFormat_r32g32b32f:
		{
			internalformat = GL_RGB32F;
			format = GL_RGB;
			type = GL_FLOAT;
			break;
		}
		case FBFormat_r32g32b32ui:
		{
			internalformat = GL_RGB32UI;
			format = GL_RGB;
			type = GL_UNSIGNED_INT;
			break;
		}
		case FBFormat_r32g32b32si:
		{
			internalformat = GL_RGB32I;
			format = GL_RGB;
			type = GL_INT;
			break;
		}
		default:
		{
			internalformat = GL_R32F;
			format = GL_RED;
			type = GL_FLOAT;
		}
	};
}

class GpuBackendOpenGL : public GpuBackend
{
private:
	GLuint resolveShaderHandle = 0;
	GLuint displayTexture = 0;
	GLint resolveAttribLocationTex = 0;

	bool checkGLErrors(const std::string &contextString = "") const;
	bool checkShader(GLuint handle, const char *shaderName) const;
	bool checkProgram(GLuint handle) const;

public:
	GpuBackendOpenGL() {}

	virtual bool init() override;
	virtual bool end() override;
	virtual bool render() override;

	virtual void updateDisplayImage(const Framebuffer &image) override;
	virtual void setViewport(int x, int y, uint width, uint height) override;
};

bool GpuBackendOpenGL::checkGLErrors(const std::string &contextString) const
{
	bool noError = true;
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR)
	{
		noError = false;

		std::string errorName;
		switch (errorCode)
		{
			case GL_INVALID_ENUM:
			{
				errorName = "GL_INVALID_ENUM";
				break;
			}
			case GL_INVALID_VALUE:
			{
				errorName = "GL_INVALID_VALUE";
				break;
			}
			case GL_INVALID_OPERATION:
			{
				errorName = "GL_INVALID_OPERATION";
				break;
			}
			case GL_INVALID_FRAMEBUFFER_OPERATION:
			{
				errorName = "GL_INVALID_FRAMEBUFFER_OPERATION";
				break;
			}
			case GL_OUT_OF_MEMORY:
			{
				errorName = "GL_OUT_OF_MEMORY";
				break;
			}
			case GL_STACK_UNDERFLOW:
			{
				errorName = "GL_STACK_UNDERFLOW";
				break;
			}
			case GL_STACK_OVERFLOW:
			{
				errorName = "GL_STACK_OVERFLOW";
				break;
			}
			default:
			{
				errorName = "Unrecognized error";
			}
		};

		std::cerr << "[OpenGL GPU Backend] ERROR (" << errorName << ")" <<
		(contextString.empty() ? "" : " in ") << contextString << std::endl;
	}

	return noError;
}

bool GpuBackendOpenGL::checkShader(GLuint handle, const char *shaderName) const
{
	GLint status = 0, logLength = 0;
	glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
	glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logLength);

	if ((GLboolean)status == GL_FALSE)
		std::cerr << "[OpenGL GPU Backend] ERROR: " << shaderName << " compilation failed!" << std::endl;

	if (logLength > 1)
	{
		std::string log;
		log.resize((int)(logLength + 1));
		glGetShaderInfoLog(handle, logLength, nullptr, (GLchar*)log.data());
		std::cerr << log << std::endl;
	}

	return (GLboolean)status == GL_TRUE;
}

bool GpuBackendOpenGL::checkProgram(GLuint handle) const
{
	GLint status = 0, logLength = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);
	glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logLength);

	if ((GLboolean)status == GL_FALSE)
		std::cerr << "[OpenGL GPU Backend] ERROR: failed to link shader program!" << std::endl;

	if (logLength > 1)
	{
		std::string log;
		log.resize((int)(logLength + 1));
		glGetProgramInfoLog(handle, logLength, nullptr, (GLchar*)log.data());
		std::cerr << log << std::endl;
	}
	return (GLboolean)status == GL_TRUE;
}

bool GpuBackendOpenGL::init()
{
	if (glewInit() != GLEW_OK)
		return false;

	const GLchar *vertexShader =
		"#version 410\n"
		"out vec2 texCoord;\n"
		"void main()\n"
		"{\n"
		"	float x = -1.0 + float((gl_VertexID & 1) << 2);\n"
		"	float y = -1.0 + float((gl_VertexID & 2) << 1);\n"
		"	texCoord.x = (x + 1.0) * 0.5;\n"
		"	texCoord.y = (y + 1.0) * 0.5;\n"
		"	gl_Position = vec4(x, y, 0.0, 1.0);\n"
		"}\n";

	GLuint vertHandle = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertHandle, 1, &vertexShader, nullptr);
	glCompileShader(vertHandle);
	if (!checkShader(vertHandle, "vertex shader"))
		return false;

	const GLchar *fragmentShader =
		"#version 410\n"
		"in vec2 texCoord;\n"
		"uniform sampler2D Texture;\n"
		"layout (location = 0) out vec4 Color;\n"
		"void main()\n"
		"{\n"
		"	Color = texture(Texture, texCoord.xy);\n"
		"}\n";

	GLuint fragHandle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragHandle, 1, &fragmentShader, nullptr);
	glCompileShader(fragHandle);
	if (!checkShader(fragHandle, "fragment shader"))
		return false;

	resolveShaderHandle = glCreateProgram();
	glAttachShader(resolveShaderHandle, vertHandle);
	glAttachShader(resolveShaderHandle, fragHandle);
	glLinkProgram(resolveShaderHandle);
	if (!checkProgram(resolveShaderHandle))
		return false;

	// Clean up once program is linked
	glDetachShader(resolveShaderHandle, vertHandle);
	glDetachShader(resolveShaderHandle, fragHandle);
	glDeleteShader(vertHandle);
	glDeleteShader(fragHandle);
	vertHandle = 0;
	fragHandle = 0;

	resolveAttribLocationTex = glGetUniformLocation(resolveShaderHandle, "Texture");

	const byte texInitData[] =
	{
		0xff, 0x00, 0x00, 0xff,		0xff, 0x00, 0xff, 0xff,
		0x00, 0xff, 0x00, 0xff,		0x00, 0x00, 0xff, 0xff
	};

	glGenTextures(1, &displayTexture);
	glBindTexture(GL_TEXTURE_2D, displayTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_UNSIGNED_BYTE, texInitData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	return checkGLErrors();
}

bool GpuBackendOpenGL::end()
{
	glDeleteProgram(resolveShaderHandle);
	resolveShaderHandle = 0;
	resolveAttribLocationTex = 0;

	glDeleteTextures(1, &displayTexture);
	displayTexture = 0;

	return checkGLErrors();
}

bool GpuBackendOpenGL::render()
{
	glClearColor(0.15, 0.1, 0.15, 1.0);
	glClear(GL_COLOR_BUFFER_BIT);

	// Full-screen draw
	{
		glDisable(GL_BLEND);
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glDisable(GL_STENCIL_TEST);
		glDisable(GL_SCISSOR_TEST);

		glUseProgram(resolveShaderHandle);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, displayTexture);
		glUniform1i(resolveAttribLocationTex, 0);

		GLuint vaoHandle;
		glGenVertexArrays(1, &vaoHandle);
		glBindVertexArray(vaoHandle);

		glDrawArrays(GL_TRIANGLES, 0, 3);

		glDeleteVertexArrays(1, &vaoHandle);

		// Reset state
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}

	static int frameNumber = 0;
	return checkGLErrors(std::string("frame #") + std::to_string(frameNumber++));
}

void GpuBackendOpenGL::updateDisplayImage(const Framebuffer &image)
{
	glBindTexture(GL_TEXTURE_2D, displayTexture);
	const byte *data = image.getData();

	OpengGLTextureDesc desc(image);
	glTexImage2D(desc.target, desc.level, desc.internalformat, desc.width, desc.height, desc.border, desc.format, desc.type, data);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void GpuBackendOpenGL::setViewport(int x, int y, uint width, uint height)
{
	glViewport(x, y, width, height);
}
