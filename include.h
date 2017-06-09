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
#include <functional>
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

void calculateOrtho(const glm::vec3 & cameraPosition,
						const glm::vec3 & cameraLookAt,
						const glm::vec3 & lightDirection,
						glm::mat4 & view,
						glm::mat4 & projection);

// some print function

inline void printVec3(const glm::vec3 & v, const char * s = "vec3") {
	printf("%s : %f %f %f\n", s, v.x, v.y, v.z);
}

inline void printMat4(const glm::mat4 & m, const char * s = "mat4") {
	printf(	"%s :\n"
			"\t%f %f %f %f \n"
			"\t%f %f %f %f \n"
			"\t%f %f %f %f \n"
			"\t%f %f %f %f \n", s,
			m[0][0], m[1][0], m[2][0], m[3][0],
			m[0][1], m[1][1], m[2][1], m[3][1],
			m[0][2], m[1][2], m[2][2], m[3][2],
			m[0][3], m[1][3], m[2][3], m[3][3]);
}

#endif