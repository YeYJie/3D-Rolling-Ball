#ifndef _SKYBOX_H_
#define _SKYBOX_H_

#include "include.h"
#include "model.h"
#include "skyboxShader.h"


class SkyboxRenderer
{

public:

	SkyboxRenderer(SkyboxShader * shader, glm::mat4 projectionMatrix)
	{
		_shader = shader;
		_shader->bindGL();
		_shader->setProjectionMatrix(projectionMatrix);
		_shader->unbindGL();

		initGL();
	}

	void render() const;

private:

	SkyboxShader * _shader;

	GLuint _cube;

	GLuint _cubeMapDay;
	GLuint _cubeMapNight;

private:

	void initGL();

};

#endif