#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "include.h"
#include "texture.h"
// #include "vertex.h"

class Terrain
{

public:

	Terrain(const char * heightMapFileName);

	void bindGL() const;

	void draw() const;

	void unbindGL() const;

	// TODO : use float parameter
	// glm::vec3 getGradient(const int x, const int z) const;
	float getHeight(const int x, const int z) const;

	glm::vec3 getGradient(const float x, const float z) const;
	float getHeight(const float x, const float z) const;


	glm::mat4 getModelMatrix() const { return glm::mat4(1.0f); }

	const vector<Texture> & getTexture() const { return _texture; }

private:

	GLuint _VAO = 0;
	GLuint _positionVBO = 0;
	GLuint _textCoordsVBO = 0;
	GLuint _normalVBO = 0;
	GLuint _EBO = 0;
	GLuint _indicesNum;


	int _size = 100;

	glm::vec3 _position = glm::vec3(0.0f);

	vector<Texture> _texture;

	vector<vector<float>> _heightMap;

private:

	void loadHeightMap(const char * heightMapFileName);
	void loadTexture(const vector<const char*> & textureFiles);
	glm::vec3 calculateNormal(int x, int z);

	void initGL(const vector<float> & position,
						const vector<float> & normal,
						const vector<float> & textCoords,
						const vector<unsigned int> & indices);

};

#endif