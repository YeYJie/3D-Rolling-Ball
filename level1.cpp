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

void level1(GLFWwindow * window,
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
	int numStar = 10;
	int collectedStar = 0;
	int frameEpoch = 0; // detect collision every 10 frames

	// terrain
	TerrainPtr terrain(new Terrain("h1.jpg", 2.0f,
				[](float h)->float {
					return h / 255.0 * 200.0 - 50;
				}
		));

	// entity
	vector<EntityPtr> entities;
	entities.push_back(static_pointer_cast<Entity>(ball));

	RawModelPtr starRawModel(new RawModel(LoadObjModel("star.obj")));
	TexturePtr starTexture(new Texture(LoadTexture("star.png")));
	TexturedModelPtr starTexturedModel(new TexturedModel(starRawModel, starTexture));

	for(int i = 0; i < numStar; ++i) {
		float x = rand() % 800 - 400;
		float z = rand() % 800 - 400;
		entities.push_back(EntityPtr(
				new Entity(starTexturedModel,
				glm::vec3(x, terrain->getHeight(x, z) + 5, z),
				glm::vec3(0.0f), 8.0f)
			));
	}

	RawModelPtr treeRawModel(new RawModel(LoadObjModel("tree.obj")));
	TexturePtr treeTexture(new Texture(LoadTexture("tree.png")));
	TexturedModelPtr treeTextureModel(new TexturedModel(treeRawModel, treeTexture));
	static int numTree = 10;
	for(int i = 0; i < numTree; ++i) {
		float x = rand() % 800 - 400;
		float z = rand() % 800 - 400;
		entities.push_back(EntityPtr(
				new Entity(treeTextureModel,
				glm::vec3(x, terrain->getHeight(x, z), z) - 0.5f,
				glm::vec3(0.0f), 2.0f + float(rand() % 10000) / 10000.0f)
			));
	}

	ball->setPosition(0.0f, 0.0f, 0.0f);

	// shadow
	ShadowFrameBuffer * shadowFrameBuffer = new ShadowFrameBuffer();
	// GUIPtr shadowDepth(new GUI(shadowFrameBuffer->getDepthTexture()));
	// shadowDepth->setPositionAndSize(0, 0, WIDTH / 3.0f , HEIGHT / 3.0f);
	float sunPositionXY = 500.0f;
	glm::mat4 lightSpaceProjection = glm::ortho(-500.0f, 500.0f, -500.0f, 500.0f, 0.1f, 1000.0f);
	glm::mat4 lightSpaceMatrix = glm::lookAt(
											glm::vec3(sunPositionXY, sunPositionXY, 0.0f),
											glm::vec3(0.0f),
											glm::vec3(0.0f, 1.0f, 0.0f)
										);
	// lightSpaceMatrix = lightSpaceProjection * lightSpaceMatrix;

	// gui
	vector<GUIPtr> guis;
	// guis.push_back(shadowDepth);

	// text
	vector<TextPtr> texts;
	float scaleFactor = 1.0f;
	TextPtr score(new Text(L"0/10", 10, 10, 1.0f, TEXT_MODE_NORMAL));
	score->setTextColor(0.0f, 128.0f/255.0f, 1.0f);
	texts.push_back(score);

	// direction light
	glm::vec3 lightDirection = glm::vec3(-1.0f, -1.0f, 0.0f);

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	double frameBegin = 0.0;
	double frameEnd = 0.0;
	double lastFrameTime = 0.0;

	do {

		frameBegin = glfwGetTime();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do some update
		ball->update(terrain);
		camera->update(terrain);

		glm::vec3 cameraPostion = camera->getPosition();
		glm::mat4 viewMatrix = camera->getViewMatrix();
		glm::vec3 ballPosition = ball->getPosition();


		// shodow
		shadowFrameBuffer->bindGL();
			terrainShader->bindGL();
			terrainShader->setProjectionMatrix(lightSpaceProjection);
			terrainShader->setViewMatrix(lightSpaceMatrix);
			terrainRenderer->render(terrain);
			terrainShader->unbindGL();

			entityShader->bindGL();
			entityShader->setProjectionMatrix(lightSpaceProjection);
			entityShader->setViewMatrix(lightSpaceMatrix);
			entityRenderer->render(entities);
			entityShader->unbindGL();
		shadowFrameBuffer->unbindGL();



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
		terrainShader->setProjectionMatrix(projectionMatrix);
		terrainShader->setViewMatrix(viewMatrix);
		terrainShader->setLightSpaceMatrix(lightSpaceProjection * lightSpaceMatrix);

		terrainShader->bindTexture(2, shadowFrameBuffer->getDepthTextureRaw());
		terrainShader->setShadowMap(2);

		terrainShader->setDirLight(lightDirection);
		terrainShader->setNumLights(numStar - collectedStar);
		for(int i = 0; i < numStar - collectedStar; ++i)
			terrainShader->setPointLight(i, entities[i+1]->getPosition());
		terrainRenderer->render(terrain);
		terrainShader->unbindGL();


		++frameEpoch;
		if(frameEpoch % 10 == 0)
		{
			int minIndex = 0;
			float minDistance = 10000.0f;
			for(int i = 1; i <= numStar - collectedStar; ++i) {
				float tempDistance = (ballPosition.x - entities[i]->getPositionX())
								* (ballPosition.x - entities[i]->getPositionX())
								+ (ballPosition.z - entities[i]->getPositionZ())
								* (ballPosition.z - entities[i]->getPositionZ());
				if(tempDistance < minDistance) {
					minDistance = tempDistance;
					minIndex = i;
				}
			}
			// cout << minDistance << endl;
			if(minIndex != 0
				&& minIndex <= numStar - collectedStar
				&& minDistance < 400)
			{
				++collectedStar;
				entities.erase(entities.begin() + minIndex);
				score->setContentW(
							to_wstring(collectedStar)
							+ L"/"
							+ to_wstring(numStar)
							);
			}
		}

		// entity
		entityShader->bindGL();
		entityShader->setProjectionMatrix(projectionMatrix);
		entityShader->setViewMatrix(viewMatrix);
		entityShader->setLightSpaceMatrix(lightSpaceProjection * lightSpaceMatrix);

		entityShader->bindTexture(1, shadowFrameBuffer->getDepthTextureRaw());
		entityShader->setShadowMap(1);

		entityShader->setDirLight(lightDirection);
		entityShader->setViewPosition(cameraPostion);
		entityShader->setNumLights(numStar - collectedStar);
		for(int i = 0; i < numStar - collectedStar; ++i)
			entityShader->setPointLight(i, entities[i+1]->getPosition());
		entityRenderer->render(entities);
		entityShader->unbindGL();

		guiRenderer->render(guis);
		textRenderer->render(texts);

		// sunRenderer->render(sun, camera);

		if(displayMenu) {
			menuFrameBuffer->unbindMenuFrameBuffer();
			menuFrameBuffer->render();
		}


		// some shit

		keyPressed = 0;
		mouseScrollOffset = 0;

		glfwSwapBuffers(window);
		glfwWaitEventsTimeout(1.0/60);

		if(level != 1)
			break;

		frameEnd = glfwGetTime();

		// printf("%lf ms\n", (frameEnd - frameBegin) * 1000);

	} while(true);
}