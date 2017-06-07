#include "hdr.h"

extern const int WIDTH;
extern const int HEIGHT;

HDR::HDR()
{
	_shader = new Shader("gui.vs", "hdr.fs");
	_exposureLocation = _shader->getUniformLocation("exposure");

	// init FBO
	glGenFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// color buffer
	glGenTextures(1, &_color);
	glBindTexture(GL_TEXTURE_2D, _color);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F,
					WIDTH, HEIGHT,
					0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, _color, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// depth buffer
	glGenRenderbuffers(1, &_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WIDTH, HEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void HDR::bindGL() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void HDR::unbindGL() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void HDR::setExposure(float exposure)
{
	_shader->setUniform1f(_exposureLocation, exposure);
}

void HDR::render(GUIRenderer * guiRenderer) const
{
	Shader * oldShader = guiRenderer->getShader();
	guiRenderer->setShader(_shader);

	GUIPtr hdr(new GUI(TexturePtr(new Texture(_color))));
	hdr->setPositionAndSize(0, 0, WIDTH, HEIGHT);
	guiRenderer->render(hdr);

	guiRenderer->setShader(oldShader);
}