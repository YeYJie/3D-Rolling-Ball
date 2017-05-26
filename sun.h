#ifndef _SUN_H_
#define _SUN_H_

#include "include.h"
#include "texture.h"
#include "camera.h"


class Sun
{

public:

	Sun() = delete;
	Sun(const Sun &) = delete;
	Sun & operator=(const Sun &) = delete;

	Sun(const Texture & texture, const glm::vec3 & lightDirection, float scale) {
		_texture = texture;
		_lightDirection = glm::normalize(lightDirection);
		_scale = scale;
	}

	Sun(const char * textureFileName, const glm::vec3 & lightDirection,
			float scale)
	{
		_texture = Texture(textureFileName);
		_lightDirection = glm::normalize(lightDirection);
		_scale = scale;
	}

	Texture getTexture() const { 
		return _texture; 
	}

	void setTexture(const Texture & texture) {
		_texture = texture;
	}

	glm::vec3 getLightDirection() const { 
		return _lightDirection; 
	}

	void setLightDirection(const glm::vec3 & lightDirection) {
		_lightDirection = glm::normalize(lightDirection);
	}

	void setScale(float scale) {
		_scale = scale;
	}

	void setPosition(glm::vec3 position) {
		_position = position;
	}

	glm::vec3 getPosition() const
	{
		return _position;
	}

	glm::vec3 getPosition(const Camera * camera) const
	{
		static const float SUN_DISTANCE = 5.0f;
		glm::vec3 position = camera->getPosition() - SUN_DISTANCE * _lightDirection;
		// cout << position.x << " " << position.y << " " << position.z << endl;
		return position;
		// return camera->getPosition() - SUN_DISTANCE * _lightDirection;
	}


	void bindGL() const {
		_texture.bindGL();
	}

	void unbindGL() const {
		_texture.unbindGL();
	}

private:

	glm::vec3 _position;

	Texture _texture;

	glm::vec3 _lightDirection;

	float _scale;

}; // class Sun


class SunRenderer
{

public:

	SunRenderer(Shader * shader, const glm::mat4 & projectionMatrix) {
		_shader = shader;
		_shader->bindGL();
		_shader->setProjectionMatrix(projectionMatrix);
		_shader->unbindGL();
		initGL();
	}

	void render(const Sun & sun, const Camera * camera) {
		_shader->bindGL();

		glBindVertexArray(_VAO);
		glEnableVertexAttribArray(0);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		// draw
		sun.bindGL();

		glm::mat4 modelViewMatrix = calculateModelViewMatrix(sun, camera);

		_shader->setUniformMatrix4fv("modelViewMatrix", modelViewMatrix);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		sun.unbindGL();

		glEnable(GL_DEPTH_TEST);
		glDisable(GL_BLEND);

		glDisableVertexAttribArray(0);
		glBindVertexArray(0);

		_shader->unbindGL();
	}

private:

	glm::mat4 calculateModelViewMatrix(const Sun & sun, const Camera * camera)
	{
		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::mat4 res = glm::mat4(0.0f);
		// res[0][0] = res[1][1] = res[2][2] = 1.0f;
		res[0] = glm::vec4(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], 0.0f); // first column
		res[1] = glm::vec4(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], 0.0f); // second column
		res[2] = glm::vec4(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], 0.0f); // third column
		res[3] = glm::vec4(sun.getPosition(camera), 1.0f);
		// glm::mat4 res = glm::translate(glm::mat4(1.0f), sun.getPosition());
		// res = glm::scale(res, glm::vec3(100.0f, 100.0f, 1.0f));
		res = viewMatrix * res;
		float scale = 2.0f;
		res[0][0] = res[1][1] = res[2][2] = scale;
		return res;
	}

	void initGL()
	{
		static vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};

		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
							vertices.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glBindVertexArray(0);
	}

private:

	Shader * _shader;

	GLuint _VAO;

}; // class SunRenderer


#endif