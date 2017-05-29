#include "include.h"
#include "model.h"

static GLuint LoadTextureFromPNG(const char * textureFileName);
static GLuint LoadTextureFromJPG(const char * textureFileName);


GLuint LoadTexture(const char * textureFileName)
{
	if(!textureFileName)
		return 0;
	int len = strlen(textureFileName);
	if(textureFileName[len-1] == 'g'
		&& textureFileName[len-2] == 'p'
		&& textureFileName[len-3] == 'j')
	{
		return LoadTextureFromJPG(textureFileName);
	}
	else if(textureFileName[len-1] == 'g'
		&& textureFileName[len-2] == 'n'
		&& textureFileName[len-3] == 'p')
	{
		return LoadTextureFromPNG(textureFileName);
	}
}

static GLuint LoadTextureFromPNG(const char * textureFileName)
{
	string temp(textureFileName);
	temp = "./res/" + temp;
	const char * _textureFileName = temp.c_str();

	cout << "Loading texture from png : " << _textureFileName << endl;

	int width = 0, height = 0;
	unsigned char * imageData = SOIL_load_image(_textureFileName,
								&width, &height, 0, SOIL_LOAD_RGBA);

	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height,
					0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return textureID;
}

static GLuint LoadTextureFromJPG(const char * textureFileName)
{
	string temp(textureFileName);
	temp = "./res/" + temp;
	const char * _textureFileName = temp.c_str();

	cout << "Loading texture from jpg : " << _textureFileName << endl;

	int width = 0, height = 0;
	unsigned char * imageData = SOIL_load_image(_textureFileName,
								&width, &height, 0, SOIL_LOAD_RGB);

	GLuint textureID;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height,
					0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
	glBindTexture(GL_TEXTURE_2D, 0);
	
	return textureID;
}

GLuint LoadShaders(const char * vertex_file_path,
					const char * fragment_file_path)
{

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	string VertexShaderCode;
	ifstream VertexShaderStream(vertex_file_path, ios::in);
	if(VertexShaderStream.is_open()){
		string Line = "";
		while(getline(VertexShaderStream, Line)) {
			// cout <<  "[" << Line << "]" << endl;
			VertexShaderCode += "\n" + Line;
		}
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. "
			"Are you in the right directory ? "
			"Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	string FragmentShaderCode;
	ifstream FragmentShaderStream(fragment_file_path, ios::in);
	if(FragmentShaderStream.is_open()){
		string Line = "";
		while(getline(FragmentShaderStream, Line)) {
			// cout <<  "[" << Line << "]" << endl;
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}


	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


// string split
vector<string> &split(const string &s, char delim, 
					vector<string> &elems) 
{
    stringstream ss(s);
    string item;
    while(getline(ss, item, delim)) {
    	if(item.empty()) continue;
        elems.push_back(item);
    }
    return elems;
}

vector<string> split(const string &s, char delim) 
{
    vector<string> elems;
    return split(s, delim, elems);
}

static void processVertex(vector<string> & vertexData, 
									vector<unsigned int> & indices, 
									vector<glm::vec2> & textures, 
									vector<glm::vec3> & normals, 
									vector<float> & textureArray, 
									vector<float> & normalsArray) 
{
	// if(!vertexData[0].empty()) {
		unsigned int currentVertexPointer = stoi(vertexData[0]) - 1;
		indices.push_back(currentVertexPointer);
	// }
	if(!vertexData[1].empty()) {
		glm::vec2 currentTex = textures[stoi(vertexData[1]) - 1];
		textureArray[currentVertexPointer * 2] = currentTex.x;
		textureArray[currentVertexPointer * 2 + 1] = 1 - currentTex.y;
	}
	if(!vertexData[2].empty()) {
		glm::vec3 currentNorm = normals[stoi(vertexData[2]) - 1];
		normalsArray[currentVertexPointer * 3] = currentNorm.x;
		normalsArray[currentVertexPointer * 3 + 1] = currentNorm.y;
		normalsArray[currentVertexPointer * 3 + 2] = currentNorm.z;
	}
}


RawModel LoadObjModel(const char * objFileName)
{
	string temp(objFileName);
	temp = "./res/" + temp;
	const char * _objFileName = temp.c_str();

	cout << "Loading obj : " << _objFileName << endl;

	RawModel res;

	ifstream objFile(_objFileName, ios::in);
	if(objFile.is_open())
	{
		string line = "";

		vector<glm::vec3> vertices;
		vector<glm::vec2> textures;
		vector<glm::vec3> normals;
		vector<unsigned int> indices;
		vector<float> verticesArray;
		vector<float> normalsArray;
		vector<float> textureArray;
		vector<unsigned int> indicesArray;

		while(true) 
		{
			char * pEnd;
			getline(objFile, line);
			vector<string> currentLine = split(line, ' ');
			if(currentLine[0] == "v") {
				vertices.push_back(
								glm::vec3(
										strtof(currentLine[1].c_str(), &pEnd),
										strtof(currentLine[2].c_str(), &pEnd),
										strtof(currentLine[3].c_str(), &pEnd)
									)
								);
			}
			else if(currentLine[0] == "vt") {
				textures.push_back(
								glm::vec2(
										strtof(currentLine[1].c_str(), &pEnd),
										strtof(currentLine[2].c_str(), &pEnd)
									)
								);
			}
			else if(currentLine[0] == "vn") {
				normals.push_back(
								glm::vec3(
										strtof(currentLine[1].c_str(), &pEnd),
										strtof(currentLine[2].c_str(), &pEnd),
										strtof(currentLine[3].c_str(), &pEnd)
									)
								);
			}
			else if(currentLine[0] == "f") {
				textureArray =  vector<float>(vertices.size() * 2, 0);
				normalsArray = vector<float>(vertices.size() * 3, 3);
				break;
			}
		}

		while(!line.empty())
		{
			if(line[0] != 'f') {
				getline(objFile, line);
				continue;
			}

			vector<string> currentLine = split(line, ' ');

			vector<string> vertex1 = split(currentLine[1], '/');
			vector<string> vertex2 = split(currentLine[2], '/');
			vector<string> vertex3 = split(currentLine[3], '/');

			processVertex(vertex1, indices, textures, normals, 
								textureArray, normalsArray);
			processVertex(vertex2, indices, textures, normals, 
								textureArray, normalsArray);
			processVertex(vertex3, indices, textures, normals, 
								textureArray, normalsArray);

			getline(objFile, line);
		}
		objFile.close();


		verticesArray = vector<float>(vertices.size() * 3);
		indicesArray = vector<unsigned int>(indices.size());

		int vertexPointer = 0;
		for (const glm::vec3 & vertex : vertices) {
			verticesArray[vertexPointer++] = vertex.x;
			verticesArray[vertexPointer++] = vertex.y;
			verticesArray[vertexPointer++] = vertex.z;
		}

		for (int i = 0; i < indices.size(); i++) {
			indicesArray[i] = indices[i];
		}
		return RawModel(verticesArray, textureArray, normalsArray, indicesArray);



	}else{
		cout << "fail to open : " << _objFileName << endl;
		return res;
	}
}

GLuint LoadCubeMap(const vector<const char *> & cubeMapFileName)
{
    GLuint textureID;
    glGenTextures(1, &textureID);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for(int i = 0; i < 6; ++i) {

    	cout << "Loading cube map : " << cubeMapFileName[i] << endl;

    	int width = 0, height = 0;
    	unsigned char * imgData = SOIL_load_image(cubeMapFileName[i],
								&width, &height, 0, SOIL_LOAD_RGBA);

        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,
        			GL_RGBA,
                    width, height,
                    0,
                    GL_RGBA,
                    GL_UNSIGNED_BYTE, imgData);

        // can not free image here :-), or SIGSEG !
        // SOIL_free_image_data(imgData);
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}