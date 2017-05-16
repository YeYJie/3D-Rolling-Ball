#ifndef _RAWMODEL_H_
#define _RAWMODEL_H_

#include "include.h"

class RawModel
{

public:

	RawModel() {}

	RawModel(GLuint vao, GLuint vbo, GLuint ebo, bool useEBO, int vertexNum)
		: _VAO(vao), _VBO(vbo), _EBO(ebo), _useEBO(useEBO),
			_vertexNum(vertexNum) {}

	void bindGL()
	{
		glBindVertexArray(_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		
		if(_useEBO) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		}

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
							sizeof(GLfloat) * 8, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
							sizeof(GLfloat) * 8, (void*)(3*sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
							sizeof(GLfloat) * 8, (void*)(5*sizeof(GLfloat)));
	}

	void draw()
	{
		if(_useEBO)
			glDrawElements(GL_TRIANGLES, _vertexNum, GL_UNSIGNED_INT, 0);
		else
			glDrawArrays(GL_TRIANGLES, 0, _vertexNum);
	}

	void unbindGL()
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		if(_useEBO) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
		glBindVertexArray(0);
	}

private:

	GLuint _VAO = 0;
	GLuint _VBO = 0;
	GLuint _EBO = 0;
	bool _useEBO = true;
	int _vertexNum = 0;
};

#endif