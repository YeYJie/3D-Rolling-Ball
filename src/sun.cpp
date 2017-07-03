#include "sun.h"

SunRenderer::SunRenderer(SunShader * shader, const glm::mat4 & projectionMatrix)
{
	_shader = shader;
	_shader->bindGL();
	_shader->setProjectionMatrix(projectionMatrix);
	_shader->unbindGL();
	initGL();
	initFlare();
}

void SunRenderer::initFlare()
{
	_flares = new FlareTexture[14] {
		FlareTexture(new Texture("sun6.png"), 0.5, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun4.png"), 0.23, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun2.png"), 0.1, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun7.png"), 0.05, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun1.png"), 0.02, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun3.png"), 0.06, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun9.png"), 0.12, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun5.png"), 0.07, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun1.png"), 0.12, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun9.png"), 0.1, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun3.png"), 0.07, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun5.png"), 0.3, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun4.png"), 0.4, glm::vec2(0.0f)),
		FlareTexture(new Texture("sun8.png"), 0.6, glm::vec2(0.0f))
	};
}

void SunRenderer::render(const SunPtr & sun, const Camera * camera) {
	_shader->bindGL();

	glBindVertexArray(_VAO);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glDisable(GL_DEPTH_TEST);

	// sun
	sun->bindGL();

	glm::mat4 modelViewMatrix = calculateModelViewMatrix(sun, camera);

	_shader->setRenderSun(1);
	_shader->setModelViewMatrix(modelViewMatrix);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	sun->unbindGL();

	_shader->setRenderSun(0);
	renderFlare(sun, camera);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	_shader->unbindGL();
}

void SunRenderer::renderFlare(const SunPtr & sun, const Camera * camera)
{
	glm::vec2 sunScreenCoords = getScreenCoords(sun->getPosition(camera), camera);
	float sunDistanceToCenter = sqrt((0.5f - sunScreenCoords.x) * (0.5f - sunScreenCoords.x)
									+ (0.5f - sunScreenCoords.y) * (0.5f - sunScreenCoords.y));
	float bright = 1.0 - (sunDistanceToCenter / 1.0f);

	// cout << "sun screen coords : " << sunScreenCoords.x <<  " "
	// 		<< sunScreenCoords.y << " " << bright << endl;

	if(bright < 0)
		return;

	_shader->setBright(bright);

	glm::vec2 direction = glm::vec2(0.5f, 0.5f) - sunScreenCoords;

	static float spacing = 0.14f;

	for(int i = 0; i < 14; ++i) {
		_flares[i].screenCoords = sunScreenCoords + direction * (float)i * spacing;

		_flares[i].texture->bindGL();

		glm::mat4 flareMatrix(0.0f);
		flareMatrix[0][0] = flareMatrix[1][1] = flareMatrix[2][2] = _flares[i].scale;
		flareMatrix[3][3] = 1.0f;
		flareMatrix[3][0] = _flares[i].screenCoords.x * 2.0 - 1.0;
		flareMatrix[3][1] = 1.0 - 2.0 * _flares[i].screenCoords.y;
		// _shader->setUniformMatrix4fv("flareMatrix", flareMatrix);
		_shader->setFlareMatrix(flareMatrix);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		_flares[i].texture->unbindGL();
	}

}

glm::vec2 SunRenderer::getScreenCoords(const glm::vec3 & position,
										const Camera * camera)
{
	return getScreenCoords(position, camera->getViewMatrix());
}

glm::vec2 SunRenderer::getScreenCoords(const glm::vec3 & position,
										const glm::mat4 & viewMatrix)
{
	glm::mat4 projectionMatrix = glm::perspective(45.0f, 16.0f/9.0f, 0.1f, 10000.0f);

	glm::vec4 temp = glm::vec4(position, 1.0f);
	temp = projectionMatrix * viewMatrix * temp;
	if(temp.w <= 0)
		return glm::vec2(-1.0f);
	else
		return glm::vec2(
							(temp.x / temp.w + 1) / 2.0f,
							1.0f - ((temp.y / temp.w + 1) / 2.0f)
						);
}

glm::mat4 SunRenderer::calculateModelViewMatrix(const SunPtr & sun, const Camera * camera)
{
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 res = glm::mat4(0.0f);
	// res[0][0] = res[1][1] = res[2][2] = 1.0f;
	res[0] = glm::vec4(viewMatrix[0][0], viewMatrix[1][0], viewMatrix[2][0], 0.0f); // first column
	res[1] = glm::vec4(viewMatrix[0][1], viewMatrix[1][1], viewMatrix[2][1], 0.0f); // second column
	res[2] = glm::vec4(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2], 0.0f); // third column
	res[3] = glm::vec4(sun->getPosition(camera), 1.0f);

	res = viewMatrix * res;
	float scale = 3.0f;
	res[0][0] = res[1][1] = res[2][2] = scale;
	return res;
}


void SunRenderer::initGL()
{
	static vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, _VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
						vertices.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindVertexArray(0);
}

SunRenderer::~SunRenderer()
{
	glDeleteVertexArrays(1, &_VAO);
	glDeleteBuffers(1, &_VBO);
}