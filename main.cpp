#include "include.h"
#include "ball.h"
#include "camera.h"
#include "entity.h"
#include "entityRenderer.h"
#include "shader.h"
#include "skybox.h"
#include "terrain.h"
#include "terrainRenderer.h"
#include "texture.h"
#include "gui.h"
#include "water.h"
#include "text.h"
#include "menu.h"
#include "sun.h"

extern const int WIDTH = 1422;
extern const int HEIGHT = 800;

extern const float WATERHEIGHT = 20;

// Keyboard and Mouse
//
// glfw is a piece of shit :-)

int keyPressed = 0;
int mouseX = 0;
int mouseY = 0;
bool mouseLeftPressed = false;
bool mouseRightPressed = false;
int mouseScrollOffset = 0;

int displayMenu = 0;

void onKeyBoard(GLFWwindow * window, int key,
				int scancode, int action, int mods)
{
	// if(key == GLFW_KEY_Q || key == GLFW_KEY_ESCAPE)
	// 	exit(0);
	if(key == GLFW_KEY_Q)
		exit(0);
	else if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		displayMenu = (displayMenu + 1) % 2;
		if(displayMenu)
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		else
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}
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

	// glEnable(GL_CULL_FACE);
	// glCullFace(GL_BACK);

	glm::mat4 projectionMatrix = glm::perspective(45.0f, float(WIDTH) / float(HEIGHT), 0.1f, 10000.0f);

	// entity
	Shader entityShader("entity.vs", "entity.fs");
	EntityRenderer entityRenderer(&entityShader, projectionMatrix);

	vector<Entity*> entities;

	RawModel rawModelBall = LoadObjModel("ball.obj");
	TexturedModel texturedBall(rawModelBall, Texture("ball.png"));
	Ball * ball = new Ball(&texturedBall, 
								glm::vec3(10.0f, 0.0f, 10.0f),
								// glm::vec3(82.0f, 100.f, 70.0f),
								glm::vec3(0.0f), 1.0f);
	entities.push_back(ball);
	TexturedModel * texturedTree = new TexturedModel(
										LoadObjModel("tree.obj"),
										Texture("tree.png")
									);
	// Entity * speedBall = new Entity(&texturedBall,
	// 							ball->getPosition() + ball->getSpeed() * 20.0f,
	// 							glm::vec3(0.0f), 1.0f);
	// entities.push_back(speedBall);
	// Entity * tree = new Entity(texturedTree,
	// 							glm::vec3(15.0f, 25.0f, 15.0f),
	// 							glm::vec3(0.0f), 1.0f);
	// entities.push_back(tree);

	// terrain
	Terrain terrain("h1.png", 5.0f);
	Shader * terrainShader = new Shader("terrain.vs", "terrain.fs");
	TerrainRenderer terrainRenderer(terrainShader, projectionMatrix);

	// skybox
	Shader skyboxShader("skybox.vs", "skybox.fs");
	SkyboxRenderer skyboxRenderer(&skyboxShader, projectionMatrix);


	// camera
	Camera * camera = new Camera(ball);

	// water
	WaterFrameBuffer * waterFrameBuffer = new WaterFrameBuffer();
	Shader * waterShader = new Shader("water.vs", "water.fs");
	Texture * dudvMap = new Texture("dudv2.png");
	Texture * normalMap = new Texture("normal.png");
	WaterRenderer * waterRenderer = new WaterRenderer(waterShader, 
										projectionMatrix, 
										waterFrameBuffer,
										dudvMap, normalMap);
	vector<Water> waters;
	waters.push_back(Water(0.0f, -10, 0.0f, 1024.0f));



	// gui
	vector<GUI> guis;
	GUIRenderer guiRenderer;

	// GUI sunGUI(Texture("sun.png"));
	// sunGUI.setPositionAndSize(0, 0, 500, 500);
	// guis.push_back(sunGUI);

	// GUI dudvGUI(Texture("text.png"));
	// dudvGUI.setPositionAndSize(100, 100, 500, 500);
	// guis.push_back(dudvGUI);

	// GUI guiReflection(Texture(waterFrameBuffer->getReflectionTexture()));
	// guiReflection.setPositionAndSize(0, 0, 1000, 1000);
	// guis.push_back(guiReflection);

	// GUI guiRefraction(Texture(waterFrameBuffer->getRefractionTexture()));
	// guiRefraction.setPositionAndSize(600, 0, 400, 400);
	// guis.push_back(guiRefraction);

	// Text
	// TextRenderer * textRenderer = new TextRenderer("Ubuntu-B.ttf", TEXT_TTF());
	TextRenderer * textRenderer = new TextRenderer(
							"arial.fnt", "arial.png", TEXT_SDF());
	// Text * yeyongjie = new Text("yeyongjie");
	vector<Text*> texts;
	// texts.push_back(new Text("ye yong jie", 100, 100, 1, 
	// 							glm::vec3(1.0f, 1.0f, 0.0f)));
	// texts.push_back(new Text("Y", 100, 300, 10));
	float scaleFactor = 1.0f;
	texts.push_back(new Text(L"ye yongjie", 0, 0, scaleFactor, scaleFactor,
						glm::vec3(0.0f, 1.0f, 1.0f)));

	// menu
	Menu *  menuFrameBuffer = new Menu();

	// sun
	Sun sun("sun.png", glm::vec3(0.0f, -1.0f, 1.0f), 1.0f);
	sun.setPosition(glm::vec3(10.0f));
	Shader * sunShader = new Shader("sun.vs", "sun.fs");
	SunRenderer * sunRenderer = new SunRenderer(sunShader, projectionMatrix);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do some update
		ball->update(terrain);
		camera->update(terrain);
		// speedBall->setPosition(ball->getPosition() + ball->getSpeed() * 20.0f);

		glm::vec3 cameraPostion = camera->getPosition();
		glm::vec3 ballPosition = ball->getPosition();

		float ballDistanceFromWater = ballPosition.y - WATERHEIGHT;
		ball->setPosition(ballPosition.x,
							ballPosition.y - 2 * ballDistanceFromWater,
							ballPosition.z);
		float cameraDistanceFromWater = cameraPostion.y - WATERHEIGHT;
		camera->setPosition(cameraPostion.x, 
							cameraPostion.y - 2 * cameraDistanceFromWater,
							cameraPostion.z);
		glm::mat4 viewMatrix = camera->getViewMatrix();


		glEnable(GL_CLIP_DISTANCE0);
		// water render to reflection buffer
		waterFrameBuffer->bindReflectionBuffer();

				// skybox
				skyboxShader.bindGL();
				skyboxShader.setViewMatrix(glm::mat4(glm::mat3(viewMatrix)));
				skyboxRenderer.render();
				skyboxShader.unbindGL();

				// terrain
				terrainShader->bindGL();
				terrainShader->setUniform4f("clipPlane", 0, 1, 0, -20);
				terrainShader->setViewMatrix(viewMatrix);
				terrainRenderer.render(terrain);
				terrainShader->unbindGL();

				// entity
				entityShader.bindGL();
				entityShader.setUniform4f("clipPlane", 0, 1, 0, 20);
				entityShader.setViewMatrix(viewMatrix);
				entityRenderer.render(entities);
				entityShader.unbindGL();

		ball->setPosition(ballPosition);
		camera->setPosition(cameraPostion);
		viewMatrix = camera->getViewMatrix();

		// water render to refraction buffer
		waterFrameBuffer->bindRefractionBuffer();

				// skybox
				// skyboxShader.bindGL();
				// skyboxShader.setViewMatrix(glm::mat4(glm::mat3(viewMatrix)));
				// skyboxRenderer.render();
				// skyboxShader.unbindGL();

				// terrain
				terrainShader->bindGL();
				terrainShader->setUniform4f("clipPlane", 0, -1, 0, 20);
				terrainShader->setViewMatrix(viewMatrix);
				terrainRenderer.render(terrain);
				terrainShader->unbindGL();

				// entity
				// entityShader.bindGL();
				// entityShader.setUniform4f("clipPlane", 0, -1, 0, 20);
				// entityShader.setViewMatrix(viewMatrix);
				// entityRenderer.render(entities);
				// entityShader.unbindGL();

		glDisable(GL_CLIP_DISTANCE0);
		// render to screen
		waterFrameBuffer->unbindCurrentFrameBuffer();

		if(displayMenu) {
			menuFrameBuffer->bindMenuFrameBuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

				// skybox
				skyboxShader.bindGL();
				skyboxShader.setViewMatrix(glm::mat4(glm::mat3(viewMatrix)));
				skyboxRenderer.render();
				skyboxShader.unbindGL();
			
				// terrain
				terrainShader->bindGL();
				terrainShader->setViewMatrix(viewMatrix);
				terrainRenderer.render(terrain);
				terrainShader->unbindGL();

				// entity
				entityShader.bindGL();
				entityShader.setViewMatrix(viewMatrix);
				entityShader.setUniform3f("viewPosition", cameraPostion);
				entityRenderer.render(entities);
				entityShader.unbindGL();

				// static float rx = 0;
				// ball->setRotation(rx, 0.0f, 0.0f);
				// rx += 0.005f;

				// // // water
				// waterShader->bindGL();
				// waterShader->setUniform3f("viewPosition", cameraPostion);
				// waterRenderer->render(waters, camera);
				// waterShader->unbindGL();

				guiRenderer.render(guis);
				textRenderer->render(texts);

				sunRenderer->render(sun, camera);

		if(displayMenu) {
			menuFrameBuffer->unbindMenuFrameBuffer();
			menuFrameBuffer->render();
		}


		// some shit

		keyPressed = 0;
		mouseScrollOffset = 0;

		glfwSwapBuffers(window);
		glfwWaitEventsTimeout(1.0/60);
	} while(true);

	glfwTerminate();

}