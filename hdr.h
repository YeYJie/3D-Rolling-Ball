#ifndef _HDR_H_
#define _HDR_H_

#include "include.h"
#include "shader.h"
#include "gui.h"


class HDR
{

public:

	HDR();

	~HDR();

	void bindGL() const;

	void unbindGL() const;

	void render(GUIRenderer * guiRenderer) const;

	void setExposure(float exposure);

private:

	GLuint _FBO;
	GLuint _color;
	GLuint _depth;

	Shader * _shader;
	GLuint _exposureLocation; // not using specific shader
};


#endif