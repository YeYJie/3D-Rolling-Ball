#ifndef _WATER_H_
#define _WATER_H_

#include "include.h"
#include "shader.h"
#include "camera.h"
#include "texture.h"
#include "waterShader.h"

class Water
{
public:

	Water() = default;

	Water(glm::vec3 position, float sideLength)
		: _position(position), _sideLength(sideLength) {}

	Water(float x, float y, float z, float sideLength)
		: _position(glm::vec3(x, y, z)), _sideLength(sideLength) {}

	glm::vec3 getPosition() const {
		return _position;
	}

	void setPosition(glm::vec3 position) {
		_position = position;
	}

	void setScale(float s) {
		_sideLength = s;
	}

	glm::mat4 getModelMatrix() const {
		// glm::mat4 res = glm::translate(glm::mat4(1.0f), _position);
		// res = glm::scale(res, glm::vec3(_scale, 0.0f, _scale));
		// return res;
		return glm::mat4(
				_sideLength/2.0f, 0.0f, 0.0f, 0.0f, 	// the first column
				0.0f, 1.0f, 0.0f, 0.0f,					// the second column
				0.0f, 0.0f, _sideLength/2.0f, 0.0f,		// the third column
				_position.x, _position.y, _position.z, 1.0f
			);
	}

private:

	glm::vec3 _position = glm::vec3(0.0f);

	// float _scale = 1.0f;
	float _sideLength = 10.0f;

}; // class Water

typedef shared_ptr<Water> WaterPtr;



class WaterFrameBuffer
{

public:

	WaterFrameBuffer();

	~WaterFrameBuffer();

	void bindReflectionBuffer() const;
	void bindRefractionBuffer() const;
	void unbindCurrentFrameBuffer() const;

	TexturePtr getReflectionTexture() const;
	TexturePtr getRefractionTexture() const;

	GLuint getReflectionTextureRaw() const;
	GLuint getRefractionTextureRaw() const;

private:
	void bindFrameBuffer(GLuint framebuffer,
							const int width, const int height) const;

private:

	static const int FRAMEBUFFERWIDTH = 500;
	static const int FRAMEBUFFERHEIGHT = 500;

	GLuint _reflectionFBO;
	GLuint _reflectionColor;
	GLuint _reflectionDepthRenderBuffer;

	GLuint _refractionFBO;
	GLuint _refractionColor;
	GLuint _refractionDepthRenderBuffer;

}; // class WaterFrameBuffer



class WaterRenderer
{

public:

	WaterRenderer(WaterShader * shader,
					glm::mat4 projectionMatrix,
					WaterFrameBuffer * waterFrameBuffer,
					Texture * dudv,
					Texture * normal);

	void render(const vector<WaterPtr> & waters,
				const Camera * camera) const;

private:

	WaterShader * _shader;

	WaterFrameBuffer * _waterFrameBuffer;

	GLuint _VAO;
	GLuint _VBO;

	Texture * _dudv;
	Texture * _normal;

private:

	void initGL();

}; // class WaterRenderer

#endif