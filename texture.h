#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "include.h"

class Texture
{

public:

	Texture() = default;

	Texture(GLuint textureID) : _textureID(textureID) {}

	Texture(const char * textureFileName) {
		_textureID = LoadTexture(textureFileName);
	}

	~Texture() {
		glDeleteTextures(1, &_textureID);
	}

	void bindGL(int textureIndex = 0) const 
	{
		glActiveTexture(GL_TEXTURE0 + textureIndex);
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}

	void unbindGL() const {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:

	GLuint _textureID;

};

typedef shared_ptr<Texture> TexturePtr;

#endif