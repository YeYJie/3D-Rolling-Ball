#ifndef _TEXTUREDMODEL_H_
#define _TEXTUREDMODEL_H_

#include "include.h"
#include "rawModel.h"
#include "texture.h"

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

};

#endif