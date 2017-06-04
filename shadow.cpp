#include "shadow.h"

extern const int WIDTH;
extern const int HEIGHT;
const int SHADOWFRAMEBUFFERWIDTH = WIDTH * 2;
const int SHADOWFRAMEBUFFERHEIGHT = HEIGHT * 2;

ShadowFrameBuffer::ShadowFrameBuffer()
{
	glGenFramebuffers(1, &_FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);

	// glGenRenderbuffers(1, &_depthRenderBuffer);
	// glBindRenderbuffer(GL_RENDERBUFFER, _depthRenderBuffer);
	// glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
	// 						SHADOWFRAMEBUFFERWIDTH, SHADOWFRAMEBUFFERHEIGHT);
	// glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	// 						GL_RENDERBUFFER, _depthRenderBuffer);

	glGenTextures(1, &_depthTexture);
	glBindTexture(GL_TEXTURE_2D, _depthTexture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
	             SHADOWFRAMEBUFFERWIDTH, SHADOWFRAMEBUFFERHEIGHT,
	             0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D,
				_depthTexture, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	// glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

ShadowFrameBuffer::~ShadowFrameBuffer()
{
	glDeleteTextures(1, &_depthTexture);
	glDeleteFramebuffers(1, &_FBO);
}

void ShadowFrameBuffer::bindGL() const
{
	// glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _FBO);
	glViewport(0, 0, SHADOWFRAMEBUFFERWIDTH, SHADOWFRAMEBUFFERHEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ShadowFrameBuffer::unbindGL() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

TexturePtr ShadowFrameBuffer::getDepthTexture() const
{
	return TexturePtr(new Texture(_depthTexture));
}

GLuint ShadowFrameBuffer::getDepthTextureRaw() const
{
	return _depthTexture;
}