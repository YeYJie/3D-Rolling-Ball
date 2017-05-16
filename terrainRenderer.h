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


	void render(const vector<Terrain> & terrains) const 
	{
		_shader->bindGL();
		for(auto & terrain : terrains)
		{
			_shader->setModelMatrix(terrain.getModelMatrix());
			
			terrain.bindGL();
			const vector<Texture> & textures = terrain.getTexture();
			for(int i = 0; i < textures.size(); ++i) {
				textures[i].bindGL(i);
				char name[6];
				snprintf(name, sizeof(name), "text%d", i + 1);
				// cout << "binding texture : " << name << endl;
				_shader->setUniform1i(name, i);
			}
			terrain.draw();
			terrain.unbindGL();
		}
		_shader->unbindGL();
	}

private:

	Shader * _shader;
	// glm::mat4 _projectionMatrix = glm::mat4(0.0f);

};

#endif