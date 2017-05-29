#ifndef _RAWMODEL_H_
#define _RAWMODEL_H_

#include "include.h"
#include "texture.h"

class RawModel
{

public:

	RawModel() {}

	RawModel(const vector<float> & vertex, const vector<float> & texture,
			const vector<float> & normal, const vector<unsigned int> & indices)
	{
		glGenVertexArrays(1, &_VAO);
		glBindVertexArray(_VAO);

		glGenBuffers(1, &_positionVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
		glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float),
						vertex.data(), GL_STATIC_DRAW);
	    glEnableVertexAttribArray(0);
	    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &_textCoordsVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _textCoordsVBO);
		glBufferData(GL_ARRAY_BUFFER, texture.size() * sizeof(float),
						texture.data(), GL_STATIC_DRAW);
	    glEnableVertexAttribArray(1);
	    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &_normalVBO);
		glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
		glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float),
						normal.data(), GL_STATIC_DRAW);
	    glEnableVertexAttribArray(2);
	    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		glGenBuffers(1, &_EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
						indices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);

		_vertexNum = indices.size();
	}

	~RawModel()
	{
		glDeleteBuffers(1, &_EBO);
		glDeleteBuffers(1, &_normalVBO);
		glDeleteBuffers(1, &_textCoordsVBO);
		glDeleteBuffers(1, &_positionVBO);
		glDeleteVertexArrays(1, &_VAO);
	}

	void bindGL()
	{
		glBindVertexArray(_VAO);
	}

	void draw()
	{
		glDrawElements(GL_TRIANGLES, _vertexNum, GL_UNSIGNED_INT, 0);
	}

	void unbindGL()
	{
		glBindVertexArray(0);
	}

private:

	GLuint _VAO = 0;
	GLuint _positionVBO = 0;
	GLuint _textCoordsVBO = 0;
	GLuint _normalVBO = 0;
	GLuint _EBO = 0;

	int _vertexNum = 0;

}; // class RawModel

typedef shared_ptr<RawModel> RawModelPtr;



class TexturedModel
{

public:

	TexturedModel() = delete;

	TexturedModel(RawModelPtr rawModel, TexturePtr texture)
		: _rawModel(rawModel), _texture(texture) {}

	RawModelPtr getRawModel() const { return _rawModel; }
	void setrawModel(RawModelPtr rawModel) { _rawModel = rawModel; }

	TexturePtr getTexture() const { return _texture; }
	void setTexture(TexturePtr texture) { _texture = texture; } 

	void bindGL()
	{
		_rawModel->bindGL();
		_texture->bindGL();
	}

	void draw()
	{
		_rawModel->draw();
	}

	void unbindGL()
	{
		_rawModel->unbindGL();
		_texture->unbindGL();
	}

private:

	RawModelPtr _rawModel;
	TexturePtr _texture;

}; // class TexturedModel

typedef shared_ptr<TexturedModel> TexturedModelPtr;

#endif