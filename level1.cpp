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

			Shader * entityShader,
			EntityRenderer * entityRenderer,

			BallPtr ball,

			Shader * terrainShader,
			TerrainRenderer * terrainRenderer,

			Shader * skyboxShader,
			SkyboxRenderer * skyboxRenderer,

			Camera * camera,

			Shader * waterShader,
			WaterFrameBuffer * waterFrameBuffer,
			WaterRenderer * waterRenderer,

			GUIRenderer * guiRenderer,
			TextRenderer * textRenderer,
			Menu * menuFrameBuffer,

			Shader * sunShader,
			SunRenderer * sunRenderer
			)
{
	int numStar = 10;
	int collectedStar = 0;
	int frameEpoch = 0; // detect collision every 10 frames

	// terrain
	TerrainPtr terrain(new Terrain("h1.jpg", 2.0f));

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

	// gui
	vector<GUIPtr> guis;

	// text
	vector<TextPtr> texts;
	float scaleFactor = 1.0f;
	TextPtr text1(new Text(L"0 / 10", 0, 0, scaleFactor, scaleFactor,
						glm::vec3(0.0f, 1.0f, 1.0f)));
	texts.push_back(text1);

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
		// cout << "bal : " << ballPosition.x
		// 		<< " " << ballPosition.y
		// 		<< " " << ballPosition.z << endl;

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
		terrainShader->setUniform3f("dirLight", lightDirection);
		terrainShader->setUniform1i("numLights", numStar - collectedStar);
		for(int i = 0; i < numStar - collectedStar; ++i) {
			string pointLight = "pointLight[" + to_string(i) + "]";
			terrainShader->setUniform3f(pointLight.c_str(), entities[i+1]->getPosition());
		}
		terrainRenderer->render(terrain);
		terrainShader->unbindGL();


		++frameEpoch;
		if(frameEpoch % 10 == 0)
		{
			int minIndex = 0;
			float minDistance = 10000.0f;
			for(int i = 1; i < entities.size(); ++i) {
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
				text1->setContentW(
							to_wstring(collectedStar)
							+ L" / "
							+ to_wstring(numStar)
							);
			}
		}

		// entity
		entityShader->bindGL();
		entityShader->setViewMatrix(viewMatrix);
		entityShader->setUniform3f("dirLight", lightDirection);
		entityShader->setUniform1i("numLights", numStar - collectedStar);
		for(int i = 0; i < numStar - collectedStar; ++i) {
			string pointLight = "pointLight[" + to_string(i) + "]";
			entityShader->setUniform3f(pointLight.c_str(), entities[i+1]->getPosition());
		}
		entityShader->setUniform3f("viewPosition", cameraPostion);
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

		printf("%lf ms\n", (frameEnd - frameBegin) * 1000);

	} while(true);
}