#ifndef _ENTITYRENDERER_H_
#define _ENTITYRENDERER_H_

#include "include.h"
#include "shader.h"

class EntityRenderer
{

public:

	EntityRenderer(Shader shader, glm::mat4 projectionMatrix)
		: _shader(shader), _projectionMatrix(projectionMatrix) {}

	Shader getShader() const { return _shader; }
	void setShader(Shader shader) { _shader = shader; }

	glm::mat4 getProjectionMatrix() const { return _projectionMatrix; }
	void setProjectionMatrix(glm::mat4 projectionMatrix) {
		_projectionMatrix = projectionMatrix;
	}

	void render(map<TexturedModel, vector<Entity>> entities) const
	{
		_shader.bindGL();
		for(auto i : entities)
		{
			auto texturedModel = i.first;
			texturedModel.bindGL();

			auto entityVector = i.second;
			for(auto entity : entityVector)
			{
				_shader.setModelMatrix(entity.getModelMatrix());
				texturedModel.draw();
			}

			texturedModel.unbindGL();
		}
		_shader.unbindGL();
	}

private:

	Shader _shader;

	glm::mat4 _projectionMatrix = glm::mat4(0.0f);

};

#endif