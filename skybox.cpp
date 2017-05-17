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
	// "./skybox/right.png", 
	// "./skybox/left.png", 
	// "./skybox/top.png", 
	// "./skybox/bottom.png", 
	// "./skybox/back.png", 
	// "./skybox/front.png"
	"./skybox/cloud9_ft.tga",
    "./skybox/cloud9_bk.tga",
    "./skybox/cloud9_dn.tga",
    "./skybox/cloud9_up.tga",
    "./skybox/cloud9_rt.tga",
    "./skybox/cloud9_lf.tga",
};

#include "tgaLoader.h"
static GLuint loadCubemap(const vector<const char *> & faces)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for(int i = 0; i < 6; ++i) {
        TgaFile tga;
        loadTgaFile(faces[i], &tga);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
                    tga.colorMode == 3 ? GL_RGB : GL_RGBA,
                    tga.imageWidth, tga.imageHeight,
                    0,
                    tga.colorMode == 3 ? GL_RGB : GL_RGBA,
                    GL_UNSIGNED_BYTE, tga.imageData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

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

	_cubeMap = loadCubemap(cubeMapFileName);
	// _cubeMap = LoadCubeMap(cubeMapFileName);
}

void SkyboxRenderer::render() const
{
    glDepthFunc(GL_LEQUAL);

	glBindVertexArray(_cube);
	glEnableVertexAttribArray(0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeMap);

	_shader->setUniform1i("cubeMap", 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);

    // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	glBindVertexArray(0);

    glDepthFunc(GL_LESS);
}