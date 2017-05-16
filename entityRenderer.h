#ifndef _ENTITYRENDERER_H_
#define _ENTITYRENDERER_H_

#include "include.h"
#include "shader.h"

class EntityRenderer
{

public:

	EntityRenderer(Shader * shader, const glm::mat4 projectionMatrix)
		: _shader(shader)
	{
		_shader->bindGL();
		_shader->setProjectionMatrix(projectionMatrix);
		_shader->unbindGL();
	}


	// void render(cost map<TexturedModel, vector<Entity*>> & entities) const
	void render(const vector<Entity*> & entities) const
	{
		_shader->bindGL();
		for(const auto & entity : entities)
		{
			TexturedModel * t = entity->getModel();
			t->bindGL();
			_shader->setModelMatrix(entity->getModelMatrix());
			t->draw();
			t->unbindGL();			
		}
		// for(auto i : entities)
		// {
		// 	auto texturedModel = i.first;
		// 	texturedModel.bindGL();

		// 	auto entityVector = i.second;
		// 	for(auto entity : entityVector)
		// 	{
		// 		_shader->setModelMatrix(entity->getModelMatrix());
		// 		texturedModel.draw();
		// 	}

		// 	texturedModel.unbindGL();
		// }
		_shader->unbindGL();
	}

private:

	Shader * _shader;

	// glm::mat4 _projectionMatrix = glm::mat4(0.0f);

};

#endif