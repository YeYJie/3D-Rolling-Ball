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

	void render(const EntityPtr & entity) const
	{
		_shader->bindGL();
		TexturedModelPtr t = entity->getModel();
		t->bindGL();
		_shader->setModelMatrix(entity->getModelMatrix());
		t->draw();
		t->unbindGL();
		_shader->unbindGL();
	}

	void render(const vector<EntityPtr> & entities) const
	{
		_shader->bindGL();
		for(auto entity : entities)
		{
			TexturedModelPtr t = entity->getModel();
			t->bindGL();
			_shader->setModelMatrix(entity->getModelMatrix());
			t->draw();
			t->unbindGL();			
		}
		_shader->unbindGL();
	}

private:

	Shader * _shader;

	// glm::mat4 _projectionMatrix = glm::mat4(0.0f);

};

#endif