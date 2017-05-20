#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include "include.h"
#include "shader.h"
#include "texture.h"
#include "gui.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class Text
{

public:

	Text(const string & s, float x, float y, float scale, 
			glm::vec3 color = glm::vec3(0.0f))
	{
		_content = s;
		_x = x;
		_y = y;
		_scaleX = _scaleY = scale;
		_color = color;
	}

	Text(const string & s, float x, float y,
							float scaleX, float scaleY,
							glm::vec3 color = glm::vec3(0.0f))
	{
		_content = s;
		_x = x;
		_y = y;
		_scaleX = scaleX;
		_scaleY = scaleY;
		_color = color;
	}

	string getContent() const { return _content; }

	glm::vec2 getPosition() const { return glm::vec2(_x, _y); }

	glm::vec2 getScale() const { return glm::vec2(_scaleX, _scaleY); }

	glm::vec3 getColor() const { return _color; }

	void setColor(glm::vec3 color) { _color = color; }

	void setColor(float r, float g, float b) {
		_color = glm::vec3(r, g ,b);
	}

private:

	string _content = "";

	glm::vec3 _color = glm::vec3(0.0f);

	float _x = 0.0f;
	float _y = 0.0f;

	float _scaleX = 0.1f;
	float _scaleY = 0.1f;

}; // class Text




class TextRenderer
{

public:

    struct Char {
        Texture texture;
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
    };

	TextRenderer(const char * ttf);

	void render(const vector<Text*> texts);

private:

	Shader * _shader;

	GLuint _VAO;

	map<char, Char> _charMap;

private:

	void initGL();

}; // class TextRenderer

#endif