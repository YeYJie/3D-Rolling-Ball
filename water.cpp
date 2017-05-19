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

	// glGenTextures(1, &_reflectionDepth);
	// glBindTexture(GL_TEXTURE_2D, _reflectionDepth);

	// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT,
	// 				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	// 						GL_TEXTURE_2D, _reflectionDepth, 0);
	// glBindTexture(GL_TEXTURE_2D, 0);


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

	// glGenTextures(1, &_refractionDepth);
	// glBindTexture(GL_TEXTURE_2D, _refractionDepth);

	// glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, FRAMEBUFFERWIDTH, FRAMEBUFFERHEIGHT,
	// 				0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	// 						GL_TEXTURE_2D, _refractionDepth, 0);
	// glBindTexture(GL_TEXTURE_2D, 0);


	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
	glViewport(0, 0, width, height);	
}

GLuint WaterFrameBuffer::getReflectionTexture() const
{
	return _reflectionColor;
}

GLuint WaterFrameBuffer::getRefractionTexture() const
{
	return _refractionColor;
}

// GLuint WaterFrameBuffer::getRefractionDepthTexture() const
// {
// 	return _refractionDepth;
// }


/************************************************
*
*		WaterRenderer
*
************************************************/

WaterRenderer::WaterRenderer(Shader * shader, glm::mat4 projectionMatrix,
								WaterFrameBuffer * waterFrameBuffer, 
								Texture * dudv)
	// : _dudv("dudv.png")
{
	_shader = shader;
	_waterFrameBuffer = waterFrameBuffer;
	// _dudv = new Texture("dudv.png");
	_dudv = dudv;

	_shader->bindGL();
	_shader->setProjectionMatrix(projectionMatrix);

	_shader->setUniform1i("reflectionTexture", 0);
	_shader->setUniform1i("refractionTexture", 1);
	_shader->setUniform1i("dudvTexture", 2);

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

void WaterRenderer::render(const vector<Water> & waters,
							const Camera * camera) const
{
	_shader->bindGL();
	_shader->setViewMatrix(camera->getViewMatrix());

	static float distortionOffset = 0.0f;
	_shader->setUniform1f("distortionOffset", distortionOffset);
	distortionOffset += 0.0001f;
	// distortionOffset %= 1;
	if(distortionOffset > 1.0f) distortionOffset -= 1.0f;

	glBindVertexArray(_VAO);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _waterFrameBuffer->getReflectionTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _waterFrameBuffer->getRefractionTexture());

	// glActiveTexture(GL_TEXTURE2);
	// glBindTexture(GL_TEXTURE_2D, dudvTexture);
	_dudv->bindGL(2);

	// glActiveTexture(GL_TEXTURE3);
	// glBindTexture(GL_TEXTURE_2D, normalMap);
	// glActiveTexture(GL_TEXTURE4);
	// glBindTexture(GL_TEXTURE_2D, fbos.getRefractionDepthTexture());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	for(auto i : waters) {
		_shader->setModelMatrix(i.getModelMatrix());
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	glDisable(GL_BLEND);
	_shader->unbindGL();
}