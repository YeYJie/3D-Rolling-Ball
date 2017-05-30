#ifndef _INCLUDE_H_
#define _INCLUDE_H_

#include <cassert>
#include <vector>
#include <string>
#include <map>
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <algorithm>
#include <memory>
#include <thread>
using namespace std;

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <SOIL/SOIL.h>

GLuint LoadTexture(const char * textureFileName);

GLuint LoadShaders(const char * vertex_file_path,
					const char * fragment_file_path);

class RawModel;

RawModel LoadObjModel(const char * objFileName);

GLuint LoadCubeMap(const vector<const char *> & cubeMapFileName);

vector<string> &split(const string &s, char delim, 
					vector<string> &elems);

vector<string> split(const string &s, char delim);

#endif
