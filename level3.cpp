#include "include.h"
#include "ball.h"
#include "camera.h"
#include "entity.h"
#include "entityShader.h"
#include "entityRenderer.h"
#include "shader.h"
#include "skybox.h"
#include "skyboxShader.h"
#include "terrain.h"
#include "terrainShader.h"
#include "terrainRenderer.h"
#include "texture.h"
#include "gui.h"
#include "water.h"
#include "waterShader.h"
#include "text.h"
#include "menu.h"
#include "sun.h"
#include "sunShader.h"
#include "shadow.h"
#include "hdr.h"
#include "light.h"

extern const int WIDTH;
extern const int HEIGHT;

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

extern int displayMenu;
extern int level;

void level3(GLFWwindow * window,
			const glm::mat4 & projectionMatrix,

			EntityShader * entityShader,
			EntityRenderer * entityRenderer,

			BallPtr ball,

			TerrainShader * terrainShader,
			TerrainRenderer * terrainRenderer,

			SkyboxShader * skyboxShader,
			SkyboxRenderer * skyboxRenderer,

			Camera * camera,

			WaterShader * waterShader,
			WaterFrameBuffer * waterFrameBuffer,
			WaterRenderer * waterRenderer,

			GUIRenderer * guiRenderer,
			TextRenderer * textRenderer,
			Menu * menuFrameBuffer,

			SunShader * sunShader,
			SunRenderer * sunRenderer
			)
{
	// terrain
	TerrainPtr terrain(new Terrain("h3.jpg", 100.0f,
				[](float h)->float {
					return 1.0f;
				}
		));

	// entity
	vector<EntityPtr> entities;
	entities.push_back(static_pointer_cast<Entity>(ball));
	ball->setPosition(-200.0f, 0.0f, -200.0f);

	RawModelPtr boxRawModel(new RawModel(LoadObjModel("box.obj")));

	TexturePtr yellowTexture(new Texture(LoadTexture("yellow.png")));
	TexturePtr redTexture(new Texture(LoadTexture("red.png")));
	TexturePtr greenTexture(new Texture(LoadTexture("green.png")));
	TexturePtr blueTexture(new Texture(LoadTexture("blue.png")));

	TexturedModelPtr yellowBox(new TexturedModel(boxRawModel, yellowTexture));
	TexturedModelPtr redBox(new TexturedModel(boxRawModel, redTexture));
	TexturedModelPtr greenBox(new TexturedModel(boxRawModel, greenTexture));
	TexturedModelPtr blueBox(new TexturedModel(boxRawModel, blueTexture));

	// ball->setModel(yellowBox);

	LightPtr yellowLight(new Light());
	yellowLight->setModel(yellowBox);
	yellowLight->setPosition(glm::vec3(50 - 200, terrain->getHeight(50 - 200, 50 - 200) + 20, 50 - 200));
	yellowLight->setScale(8.0f);
	yellowLight->setType(POINT_LIGHT);
	yellowLight->setAmbient(0.1, 0.1, 0.0);
	yellowLight->setDiffuse(0.5, 0.5, 0.0);
	yellowLight->setSpecular(0.3, 0.3, 0.0);
	// yellowLight->setDirty(2);
	// yellowLight->setShininess(1.0);
	entities.push_back(static_pointer_cast<Entity>(yellowLight));

	LightPtr redLight(new Light());
	redLight->setModel(redBox);
	redLight->setPosition(glm::vec3(-50 - 200, terrain->getHeight(-50 - 200, -50 - 200) + 50, -50 - 200));
	redLight->setScale(8.0f);
	redLight->setType(POINT_LIGHT);
	redLight->setAmbient(0.1, 0.0, 0.0);
	redLight->setDiffuse(0.5, 0.0, 0.0);
	redLight->setSpecular(0.3, 0.0, 0.0);
	// redLight->setShininess(1.0);
	entities.push_back(static_pointer_cast<Entity>(redLight));

	LightPtr greenLight(new Light());
	greenLight->setModel(greenBox);
	greenLight->setPosition(glm::vec3(50 - 200, terrain->getHeight(50 - 200, -50 - 200) + 30, -50 - 200));
	greenLight->setScale(8.0f);
	greenLight->setType(POINT_LIGHT);
	greenLight->setAmbient(0.0, 0.1, 0.0);
	greenLight->setDiffuse(0.0, 0.5, 0.0);
	greenLight->setSpecular(0.0, 0.3, 0.0);
	// greenLight->setShininess(1.0);
	entities.push_back(static_pointer_cast<Entity>(greenLight));

	LightPtr blueLight(new Light());
	blueLight->setModel(blueBox);
	blueLight->setPosition(glm::vec3(-50 - 200, terrain->getHeight(-50 - 200, 50 - 200) + 20, 70 - 200));
	blueLight->setScale(8.0f);
	blueLight->setType(POINT_LIGHT);
	blueLight->setAmbient(0.0, 0.0, 20.0);
	blueLight->setDiffuse(0.0, 0.0, 20.0);
	blueLight->setSpecular(0.0, 0.0, 3.0);
	blueLight->setShininess(32.0);
	entities.push_back(static_pointer_cast<Entity>(blueLight));


	LightPtr directionLight(new Light());
	directionLight->setType(DIRECTION_LIGHT);
	directionLight->setDirection(glm::vec3(-1.0f, -1.0f, 0.0f));
	directionLight->setAmbient(0.1);
	directionLight->setDiffuse(0.5);
	directionLight->setSpecular(0.3);
	// directionLight->setShininess(1.0);


	entityShader->bindGL();
	entityShader->resetLight();
	entityShader->addLight(directionLight);
	entityShader->addLight(yellowLight);
	entityShader->addLight(redLight);
	entityShader->addLight(greenLight);
	entityShader->addLight(blueLight);
	entityShader->unbindGL();

	terrainShader->bindGL();
	terrainShader->resetLight();
	terrainShader->addLight(directionLight);
	terrainShader->addLight(yellowLight);
	terrainShader->addLight(redLight);
	terrainShader->addLight(greenLight);
	terrainShader->addLight(blueLight);
	terrainShader->unbindGL();


	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	double frameBegin = 0.0;
	double frameEnd = 0.0;
	double lastFrameTime = 0.0;

	HDR * hdr = new HDR();

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	do {

		frameBegin = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do some update
		ball->update(terrain);
		camera->update(terrain);

		glm::vec3 cameraPostion = camera->getPosition();
		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::vec3 ballPosition = ball->getPosition();

		// printVec3(ballPosition, "ballPosition");

		if(displayMenu) {
			menuFrameBuffer->bindMenuFrameBuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
		else {
			hdr->bindGL();
		}

			// terrain
			terrainShader->bindGL();
			terrainShader->setViewMatrix(viewMatrix);
			terrainShader->updateLight();
			terrainRenderer->render(terrain);
			terrainShader->unbindGL();


			// entity
			entityShader->bindGL();
			entityShader->setViewMatrix(viewMatrix);
			entityShader->setViewPosition(cameraPostion);
			entityShader->updateLight();
			entityRenderer->render(entities);
			entityShader->unbindGL();


		if(displayMenu) {
			menuFrameBuffer->unbindMenuFrameBuffer();
			menuFrameBuffer->render();
		}
		else {
			hdr->unbindGL();
			hdr->render(guiRenderer);
		}

		// some shit

		keyPressed = 0;
		mouseScrollOffset = 0;

		glfwSwapBuffers(window);
		glfwWaitEventsTimeout(1.0/60);

		if(level != 3)
			break;

		frameEnd = glfwGetTime();

		// printf("%lf ms\n", (frameEnd - frameBegin) * 1000);

	} while(true);
}