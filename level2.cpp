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

void level2(GLFWwindow * window, 
			const glm::mat4 & projectionMatrix,
			
			Shader * entityShader,
			EntityRenderer * entityRenderer,
			// vector<Entity*> & entities,
			
			BallPtr ball,

			Shader * terrainShader,
			TerrainRenderer * terrainRenderer,

			Shader * skyboxShader,
			SkyboxRenderer * skyboxRenderer,

			Camera * camera,

			Shader * waterShader,
			WaterFrameBuffer * waterFrameBuffer,
			WaterRenderer * waterRenderer,
			// vector<Water> & waters,

			GUIRenderer * guiRenderer,
			// vector<GUI> & guis,

			TextRenderer * textRenderer,
			// vector<Text*> & texts,

			Menu * menuFrameBuffer,

			Shader * sunShader,
			SunRenderer * sunRenderer
			)
{
	vector<EntityPtr> entities;
	entities.push_back(static_pointer_cast<Entity>(ball));

	// terrain
	TerrainPtr terrain(new Terrain("h1.png", 5.0f));

	vector<GUIPtr> guis;

	vector<TextPtr> texts;
	float scaleFactor = 1.0f;
	TextPtr text1(new Text(L"level2", 0, 0, scaleFactor, scaleFactor,
						glm::vec3(0.0f, 1.0f, 1.0f)));
	texts.push_back(text1);

	// sun
	SunPtr sun(new Sun("sun.png", glm::vec3(0.0f, -1.0f, 1.0f), 1.0f));
	sun->setPosition(glm::vec3(10.0f));

	glViewport(0, 0, WIDTH, HEIGHT);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	do {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// do some update
		ball->update(terrain);
		camera->update(terrain);

		glm::vec3 cameraPostion = camera->getPosition();
		glm::vec3 ballPosition = ball->getPosition();

		glm::mat4 viewMatrix = camera->getViewMatrix();


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
				// terrainShader->bindGL();
				// terrainShader->setViewMatrix(viewMatrix);
				// terrainRenderer->render(terrain);
				// terrainShader->unbindGL();

				// entity
				entityShader->bindGL();
				entityShader->setViewMatrix(viewMatrix);
				entityShader->setUniform3f("viewPosition", cameraPostion);
				entityRenderer->render(entities);
				entityShader->unbindGL();


				// // water
				// waterShader->bindGL();
				// waterShader->setUniform3f("viewPosition", cameraPostion);
				// waterRenderer->render(waters, camera);
				// waterShader->unbindGL();

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

		if(level != 2)
			break;

	} while(true);
}