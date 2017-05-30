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

extern const float WATERHEIGHT = -10;

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
int level = 1;

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

void level1(GLFWwindow * window,
			const glm::mat4 & projectionMatrix,
			Shader * entityShader, EntityRenderer * entityRenderer, BallPtr ball,
			Shader * terrainShader, TerrainRenderer * terrainRenderer,
			Shader * skyboxShader, SkyboxRenderer * skyboxRenderer,
			Camera * camera,
			Shader * waterShader, WaterFrameBuffer * waterFrameBuffer, WaterRenderer * waterRenderer,
			GUIRenderer * guiRenderer,
			TextRenderer * textRenderer,
			Menu * menuFrameBuffer,
			Shader * sunShader,SunRenderer * sunRenderer );

void level2(GLFWwindow * window,
			const glm::mat4 & projectionMatrix,
			Shader * entityShader, EntityRenderer * entityRenderer, BallPtr ball,
			Shader * terrainShader, TerrainRenderer * terrainRenderer,
			Shader * skyboxShader, SkyboxRenderer * skyboxRenderer,
			Camera * camera,
			Shader * waterShader, WaterFrameBuffer * waterFrameBuffer, WaterRenderer * waterRenderer,
			GUIRenderer * guiRenderer,
			TextRenderer * textRenderer,
			Menu * menuFrameBuffer,
			Shader * sunShader,SunRenderer * sunRenderer );

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

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glm::mat4 projectionMatrix = glm::perspective(
										45.0f,
										float(WIDTH) / float(HEIGHT),
										0.1f,
										10000.0f);

	// entity
	Shader * entityShader = new Shader("entity.vs", "entity.fs");
	EntityRenderer * entityRenderer
						= new EntityRenderer(entityShader, projectionMatrix);
	// vector<Entity*> entities;

	RawModelPtr rawModelBall(new RawModel(LoadObjModel("ball.obj")));
	TexturePtr ballTexture(new Texture(LoadTexture("ball.png")));
	TexturedModelPtr texturedBall(new TexturedModel(rawModelBall, ballTexture));
	BallPtr ball(new Ball(texturedBall,
							glm::vec3(10.0f, 0.0f, 10.0f),
							glm::vec3(0.0f), 1.0f));

	// terrain
	Shader * terrainShader = new Shader("terrain.vs", "terrain.fs");
	TerrainRenderer * terrainRenderer
						= new TerrainRenderer(terrainShader, projectionMatrix);

	// skybox
	Shader * skyboxShader = new Shader("skybox.vs", "skybox.fs");
	SkyboxRenderer * skyboxRenderer
						= new SkyboxRenderer(skyboxShader, projectionMatrix);

	// camera
	Camera * camera = new Camera(ball);

	// water
	WaterFrameBuffer * waterFrameBuffer = new WaterFrameBuffer();
	Shader * waterShader = new Shader("water.vs", "water.fs");
	WaterRenderer * waterRenderer = new WaterRenderer(waterShader,
										projectionMatrix,
										waterFrameBuffer,
										new Texture("dudv2.png"),
										new Texture("normal.png"));
	// vector<Water> waters;

	// gui
	GUIRenderer * guiRenderer = new GUIRenderer();
	// vector<GUI> guis;

	// text
	TextRenderer * textRenderer = new TextRenderer(
							"arial.fnt", "arial.png", TEXT_SDF());
	// vector<Text*> texts;

	// menu
	Menu *  menuFrameBuffer = new Menu();

	// sun
	Shader * sunShader = new Shader("sun.vs", "sun.fs");
	SunRenderer * sunRenderer = new SunRenderer(sunShader, projectionMatrix);


	while(true) {
		if(level == 1) {
			cout << "\nswitching to level1\n\n";
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			level1(window, projectionMatrix,
					entityShader, entityRenderer, ball,
					terrainShader, terrainRenderer,
					skyboxShader, skyboxRenderer,
					camera,
					waterShader, waterFrameBuffer, waterRenderer,
					guiRenderer,
					textRenderer,
					menuFrameBuffer,
					sunShader, sunRenderer);
		}
		if(level == 2) {
			cout << "\nswitching to level2\n\n";
			glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
			level2(window, projectionMatrix,
					entityShader, entityRenderer, ball,
					terrainShader, terrainRenderer,
					skyboxShader, skyboxRenderer,
					camera,
					waterShader, waterFrameBuffer, waterRenderer,
					guiRenderer,
					textRenderer,
					menuFrameBuffer,
					sunShader, sunRenderer);
		}
	}

	glfwTerminate();

}