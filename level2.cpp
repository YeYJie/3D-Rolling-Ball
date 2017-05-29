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

}