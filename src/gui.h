#ifndef _GUI_H_
#define _GUI_H_

#include "include.h"
#include "texture.h"
#include "shader.h"

class GUI
{
public:

	GUI() = default;

	GUI(TexturePtr texture) : _texture(texture) {}

	GUI(TexturePtr texture, float x, float y, float scale)
		: _texture(texture), _x(x), _y(y), _scaleX(scale), _scaleY(scale) {}

	GUI(TexturePtr texture, float x, float y, float scaleX, float scaleY)
		: _texture(texture), _x(x), _y(y), _scaleX(scaleX), _scaleY(scaleY) {}

	void setTexture(TexturePtr texture) {
		_texture = texture;
	}

	void setTexture(GLuint texture) {
		_texture = TexturePtr(new Texture(texture));
	}

	void setPositionAndSize(int x, int y, int width, int height);

 	// you should always set size before setting top-left conner position
	GUI & setTopLeftConnerPos(int x, int y);

	GUI & setCenterPos(int x, int y);

	GUI & setSize(int width, int height);

	glm::mat4 getModelMatrix() const;

	void bindGL() const;

	void unbindGL() const;

private:

	TexturePtr _texture;

	float _x = 0.0f;
	float _y = 0.0f;
	// float _scale = 0.5f;
	float _scaleX = 0.5f;
	float _scaleY = 0.5f;

}; // calss GUI

typedef shared_ptr<GUI> GUIPtr;



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

	~GUIRenderer();

	void render(const vector<GUIPtr> & guis);

	void render(const GUIPtr & gui);

	Shader * getShader() const { return _shader; }

	void setShader(Shader * shader) { _shader = shader; }

private:

	Shader * _shader;

	GLuint _VAO;
	GLuint _VBO;

private:

	void initGL();

}; // class GUIRenderer

#endif