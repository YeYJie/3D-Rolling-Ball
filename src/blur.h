#ifndef _BLUR_H_
#define _BLUR_H_

#include "include.h"
#include "texture.h"
#include "shader.h"
#include "gui.h"

class GaussShader;

class GaussBlur
{

public:

	GaussBlur(int width, int height);

	~GaussBlur();

	GLuint blur(TexturePtr texture, int n);

	GLuint blur(GLuint texture, int n);

private:

	GaussShader * _shader;

	GLuint _FBO[2];
	GLuint _colorBuffer[2];

	GLuint _VAO;
	GLuint _VBO;

private:

	void renderQuad();

}; // class GaussBlur

typedef shared_ptr<GaussBlur> GaussBlurPtr;



class GaussShader : public Shader
{

public:

	GaussShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_horizontal = getUniformLocation("horizontal");
		_texture = getUniformLocation("text");

		unbindGL();
	}

	void setHorizontal(int value) {
		setUniform1i(_horizontal, value);
	}

	void setTexture(int value) {
		setUniform1i(_texture, value);
	}

private:

	GLint _horizontal;

	GLint _texture;

}; // class GaussShader

#endif