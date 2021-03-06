#include "water.h"

extern const int WIDTH;
extern const int HEIGHT;

/************************************************
*
*		WaterFrameBuffer
*
************************************************/

WaterFrameBuffer::WaterFrameBuffer()
{
	// reflection FBO
	glGenFramebuffers(1, &_reflectionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _reflectionFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glGenTextures(1, &_reflectionColor);
	glBindTexture(GL_TEXTURE_2D, _reflectionColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT,
					0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, _reflectionColor, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &_reflectionDepthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _reflectionDepthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
							FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							GL_RENDERBUFFER, _reflectionDepthRenderBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// refraction FBO
	glGenFramebuffers(1, &_refractionFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _refractionFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	glGenTextures(1, &_refractionColor);
	glBindTexture(GL_TEXTURE_2D, _refractionColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT,
					0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, _refractionColor, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenRenderbuffers(1, &_refractionDepthRenderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, _refractionDepthRenderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
							FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
							GL_RENDERBUFFER, _refractionDepthRenderBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

WaterFrameBuffer::~WaterFrameBuffer()
{
	glDeleteFramebuffers(1, &_reflectionFBO);
	glDeleteTextures(1, &_reflectionColor);
	glDeleteFramebuffers(1, &_refractionFBO);
	glDeleteTextures(1, &_refractionColor);
}

void WaterFrameBuffer::bindReflectionBuffer() const
{
	bindFrameBuffer(_reflectionFBO, FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT);
}

void WaterFrameBuffer::bindRefractionBuffer() const
{
	bindFrameBuffer(_refractionFBO, FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT);
}

void WaterFrameBuffer::unbindCurrentFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);
}

void WaterFrameBuffer::bindFrameBuffer(GLuint framebuffer,
									const int width, const int height) const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, width, height);
}

TexturePtr WaterFrameBuffer::getReflectionTexture() const
{
	return TexturePtr(new Texture(_reflectionColor));
}

TexturePtr WaterFrameBuffer::getRefractionTexture() const
{
	return TexturePtr(new Texture(_refractionColor));
}

GLuint WaterFrameBuffer::getReflectionTextureRaw() const
{
	return _reflectionColor;
}

GLuint WaterFrameBuffer::getRefractionTextureRaw() const
{
	return _refractionColor;
}



/************************************************
*
*		WaterRenderer
*
************************************************/

WaterRenderer::WaterRenderer(WaterShader * shader, glm::mat4 projectionMatrix,
								WaterFrameBuffer * waterFrameBuffer,
								Texture * dudv, Texture * normal)
{
	_shader = shader;
	_waterFrameBuffer = waterFrameBuffer;
	_dudv = dudv;
	_normal = normal;

	_shader->bindGL();

	_shader->setProjectionMatrix(projectionMatrix);
	_shader->setReflectionTexture(0);
	_shader->setRefractionTexture(1);
	_shader->setDudvTexture(2);
	_shader->setNormalMap(3);

	_shader->unbindGL();
	initGL();
}

static vector<float> vertices = {
	-1, 0, -1,
	-1, 0, 1,
	1, 0, -1,
	1, 0, -1,
	-1, 0, 1,
	1, 0, 1
};

void WaterRenderer::initGL()
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
					vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	// glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void WaterRenderer::render(const vector<WaterPtr> & waters,
							const Camera * camera) const
{
	_shader->bindGL();
	_shader->setViewMatrix(camera->getViewMatrix());

	static float distortionOffset = 0.0f;
	_shader->setDistortionOffset(distortionOffset);
	distortionOffset += 0.0004f;
	// distortionOffset %= 1;
	if(distortionOffset > 1.0f) distortionOffset -= 1.0f;

	glBindVertexArray(_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _waterFrameBuffer->getReflectionTextureRaw());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _waterFrameBuffer->getRefractionTextureRaw());

	// glActiveTexture(GL_TEXTURE2);
	// glBindTexture(GL_TEXTURE_2D, dudvTexture);
	_dudv->bindGL(2);
	_normal->bindGL(3);
	// glActiveTexture(GL_TEXTURE3);
	// glBindTexture(GL_TEXTURE_2D, normalMap);
	// glActiveTexture(GL_TEXTURE4);
	// glBindTexture(GL_TEXTURE_2D, fbos.getRefractionDepthTexture());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(auto i : waters) {
		_shader->setModelMatrix(i->getModelMatrix());
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisable(GL_BLEND);
	_shader->unbindGL();
}