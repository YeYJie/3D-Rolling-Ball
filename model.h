#ifndef _RAWMODEL_H_
#define _RAWMODEL_H_

#include "include.h"
#include "texture.h"

class RawModel
{

public:

	RawModel() {}

	// RawModel(GLuint vao, GLuint vbo, GLuint ebo, bool useEBO, int vertexNum)
	// 	: _VAO(vao), _VBO(vbo), _EBO(ebo), _useEBO(useEBO),
	// 		_vertexNum(vertexNum) {}

	RawModel(const vector<float> & vertex, const vector<float> & texture,
			const vector<float> & normal, const vector<unsigned int> & indices)
	{
		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		GLuint positionVBO;
		glGenBuffers(1, &positionVBO);
		glBindBuffer(GL_ARRAY_BUFFER, positionVBO);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float),
						vertex.data(), GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	    GLuint textCoordsVBO;
		glGenBuffers(1, &textCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, textCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(float),
						texture.data(), GL_STATIC_DRAW);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	    GLuint normalVBO;
		glGenBuffers(1, &normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
		glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float),
						normal.data(), GL_STATIC_DRAW);
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	    GLuint EBO;
		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
						indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		_vertexNum = indices.size();
	}

	void bindGL()
	{
		glBindVertexArray(_VAO);
		// glBindBuffer(GL_ARRAY_BUFFER, _VBO);
		
		// if(_useEBO) {
		// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		// }

		// glEnableVertexAttribArray(0);
		// glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		// 					sizeof(GLfloat) * 8, (void*)0);
		// glEnableVertexAttribArray(1);
		// glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE,
		// 					sizeof(GLfloat) * 8, (void*)(3*sizeof(GLfloat)));
		// glEnableVertexAttribArray(2);
		// glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE,
		// 					sizeof(GLfloat) * 8, (void*)(5*sizeof(GLfloat)));
	}

	void draw()
	{
		// if(_useEBO)
			glDrawElements(GL_TRIANGLES, _vertexNum, GL_UNSIGNED_INT, 0);
		// else
			// glDrawArrays(GL_TRIANGLES, 0, _vertexNum);
	}

	void unbindGL()
	{
		// glDisableVertexAttribArray(0);
		// glDisableVertexAttribArray(1);
		// glDisableVertexAttribArray(2);
		// glBindBuffer(GL_ARRAY_BUFFER, 0);
		// if(_useEBO) {
		// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		// }
		glBindVertexArray(0);
	}

private:

	GLuint _VAO = 0;
	// GLuint _VBO = 0;
	// GLuint _EBO = 0;
	// bool _useEBO = true;
	int _vertexNum = 0;
}; // class RawModel


class TexturedModel
{

public:

	TexturedModel() = delete;

	TexturedModel(RawModel rawModel, Texture texture)
		: _rawModel(rawModel), _texture(texture) {}

	RawModel getRawModel() const { return _rawModel; }
	void setrawModel(RawModel rawModel) { _rawModel = rawModel; }

	Texture getTexture() const { return _texture; }
	void setTexture(Texture texture) { _texture = texture; } 

	void bindGL()
	{
		_rawModel.bindGL();
		_texture.bindGL();
	}

	void draw()
	{
		_rawModel.draw();
	}

	void unbindGL()
	{
		_rawModel.unbindGL();
		_texture.unbindGL();
	}

private:

	RawModel _rawModel;
	Texture _texture;

}; // class TexturedModel

#endif