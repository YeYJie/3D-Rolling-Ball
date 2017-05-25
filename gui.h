#ifndef _GUI_H_
#define _GUI_H_

#include "include.h"
#include "texture.h"
#include "shader.h"

class GUI
{
public:

	GUI() = default;

	GUI(Texture texture) : _texture(texture) {}

	GUI(Texture texture, float x, float y, float scale)
		: _texture(texture), _x(x), _y(y), _scaleX(scale), _scaleY(scale) {}

	GUI(Texture texture, float x, float y, float scaleX, float scaleY)
		: _texture(texture), _x(x), _y(y), _scaleX(scaleX), _scaleY(scaleY) {}


	void setTexture(Texture texture) {
		_texture = texture;
	}

	void setPositionAndSize(int x, int y, int width, int height);

	glm::mat4 getModelMatrix() const;

	void bindGL() const;

	void unbindGL() const;

private:

	Texture _texture;

	float _x = 0.0f;
	float _y = 0.0f;
	// float _scale = 0.5f;
	float _scaleX = 0.5f;
	float _scaleY = 0.5f;

}; // calss GUI



class GUIRenderer
{

public:

	GUIRenderer() {
		_shader = new Shader("gui.vs", "gui.fs");
		initGL();
	}

	GUIRenderer(const char * vs, const char * fs) {
		_shader = new Shader(vs, fs);
		initGL();
	}

	void render(const vector<GUI> & guis);

	void render(const GUI & gui);

private:

	Shader * _shader;

	GLuint _VAO;

private:

	void initGL();

}; // class GUIRenderer

#endif