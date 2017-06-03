#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "include.h"
#include "texture.h"
// #include "vertex.h"

float _defaultHeightFunction(float h);

class Terrain
{

public:

	Terrain(const char * heightMapFileName, float scale,
			std::function<float(float)> heightFunction = _defaultHeightFunction);

	~Terrain();

	void bindGL() const;

	void draw() const;

	void unbindGL() const;

	glm::vec3 getGradient(float x, float z) const;

	float getHeight(float x, float z) const;

	glm::mat4 getModelMatrix() const;

	float getScale() const { return _scale; }

	const vector<TexturePtr> & getTexture() const { return _texture; }

	void correctPosition(float & x, float & z) const;

private:

	// float _defaultHeightFunction(float h);
	void updateModelMatrix();

	float getHeightRaw(const int x, const int z) const;

private:

	GLuint _VAO = 0;
	GLuint _positionVBO = 0;
	GLuint _textCoordsVBO = 0;
	GLuint _normalVBO = 0;
	GLuint _EBO = 0;

	GLuint _indicesNum;


	int _size;
	int _half_size;

	float _scale = 1.0f;

	glm::mat4 _modelMatrix;

	vector<TexturePtr> _texture;

	vector<vector<float>> _heightMap;
	std::function<float(float)> _heightFunction;

private:

	void loadHeightMap(const char * heightMapFileName);
	void loadTexture(const vector<const char*> & textureFiles);
	glm::vec3 calculateNormal(int x, int z);

	void initGL(const vector<float> & position,
						const vector<float> & normal,
						const vector<float> & textCoords,
						const vector<unsigned int> & indices);

};

typedef shared_ptr<Terrain> TerrainPtr;

#endif