#include "skybox.h"

static vector<float> vertex = {
		-1000.0f, 1000.0f, -1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		1000.0f, 1000.0f, -1000.0f,
		-1000.0f, 1000.0f, -1000.0f,

		-1000.0f, -1000.0f, 1000.0f,
		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, 1000.0f, -1000.0f,
		-1000.0f, 1000.0f, -1000.0f,
		-1000.0f, 1000.0f, 1000.0f,
		-1000.0f, -1000.0f, 1000.0f,

		1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,

		-1000.0f, -1000.0f, 1000.0f,
		-1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, -1000.0f, 1000.0f,
		-1000.0f, -1000.0f, 1000.0f,

		-1000.0f, 1000.0f, -1000.0f,
		1000.0f, 1000.0f, -1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		1000.0f, 1000.0f, 1000.0f,
		-1000.0f, 1000.0f, 1000.0f,
		-1000.0f, 1000.0f, -1000.0f,

		-1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f, 1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		1000.0f, -1000.0f, -1000.0f,
		-1000.0f, -1000.0f, 1000.0f,
		1000.0f, -1000.0f, 1000.0f
};

static vector<const char*> cubeMapFileName = {
	"./skybox/right.png", 
	"./skybox/left.png", 
	"./skybox/top.png", 
	"./skybox/bottom.png", 
	"./skybox/back.png", 
	"./skybox/front.png"
};

void SkyboxRenderer::initGL()
{
	glGenVertexArrays(1, &_cube);
	glBindVertexArray(_cube);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertex.size() * sizeof(float),
						vertex.data(), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);

	_cubeMap = LoadCubeMap(cubeMapFileName);
}

void SkyboxRenderer::render() const
{
    glDepthFunc(GL_LEQUAL);

	glBindVertexArray(_cube);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);

	// _shader->setUniform1i("cubeMap", 0);

	static float rotate = 0.0f;
	static glm::vec3 __yaxis = glm::vec3(0.0, 1.0, 0.0);
	glm::mat4 modelMatrix = glm::rotate(glm::mat4(1.0f), rotate, __yaxis);
	_shader->setModelMatrix(modelMatrix);
	rotate += 0.00005f;

    glDrawArrays(GL_TRIANGLES, 0, 36);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}