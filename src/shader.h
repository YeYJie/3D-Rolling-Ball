#ifndef _SHADER_H_
#define _SHADER_H_

#include "include.h"

class Shader
{

public:

	Shader(const char * vertexShaderFileName,
			const char * fragmentShaderFileName)
	{
		string vs(vertexShaderFileName);
		vs = "./shader/" + vs;

		string fs(fragmentShaderFileName);
		fs = "./shader/" + fs;

		_programID = LoadShaders(vs.c_str(), fs.c_str());

		bindGL();

		_modelMatrixLocation = getUniformLocation("model");
		_viewMatrixLocation = getUniformLocation("view");
		_projectionMatrixLocation = getUniformLocation("projection");

		_lightSpaceMatrixLocation = getUniformLocation("lightSpaceMatrix");

		unbindGL();
	}

	void bindGL() const {
		glUseProgram(_programID);
	}

	void unbindGL() const {
		glUseProgram(0);
	}

	GLint getUniformLocation(const char * name) const {
		return glGetUniformLocation(_programID, name);
	}

	void setModelMatrix(const glm::mat4 & modelMatrix) const {
		glUniformMatrix4fv(_modelMatrixLocation, 1, GL_FALSE,
								glm::value_ptr(modelMatrix));
	}

	void setViewMatrix(const glm::mat4 & viewMatrix) const {
		glUniformMatrix4fv(_viewMatrixLocation, 1, GL_FALSE,
								glm::value_ptr(viewMatrix));
	}

	void setProjectionMatrix(const glm::mat4 & projectionMatrix) const {
		glUniformMatrix4fv(_projectionMatrixLocation, 1, GL_FALSE,
								glm::value_ptr(projectionMatrix));
	}

	void setLightSpaceMatrix(const glm::mat4 & lightSpaceMatrix) const {
		glUniformMatrix4fv(_lightSpaceMatrixLocation, 1, GL_FALSE,
								glm::value_ptr(lightSpaceMatrix));
	}

	void setUniform1i(const char * name, int value) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniform1i(location, value);
	}

	void setUniform1i(GLuint location, int value) {
		glUniform1i(location, value);
	}

	void setUniform1f(GLuint location, float value) {
		glUniform1f(location, value);
	}

	void setUniform2f(GLuint location, float x, float y) {
		glUniform2f(location, x, y);
	}

	void setUniform2f(GLuint location, const glm::vec2 v) {
		setUniform2f(location, v.x, v.y);
	}

	void setUniform3f(GLuint location, float x, float y, float z) {
		glUniform3f(location, x, y, z);
	}

	void setUniform3f(GLuint location, const glm::vec3 v) {
		setUniform3f(location, v.x, v.y, v.z);
	}

	void setUniform4f(GLuint location, float x, float y, float z, float w) {
		glUniform4f(location, x, y, z, w);
	}

	void setUniform4f(GLuint location, const glm::vec4 & v) {
		setUniform4f(location, v.x, v.y, v.z, v.w);
	}

	void setUniformMatrix4fv(GLuint location, const glm::mat4 & mat) {
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void setUniformMatrix3fv(GLuint location, const glm::mat3 & mat) {
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	void bindTexture(GLuint textureUnit = 0, GLuint texture = 0) const {
		// cout << "Shader::bindTexture : " << textureUnit << " " << texture << endl;
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, texture);
	}

	GLuint getProgramID() const { return _programID; }

private:

	GLuint _programID = 0;

	GLint _modelMatrixLocation = 0;
	GLint _viewMatrixLocation = 0;
	GLint _projectionMatrixLocation = 0;

	GLint _lightSpaceMatrixLocation;

};

#endif