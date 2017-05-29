#ifndef _TERRAINRENDERER_H_
#define _TERRAINRENDERER_H_

#include "include.h"
#include "shader.h"

class TerrainRenderer
{

public:

	TerrainRenderer(Shader * shader, const glm::mat4 projectionMatrix)
		: _shader(shader)
	{
		_shader->bindGL();
		_shader->setProjectionMatrix(projectionMatrix);
		_shader->unbindGL();
	}


	void render(const vector<TerrainPtr> & terrains) const 
	{
		_shader->bindGL();
		for(auto & terrain : terrains)
			renderSingleTerrain(terrain);
		_shader->unbindGL();
	}

	void render(const TerrainPtr & terrain) const
	{
		_shader->bindGL();
		renderSingleTerrain(terrain);
		_shader->unbindGL();	
	}

private:

	void renderSingleTerrain(const TerrainPtr & terrain) const
	{
		_shader->setModelMatrix(terrain->getModelMatrix());
		terrain->bindGL();
		const vector<TexturePtr> & textures = terrain->getTexture();
		for(int i = 0; i < textures.size(); ++i) {
			textures[i]->bindGL(i);
			char name[6];
			snprintf(name, sizeof(name), "text%d", i + 1);
			_shader->setUniform1i(name, i);
		}
		terrain->draw();

		for(int i = 0; i < textures.size(); ++i) {
			textures[i]->unbindGL();
		}
		terrain->unbindGL();
	}

private:

	Shader * _shader;

};

#endif