#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{

public:

	Texture(GLuint textureID) : _textureID(textureID) {}

	Texture(const char * textureFileName);

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

#endif