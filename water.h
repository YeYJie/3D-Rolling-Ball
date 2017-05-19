#ifndef _WATER_H_
#define _WATER_H_

#include "include.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"

class Water
{
public:

	Water() = default;

	Water(glm::vec3 position, float scale)
		: _position(position), _scale(scale) {}

	Water(float x, float y, float z, float scale)
		: _position(glm::vec3(x, y, z)), _scale(scale) {}

	glm::vec3 getPosition() const { 
		return _position; 
	}

	void setPosition(glm::vec3 position) {
		_position = position;
	} 

	void setScale(float s) {
		_scale = s;
	}

	glm::mat4 getModelMatrix() const {
		glm::mat4 res = glm::translate(glm::mat4(1.0f), _position);
		res = glm::scale(res, glm::vec3(_scale, 0.0f, _scale));
		return res;
	}

private:

	glm::vec3 _position = glm::vec3(0.0f);

	float _scale = 1.0f;

}; // class Water

class WaterFrameBuffer
{

public:

	WaterFrameBuffer();

	void bindReflectionBuffer() const;
	void bindRefractionBuffer() const;
	void unbindCurrentFrameBuffer() const;

	GLuint getReflectionTexture() const;
	GLuint getRefractionTexture() const;
	// GLuint getRefractionDepthTexture() const;

private:
	void bindFrameBuffer(GLuint framebuffer,
							const int width, const int height) const;

private:

	static const int FRAMEBUFFERWIDTH = 500;
	static const int FRAMEBUFFERHEIGHT = 500;

	GLuint _reflectionFBO;
	GLuint _reflectionColor;
	// GLuint _reflectionDepth;

	GLuint _refractionFBO;
	GLuint _refractionColor;
	// GLuint _refractionDepth;

}; // class WaterFrameBuffer

class WaterRenderer
{

public:

	WaterRenderer(Shader * shader, 
					glm::mat4 projectionMatrix,
					WaterFrameBuffer * waterFrameBuffer,
					Texture * dudv);

	void render(const vector<Water> & waters,
				const Camera * camera) const;

private:

	Shader * _shader;

	WaterFrameBuffer * _waterFrameBuffer;

	GLuint _VAO;
	GLuint _VBO;

	Texture * _dudv;

private:

	void initGL();

}; // class WaterRenderer

#endif