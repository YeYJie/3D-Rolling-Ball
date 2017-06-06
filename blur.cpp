#include "blur.h"

GaussBlur::GaussBlur(int width, int height)
	: _shader(new GaussShader("blur.vs", "blur.fs"))
{
	// we need two framebuffer
	glGenFramebuffers(2, _FBO);
	glGenTextures(2, _colorBuffer);

	for (int i = 0; i < 2; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO[i]);
		glBindTexture(GL_TEXTURE_2D, _colorBuffer[i]);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F,		// rgb 16
					width, height,
					0, GL_RGB, GL_FLOAT, NULL);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
								GL_TEXTURE_2D, _colorBuffer[i], 0);
		// glBindTexture(GL_TEXTURE_2D, 0);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// we need a quad
	static vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
									vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

GaussBlur::~GaussBlur()
{
	glDeleteBuffers(1, &_VBO);
	glDeleteVertexArrays(1, &_VAO);
	glDeleteTextures(2, _colorBuffer);
	glDeleteFramebuffers(2, _FBO);
}

GLuint GaussBlur::blur(TexturePtr texture)
{
	return blur(texture->getTextureRaw());
}

GLuint GaussBlur::blur(GLuint texture)
{
	int horizontal = 1;
	bool first_iteration = true;
	int n = 15;

	_shader->bindGL();

	glBindVertexArray(_VAO);
	glEnableVertexAttribArray(0);

	for (int i = 0; i < n; i++)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, _FBO[horizontal]);
		_shader->setHorizontal(horizontal);

		if(first_iteration)
			glBindTexture(GL_TEXTURE_2D, texture);
		else
			glBindTexture(GL_TEXTURE_2D, _colorBuffer[(horizontal + 1) % 2]);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		horizontal = (horizontal + 1) % 2;
		first_iteration = false;
	}

	_shader->unbindGL();

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return _colorBuffer[(horizontal + 1) % 2];
	// return GUIPtr(new GUI(TexturePtr(new Texture(_colorBuffer[(horizontal + 1) % 2]))));
}