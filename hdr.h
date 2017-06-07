#ifndef _HDR_H_
#define _HDR_H_

#include "include.h"
#include "shader.h"
#include "gui.h"
#include "blur.h"

class HDR
{

public:

	HDR();

	~HDR();

	void bindGL() const;

	void unbindGL() const;

	void render(GUIRenderer * guiRenderer);

	void setExposure(float exposure);

private:

	GLuint _FBO;
	GLuint _color[2];
	GLuint _depth;

	Shader * _shader;

	GLint _exposureLocation;	// i dont want to use a specific shader
	GLint _originLocation;
	GLint _bloomLocation;

	GaussBlur _blurrer;
};


#endif