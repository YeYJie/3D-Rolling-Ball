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

extern const int WIDTH;
extern const int HEIGHT;

extern const float WATERHEIGHT;

extern int keyPressed;
extern int mouseX;
extern int mouseY;
extern bool mouseLeftPressed;
extern bool mouseRightPressed;
extern int mouseScrollOffset;

extern int displayMenu;
extern int level;

void level2(GLFWwindow * window,
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
	// cout << "loading terrain..." << endl;
	TerrainPtr terrain(new Terrain("h2.png", 2.0f,
				[](float h)->float {
					return h / 255.0 * 100.0 - 50;
				}
		));

	// entity
	// cout << "loading entities..." << endl;
	vector<EntityPtr> entities;
	entities.push_back(static_pointer_cast<Entity>(ball));

	RawModelPtr starRawModel(new RawModel(LoadObjModel("star.obj")));
	TexturePtr starTexture(new Texture(LoadTexture("star.png")));
	TexturedModelPtr starTexturedModel(new TexturedModel(starRawModel, starTexture));
	int x = 30;
	int z = 90;
	entities.push_back(EntityPtr(
			new Entity(starTexturedModel,
			glm::vec3(x, terrain->getHeight(x, z) + 5, z),
			glm::vec3(0.0f), 8.0f)
		));

	RawModelPtr treeRawModel(new RawModel(LoadObjModel("tree.obj")));
	TexturePtr treeTexture(new Texture(LoadTexture("tree.png")));
	TexturedModelPtr treeTextureModel(new TexturedModel(treeRawModel, treeTexture));
	x = 125, z = 210;
	entities.push_back(EntityPtr(
			new Entity(treeTextureModel,
			glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
			glm::vec3(0.0f), 1.0f + float(rand() % 10000) / 10000.0f)
		));
	x = -265, z = 90;
	entities.push_back(EntityPtr(
			new Entity(treeTextureModel,
			glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
			glm::vec3(0.0f), 1.0f + float(rand() % 10000) / 10000.0f)
		));
	x = -265, z = 50;
	entities.push_back(EntityPtr(
			new Entity(treeTextureModel,
			glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
			glm::vec3(0.0f), 1.0f + float(rand() % 10000) / 10000.0f)
		));
	x = -113, z = -84;
	entities.push_back(EntityPtr(
			new Entity(treeTextureModel,
			glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
			glm::vec3(0.0f), 1.0f + float(rand() % 10000) / 10000.0f)
		));
	x = -40, z = -315;
		entities.push_back(EntityPtr(
			new Entity(treeTextureModel,
			glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
			glm::vec3(0.0f), 1.0f + float(rand() % 10000) / 10000.0f)
		));
	x = 55, z = -210;
	entities.push_back(EntityPtr(
			new Entity(treeTextureModel,
			glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
			glm::vec3(0.0f), 1.0f + float(rand() % 10000) / 10000.0f)
		));

	// water
	// cout << "loading water..." << endl;
	vector<WaterPtr> waters;
	WaterPtr water(new Water(0.0f, WATERHEIGHT, 0.0f, 1024.0f));
	waters.push_back(water);

	// cout << "loading other staff..." << endl;
	vector<GUIPtr> guis;
	// GUIPtr reflectionGUI(new GUI(waterFrameBuffer->getReflectionTexture()));
	// reflectionGUI->setPositionAndSize(0, 0, 300, 300);
	// guis.push_back(reflectionGUI);

	vector<TextPtr> texts;
	float scaleFactor = 1.0f;
	TextPtr text1(new Text(L"ye yongjie", 0, 0, scaleFactor, TEXT_MODE_NORMAL));
	texts.push_back(text1);

	// sun
	SunPtr sun(new Sun("sun.png", glm::vec3(0.0f, -1.0f, 1.0f), 1.0f));
	sun->setPosition(glm::vec3(10.0f));

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	ball->setPosition(32, 0, -210);

	// some light
	glm::vec3 lightDirection = glm::vec3(-1.0f, -1.0f, 0.0f);

	terrainShader->bindGL();
	terrainShader->setDirLight(lightDirection);
	terrainShader->unbindGL();

	entityShader->bindGL();
	entityShader->setDirLight(lightDirection);
	entityShader->unbindGL();

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do some update
		ball->update(terrain);
		camera->update(terrain);

		glm::vec3 cameraPostion = camera->getPosition();
		glm::vec3 ballPosition = ball->getPosition();

		// printVec3(ballPosition, "ballPosition");

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				// skybox
				skyboxShader->bindGL();
				skyboxShader->setViewMatrix(glm::mat4(glm::mat3(viewMatrix)));
				skyboxRenderer->render();
				skyboxShader->unbindGL();

				// terrain
				terrainShader->bindGL();
				terrainShader->setClipPlane(0, 1, 0, -WATERHEIGHT);
				terrainShader->setViewMatrix(viewMatrix);
				terrainRenderer->render(terrain);
				terrainShader->unbindGL();

				// entity
				entityShader->bindGL();
				entityShader->setClipPlane(0, 1, 0, -WATERHEIGHT);
				entityShader->setViewMatrix(viewMatrix);
				entityRenderer->render(entities);
				entityShader->unbindGL();

		ball->setPosition(ballPosition);
		camera->setPosition(cameraPostion);
		viewMatrix = camera->getViewMatrix();

		// water render to refraction buffer
		waterFrameBuffer->bindRefractionBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				// terrain
				terrainShader->bindGL();
				terrainShader->setClipPlane(0, -1, 0, WATERHEIGHT + 100);
				terrainShader->setViewMatrix(viewMatrix);
				terrainRenderer->render(terrain);
				terrainShader->unbindGL();

		glDisable(GL_CLIP_DISTANCE0);
		// render to screen
		waterFrameBuffer->unbindCurrentFrameBuffer();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if(displayMenu) {
			menuFrameBuffer->bindMenuFrameBuffer();
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}

				// skybox
				skyboxShader->bindGL();
				skyboxShader->setViewMatrix(glm::mat4(glm::mat3(viewMatrix)));
				skyboxRenderer->render();
				skyboxShader->unbindGL();

				// terrain
				terrainShader->bindGL();
				terrainShader->setViewMatrix(viewMatrix);
				terrainRenderer->render(terrain);
				terrainShader->unbindGL();

				// entity
				entityShader->bindGL();
				entityShader->setViewMatrix(viewMatrix);
				entityShader->setViewPosition(cameraPostion);
				entityRenderer->render(entities);
				entityShader->unbindGL();

				// water
				waterShader->bindGL();
				waterShader->setViewPosition(cameraPostion);
				waterShader->setDirLight(lightDirection);
				waterRenderer->render(waters, camera);
				waterShader->unbindGL();

				guiRenderer->render(guis);
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

		if(level != 2)
			break;

	} while(true);
}