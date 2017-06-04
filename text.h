#ifndef _TEXTRENDERER_H_
#define _TEXTRENDERER_H_

#include "include.h"
#include "shader.h"
#include "texture.h"
#include "gui.h"

#include <ft2build.h>
#include FT_FREETYPE_H

#define TEXT_MODE_NORMAL 	1
#define TEXT_MODE_OUTLINE 	2
#define TEXT_MODE_GLOW 		3
#define TEXT_MODE_DROPSHADOW 4

class Text
{

public:

	Text(const string & s, float x, float y, float scale = 1.0f,
			int textMode = TEXT_MODE_NORMAL)
	{
		_content = s;
		_x = x;
		_y = y;
		_scaleX = _scaleY = scale;
		_textMode = textMode;
	}

	Text(const string & s, float x, float y,
							float scaleX = 1.0f, float scaleY = 1.0f,
							int textMode = TEXT_MODE_NORMAL)
	{
		_content = s;
		_x = x;
		_y = y;
		_scaleX = scaleX;
		_scaleY = scaleY;
		_textMode = textMode;
	}

	Text(const wstring & s, float x, float y, float scale = 1.0f,
			int textMode = TEXT_MODE_NORMAL)
	{
		_wcontent = s;
		_x = x;
		_y = y;
		_scaleX = _scaleY = scale;
		_textMode = textMode;
	}

	Text(const wstring & s, float x, float y,
							float scaleX = 1.0f, float scaleY = 1.0f,
							int textMode = TEXT_MODE_NORMAL)
	{
		_wcontent = s;
		_x = x;
		_y = y;
		_scaleX = scaleX;
		_scaleY = scaleY;
		_textMode = textMode;
	}

	string getContent() const { return _content; }
	void setContent(const string & s) { _content = s; }

	wstring getContentW() const { return _wcontent; }
	void setContentW(const wstring & s) { _wcontent = s; }

	int getTextMode() const { return _textMode; }
	void setTextMode(int textMode) { _textMode = textMode; }

	glm::vec2 getPosition() const { return glm::vec2(_x, _y); }
	float getPositionX() const { return _x; }
	float getPositionY() const { return _y; }

	void setPosition(float x, float y) {
		_x = x;
		_y = y;
	}
	void setPositionX(float x) { _x = x; }
	void setPositionY(float y) { _y = y; }
	void movePositionX(float x) { _x += x; }
	void movePositionY(float y) { _y += y; }

	glm::vec2 getScale() const { return glm::vec2(_scaleX, _scaleY); }
	glm::vec2 setScale(float scaleX, float scaleY) {
		glm::vec2 old = glm::vec2(_scaleX, _scaleY);
		_scaleX = scaleX;
		_scaleY = scaleY;
		return old;
	}

	float getScaleX() const { return _scaleX; }
	void setScaleX(float scaleX) { _scaleX = scaleX; }

	float getScaleY() const { return _scaleY; }
	void setScaleY(float scaleY) { _scaleY = scaleY; }

	glm::vec3 getTextColor() const {
		return _textColor;
	}
	void setTextColor(const glm::vec3 & color) {
		_textColor = color;
	}
	void setTextColor(float r, float g, float b) {
		_textColor = glm::vec3(r, g ,b);
	}


	glm::vec3 getOutlineColor() const {
		return _outlineColor;
	}
	void setOutlineColor(const glm::vec3 & color) {
		_outlineColor = color;
	}
	void setOutlineColor(float r, float g, float b) {
		_outlineColor = glm::vec3(r, g ,b);
	}


	glm::vec3 getGlowColor() const {
		return _glowColor;
	}
	void setGlowColor(const glm::vec3 & color) {
		_glowColor = color;
	}
	void setGlowColor(float r, float g, float b) {
		_glowColor = glm::vec3(r, g ,b);
	}


	glm::vec3 getDropShadowColor() const {
		return _dropShadowColor;
	}
	void setDropShadowColor(const glm::vec3 & color) {
		_dropShadowColor = color;
	}
	void setDropShadowColor(float r, float g, float b) {
		_dropShadowColor = glm::vec3(r, g ,b);
	}

private:

	string _content = "";
	wstring _wcontent = L"";

	glm::vec3 _textColor = glm::vec3(1.0f, 1.0f, 0.0f); // default yellow
	glm::vec3 _outlineColor = glm::vec3(1.0f, 0.0f, 0.0f); // default red
	glm::vec3 _glowColor = glm::vec3(0.0f, 1.0f, 0.0f); // default green
	glm::vec3 _dropShadowColor = glm::vec3(0.0f); // default black

	int _textMode;

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


class TextShader : public Shader
{

public:

	TextShader(const char * vertexShaderFileName,
					const char * fragmentShaderFileName)
		: Shader(vertexShaderFileName, fragmentShaderFileName)
	{
		bindGL();

		_textTranslateMatrix = getUniformLocation("textTranslateMatrix");
		_guiTexture = getUniformLocation("guiTexture");

		_textMode = getUniformLocation("textMode");
		_textColor = getUniformLocation("textColor");
		_outlineColor = getUniformLocation("outlineColor");
		_glowColor = getUniformLocation("glowColor");
		_dropShadowColor = getUniformLocation("dropShadowColor");

		unbindGL();
	}

	void setTextTranslateMatrix(const glm::mat3 & m) {
		setUniformMatrix3fv(_textTranslateMatrix, m);
	}

	void setGuiTexture(int value) {
		setUniform1i(_guiTexture, value);
	}

	void setTextMode(int value) {
		setUniform1i(_textMode, value);
	}

	void setTextColor(const glm::vec3 & color) {
		setUniform3f(_textColor, color);
	}
	void setTextColor(float r, float g, float b) {
		setUniform3f(_textColor, r, g, b);
	}

	void setOutlineColor(const glm::vec3 & color) {
		setUniform3f(_outlineColor, color);
	}
	void setOutlineColor(float r, float g, float b) {
		setUniform3f(_outlineColor, r, g, b);
	}

	void setGlowColor(const glm::vec3 & color) {
		setUniform3f(_glowColor, color);
	}
	void setGlowColor(float r, float g, float b) {
		setUniform3f(_glowColor, r, g, b);
	}

	void setDropShadowColor(const glm::vec3 & color) {
		setUniform3f(_dropShadowColor, color);
	}
	void setDropShadowColor(float r, float g, float b) {
		setUniform3f(_dropShadowColor, r, g, b);
	}

private:

	GLint _textTranslateMatrix;

	GLint _guiTexture;

	GLint  _textMode;
	GLint _textColor;
	GLint _outlineColor;
	GLint _glowColor;
	GLint _dropShadowColor;

};

class TextRenderer
{

public:


	TextRenderer(const char * ttf, TEXT_TTF);

	TextRenderer(const char * fnt, const char * png, TEXT_SDF);

	void render(const vector<TextPtr> & texts);

private:

	TextShader * _shader;

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