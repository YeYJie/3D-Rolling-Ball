#ifndef _TEXTURE_H_
#define _TEXTURE_H_

class Texture
{

public:

	Texture(GLuint textureID) : _textureID(textureID) {}

	void bindGL() {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, _textureID);
	}

	void unbindGL() {
		glBindTexture(GL_TEXTURE_2D, 0);
	}

private:

	GLuint _textureID;

};

#endif