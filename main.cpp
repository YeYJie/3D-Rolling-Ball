#include "include.h"
#include "ball.h"
#include "camera.h"
#include "entity.h"
#include "entityRenderer.h"
#include "shader.h"
#include "terrain.h"
#include "terrainRenderer.h"
#include "texture.h"

extern const int WIDTH = 1000;
extern const int HEIGHT = 1000;

// Keyboard and Mouse
//
// glfw is a piece of shit :-)

int keyPressed = 0;
int mouseX = 0;
int mouseY = 0;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;
int mouseScrollOffset = 0;

void onKeyBoard(GLFWwindow * window, int key,
				int scancode, int action, int mods)
{
	if(key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE)
		exit(0);
	else
		keyPressed = key;
}

void onMousePosition(GLFWwindow * window, double xx, double yy)
{
	mouseX = xx;
	mouseY = yy;
}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if(button == GLFW_MOUSE_BUTTON_LEFT) {
		if(action == GLFW_PRESS) mouseLeftPressed = true;
		if(action == GLFW_RELEASE) mouseLeftPressed = false;
	}
	else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
		if(action == GLFW_PRESS) mouseRightPressed = true;
		if(action == GLFW_RELEASE) mouseRightPressed = false;
	}
}

void onMouseScroll(GLFWwindow* window, double xoffset, double yoffset)
{
	// cout << "scroll : " << yoffset << endl;
	mouseScrollOffset = yoffset;
}

void InitCallbacks(GLFWwindow * window)
{
	glfwSetKeyCallback(window, onKeyBoard);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, onMousePosition);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetScrollCallback(window, onMouseScroll);
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

	// entity
	Shader entityShader("entity.vs", "entity.fs");
	EntityRenderer entityRenderer(&entityShader, projectionMatrix);

	vector<Entity*> entities;

	RawModel rawModelBall = LoadObjModel("ball.obj");
	TexturedModel texturedBall(rawModelBall, Texture("box.png"));
	Ball * ball = new Ball(&texturedBall, 
								glm::vec3(500.0f, 100.f, 500.0f),
								glm::vec3(0.0f), 1.0f);
	entities.push_back(ball);

	// terrain
	vector<Terrain> terrains;
	terrains.push_back(Terrain("heightmap.jpg"));

	Shader terrainShader("terrain.vs", "terrain.fs");
	TerrainRenderer terrainRenderer(&terrainShader, projectionMatrix);

	// camera
	Camera * camera = new Camera(ball);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do some update
		camera->update();

		glm::mat4 viewMatrix = camera->getViewMatrix();

		// terrain
		terrainShader.bindGL();
		terrainShader.setViewMatrix(viewMatrix);
		terrainRenderer.render(terrains);
		terrainShader.unbindGL();

		// entity
		entityShader.bindGL();
		entityShader.setViewMatrix(viewMatrix);
		entityRenderer.render(entities);
		entityShader.unbindGL();

		static float rx = 0;
		ball->setRotation(rx, 0.0f, 0.0f);
		rx += 0.001f;

		// some shit

		keyPressed = 0;
		mouseScrollOffset = 0;

		glfwSwapBuffers(window);
		glfwWaitEventsTimeout(1.0/60.0);
	} while(true);

	glfwTerminate();

}