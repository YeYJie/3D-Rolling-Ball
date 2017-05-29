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

	Text(const wstring & s, float x, float y, float scale, 
			glm::vec3 color = glm::vec3(0.0f))
	{
		_wcontent = s;
		_x = x;
		_y = y;
		_scaleX = _scaleY = scale;
		_color = color;
	}

	Text(const wstring & s, float x, float y,
							float scaleX, float scaleY,
							glm::vec3 color = glm::vec3(0.0f))
	{
		_wcontent = s;
		_x = x;
		_y = y;
		_scaleX = scaleX;
		_scaleY = scaleY;
		_color = color;
	}

	string getContent() const { return _content; }

	wstring getContentW() const { return _wcontent; }

	glm::vec2 getPosition() const { return glm::vec2(_x, _y); }

	glm::vec2 setScale(float scaleX, float scaleY) {
		glm::vec2 old = glm::vec2(_scaleX, _scaleY);
		_scaleX = scaleX;
		_scaleY = scaleY;
		return old;
	}
	glm::vec2 getScale() const { return glm::vec2(_scaleX, _scaleY); }

	glm::vec3 getColor() const { return _color; }

	void setColor(glm::vec3 color) { _color = color; }

	void setColor(float r, float g, float b) {
		_color = glm::vec3(r, g ,b);
	}

private:

	string _content = "";
	wstring _wcontent = L"";

	glm::vec3 _color = glm::vec3(0.0f);

	float _x = 0.0f;
	float _y = 0.0f;

	float _scaleX = 0.1f;
	float _scaleY = 0.1f;

}; // class Text

typedef shared_ptr<Text> TextPtr;


struct SDFChar {
	int id;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;

	SDFChar(int iid = 0, int xx = 0, int yy = 0, int wwidth = 0,
			int hheight = 0, int xxoffset = 0, int yyoffset = 0,
			int xxadvance = 0)
		: id(iid), x(xx), y(yy), width(wwidth), height(hheight),
			xoffset(xxoffset), yoffset(yyoffset), xadvance(xxadvance) {}
};

struct TTFChar {
    TexturePtr texture;
    glm::ivec2 size;
    glm::ivec2 bearing;
    GLuint advance;
};


struct TEXT_TTF {};
struct TEXT_SDF {};


class TextRenderer
{

public:


	TextRenderer(const char * ttf, TEXT_TTF);

	TextRenderer(const char * fnt, const char * png, TEXT_SDF);

	void render(const vector<TextPtr> & texts);

private:

	Shader * _shader;

	GLuint _VAO;
	GLuint _positionVBO;


	bool _useSDF = false;

	map<char, TTFChar> _TTFcharMap;

	TexturePtr _textAtlas;
	map<int, SDFChar> _SDFcharMap;

private:

	void initGL();

	void renderTTF(const vector<TextPtr> & texts);
	void renderSDF(const vector<TextPtr> & texts);


}; // class TextRenderer

#endif