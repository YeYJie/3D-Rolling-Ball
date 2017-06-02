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

		unbindGL();
	}

	void bindGL() const {
		glUseProgram(_programID);
	}

	void unbindGL() const {
		glUseProgram(0);
	}

	GLuint getUniformLocation(const char * name) const {
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

private:

	GLuint _programID = 0;

	GLint _modelMatrixLocation = 0;
	GLint _viewMatrixLocation = 0;
	GLint _projectionMatrixLocation = 0;

};

#endif