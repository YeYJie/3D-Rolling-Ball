#include "include.h"
#include "ball.h"
#include "entity.h"
#include "entityRenderer.h"
#include "shader.h"
#include "terrain.h"
#include "terrainRenderer.h"
#include "texture.h"

extern const int WIDTH = 1000;
extern const int HEIGHT = 1000;

void onKeyBoard(GLFWwindow * window, int key,
				int scancode, int action, int mods)
{
	if(key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE)
		exit(0);
	// global_camera->onKeyBoard(key);
}

// void onMouse(GLFWwindow * window, double xx, double yy)
// {
// 	global_camera->onMouse(xx, yy);
// }

void InitCallbacks(GLFWwindow * window)
{
	glfwSetKeyCallback(window, onKeyBoard);
	// glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// glfwSetCursorPosCallback(window, onMouse);
}

int main()
{

	glfwInit();

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_CENTER_CURSOR, GL_TRUE);

	GLFWwindow * window = glfwCreateWindow(WIDTH, HEIGHT,
											"Rolling Ball", NULL, NULL);

	glfwMakeContextCurrent(window);

	glewExperimental = true;
	glewInit();

	InitCallbacks(window);

	glEnable(GL_DEPTH_TEST);
	// glEnable(GL_BLEND);
	// glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 10000.0f);

	// terrain
	vector<Terrain> terrains;
	terrains.push_back(Terrain("heightmap.jpg"));

	Shader terrainShader("terrain.vs", "terrain.fs");
	TerrainRenderer terrainRenderer(&terrainShader, projectionMatrix);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		terrainShader.bindGL();
		// terrainShader.setViewMatrix(glm::lookAt(glm::vec3(1000.0f, terrains[0].getHeight(1000.0f, 1000.0f), 1000.0f),
		terrainShader.setViewMatrix(glm::lookAt(glm::vec3(1000.0f, 100.0f, 1000.0f),
												glm::vec3(0.0f),
												glm::vec3(0.0f, 1.0f, 0.0f)));
		terrainRenderer.render(terrains);
		terrainShader.unbindGL();

		glfwSwapBuffers(window);

		glfwWaitEventsTimeout(1.0/60.0);
	} while(true);
	glfwTerminate();

}