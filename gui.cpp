#include "gui.h"

extern const int WIDTH;
extern const int HEIGHT;

GUI & GUI::setTopLeftConnerPos(int x, int y)
{
	_x = (float)x / (float)WIDTH * 2.0f + _scaleX - 1.0f;
	_y = 1.0f - (float)y / (float)HEIGHT * 2.0f - _scaleY;
	return *this;
}

GUI & GUI::setCenterPos(int x, int y)
{
	_x = 2.0 * (float)x / (float)WIDTH - 1.0f;
	_y = 1.0f - (float)y / (float)HEIGHT;
	return *this;
}

GUI & GUI::setSize(int width, int height)
{
	_scaleX = (float)width / (float)WIDTH;
	_scaleY = (float)height / (float)HEIGHT;
	return *this;
}

void GUI::setPositionAndSize(int x, int y, int width, int height)
{
	_scaleX = (float)width / (float)WIDTH;
	_scaleY = (float)height / (float)HEIGHT;
	_x = (float)x / (float)WIDTH * 2.0f + _scaleX - 1.0f;
	_y = 1.0f - (float)y / (float)HEIGHT * 2.0f - _scaleY;
}


glm::mat4 GUI::getModelMatrix() const 
{
	glm::mat4 res = glm::translate(glm::mat4(1.0f),
								glm::vec3(_x, _y, 0.0f));
	res = glm::scale(res, glm::vec3(_scaleX, _scaleY, 1.0f));
	return res;
}

void GUI::bindGL() const 
{
	_texture.bindGL();
}

void GUI::unbindGL() const 
{
	_texture.unbindGL();
}

void GUIRenderer::initGL()
{
	static vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
									vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void GUIRenderer::render(const vector<GUI> & guis) 
{
	_shader->bindGL();

	glBindVertexArray(_VAO);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	for(auto i : guis) {
		i.bindGL();
		_shader->setModelMatrix(i.getModelMatrix());
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		i.unbindGL();
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	_shader->unbindGL();
}

void GUIRenderer::render(const GUI & gui)
{
	_shader->bindGL();

	glBindVertexArray(_VAO);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	gui.bindGL();
	_shader->setModelMatrix(gui.getModelMatrix());
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	gui.unbindGL();

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	_shader->unbindGL();	
}