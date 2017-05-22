#ifndef _SHADER_H_
#define _SHADER_H_

#include "include.h"

class Shader
{

public:

	Shader(const char * vertexShaderFileName,
			const char * fragmentShaderFileName) 
	{
		_programID = LoadShaders(vertexShaderFileName, fragmentShaderFileName);
		
		bindGL();
		_modelMatrixLocation = glGetUniformLocation(_programID, 
																"model");
		_viewMatrixLocation = glGetUniformLocation(_programID, 
																"view");
		_projectionMatrixLocation = glGetUniformLocation(_programID, 
																"projection");
		unbindGL();
	}

	void bindGL() const {
		glUseProgram(_programID);
	}

	void unbindGL() const {
		glUseProgram(0);
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

	void setUniform1i(const char * name, int value) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniform1i(location, value);
	}

	void setUniform1f(const char * name, float value) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniform1f(location, value);		
	}

	void setUniform3f(const char * name, float x, float y, float z) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniform3f(location, x, y, z);
	}

	void setUniform3f(const char * name, const glm::vec3 v) {
		setUniform3f(name, v.x, v.y, v.z);
	}

	void setUniform4f(const char * name, float x, float y, float z, float w) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniform4f(location, x, y, z, w);
	}

	void setUniform4f(const char * name, glm::vec4 v) {
		setUniform4f(name, v.x, v.y, v.z, v.w);
	}

	void setUniformMatrix4fv(const char * name, const glm::mat4 & mat) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	}

	// void setUniformMatrix4fv(const char * name, const glm::mat4 mat) {
	// 	GLint location = glGetUniformLocation(_programID, name);
	// 	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
	// }

	void setUniformMatrix3fv(const char * name, const glm::mat3 & mat) {
		GLint location = glGetUniformLocation(_programID, name);
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));		
	}

private:

	GLuint _programID = 0;

	GLint _modelMatrixLocation = 0;
	GLint _viewMatrixLocation = 0;
	GLint _projectionMatrixLocation = 0;

};

#endif