#include "terrain.h"

Terrain::Terrain(const char * heightMapFileName)
{
	loadHeightMap(heightMapFileName);

	vector<const char*> textureFiles = { "grass.png", "mud.png" };
	loadTexture(textureFiles);
}

void Terrain::bindGL() const
{
	glBindVertexArray(_VAO);
}

void Terrain::unbindGL() const
{
	glBindVertexArray(0);
}

void Terrain::draw() const
{
	glDrawElements(GL_TRIANGLES, _indicesNum, GL_UNSIGNED_INT, 0);
}

typedef vector<float> Array;
typedef vector<Array> Matrix;
typedef vector<Array> Image;

static Matrix getGaussian(int height, int width, double sigma)
{
    Matrix kernel(height, Array(width));
    double sum=0.0;
    int i,j;

    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] = exp(-(i*i+j*j)/(2*sigma*sigma))/(2*M_PI*sigma*sigma);
            sum += kernel[i][j];
        }
    }

    for (i=0 ; i<height ; i++) {
        for (j=0 ; j<width ; j++) {
            kernel[i][j] /= sum;
        }
    }

    return kernel;
}

static void applyFilter(Image & image, Matrix & filter){

    int height = image.size();
    int width = image[0].size();

    int filterHeight = filter.size();
    int filterWidth = filter[0].size();

    int newImageHeight = height - filterHeight + 1;
    int newImageWidth = width - filterWidth + 1;

    Image newImage = vector<vector<float>>(height, vector<float>(width, 0));

	for (int i = 0 ; i < newImageHeight ; i++)
	{
		for (int j = 0 ; j < newImageWidth ; j++)
		{
			for (int h = i ; h < i + filterHeight ; h++)
			{
				for (int w = j ; w < j + filterWidth ; w++)
					newImage[i][j] += filter[h-i][w-j] * image[h][w];
			}
		}
	}
	image.swap(newImage);
}

static void smooth(Image & image)
{
	Matrix filter = getGaussian(5, 5, 100.0);
	applyFilter(image, filter);
}

void Terrain::loadHeightMap(const char * heightMapFileName)
{
	_heightMap = vector<vector<float>>(_size, vector<float>(_size, 0));

	int width = 0, height = 0, channels = 0;
	unsigned char * imgData = SOIL_load_image(heightMapFileName,
								&width, &height, &channels, SOIL_LOAD_L);
	int index = 0;
	for(int i = 0; i < height; ++i) {
		for(int j = 0; j < width; ++j) {
			_heightMap[i][j] = float(256 - imgData[index++]) / 255.0 * 100.0;
			// if(_heightMap[i][j] > 32) _heightMap[i][j] = 32;
		}
	}
	SOIL_free_image_data(imgData);

	smooth(_heightMap);


	vector<float> position;
	vector<float> normal;
	vector<float> textCoords;
	vector<unsigned int> indices;

	height = _heightMap.size();
	width = _heightMap[0].size();
	for(int z = 0; z < height; ++z)
	{
		for(int x = 0; x < width; ++x)
		{
			position.push_back(x);
			position.push_back(_heightMap[z][x]);
			position.push_back(z);

			glm::vec3 temp = calculateNormal(x, z);
			normal.push_back(temp.x);
			normal.push_back(temp.y);
			normal.push_back(temp.z);

			textCoords.push_back(x/50.0f);
			textCoords.push_back(z/50.0f);
		}
	}

	// I dont know why 5 works and any < 5 doesnt works
	for(int z = 0; z < _size - 5; ++z)
	{
		for(int x = 0; x < _size - 5; ++x)
		{
			int topLeft = z * _size + x;
			int topRight = topLeft + 1;
			int bottomLeft = topLeft + _size;
			int bottomRight = bottomLeft + 1;
			indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
		}
	}
	_indicesNum = indices.size();

	initGL(position, normal, textCoords, indices);
}

void Terrain::initGL(const vector<float> & position,
						const vector<float> & normal,
						const vector<float> & textCoords,
						const vector<unsigned int> & indices)
{
	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	glGenBuffers(1, &_positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
	glBufferData(GL_ARRAY_BUFFER, position.size() * sizeof(float),
					position.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &_textCoordsVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _textCoordsVBO);
	glBufferData(GL_ARRAY_BUFFER, textCoords.size() * sizeof(float),
					textCoords.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &_normalVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _normalVBO);
	glBufferData(GL_ARRAY_BUFFER, normal.size() * sizeof(float),
					normal.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glGenBuffers(1, &_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
					indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

glm::vec3 Terrain::calculateNormal(int x, int z)
{
	float heightL = x - 1 >= 0 ? _heightMap[z][x-1] : 0;
	float heightR = x + 1 < _size ? _heightMap[z][x+1] : 0;
	float heightD = z - 1 >= 0 ? _heightMap[z-1][x] : 0;
	float heightU = z + 1 < _size ? _heightMap[z+1][x] : 0;
	glm::vec3 normal(heightL - heightR, 2.0f, heightD - heightU);
	return glm::normalize(normal);
}

void Terrain::loadTexture(const vector<const char*> & textureFiles)
{
	for(auto i : textureFiles)
		_texture.push_back(Texture(LoadTexture(i)));
}

glm::vec3 Terrain::getGradient(const float x, const float z) const 
{
	float _0 = getHeight(x, z);
	float tmin = _0;
	int index = 0;
	
	float _1 = getHeight(x, z-1);
	float _2 = getHeight(x+1, z-1);
	float _3 = getHeight(x+1, z);
	float _4 = getHeight(x+1, z+1);
	float _5 = getHeight(x, z+1);
	float _6 = getHeight(x-1, z+1);
	float _7 = getHeight(x-1, z);
	float _8 = getHeight(x-1, z-1);

	if(tmin > _1)
		tmin = _1, index = 1;
	if(tmin > _2)
		tmin = _2, index = 2;
	if(tmin > _3)
		tmin = _3, index = 3;
	if(tmin > _4)
		tmin = _4, index = 4;
	if(tmin > _5)
		tmin = _5, index = 5;
	if(tmin > _6)
		tmin = _6, index = 6;
	if(tmin > _7)
		tmin = _7, index = 7;
	if(tmin > _8)
		tmin = _8, index = 8;


	static float gFactor = 0.3f;

	switch(index) {
		case 0 : 
			return glm::vec3(0.0f);
		case 1 : {
			float deltaHeight = _0 - _1;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(0.0f, 0.0f, -1.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 2 : {
			float deltaHeight = _0 - _2;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(1.0f, 0.0f, -1.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 3 : {
			float deltaHeight = _0 - _3;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(1.0f, 0.0f, 0.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 4 : {
			float deltaHeight = _0 - _4;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 5 : {
			float deltaHeight = _0 - _5;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 6 : {
			float deltaHeight = _0 - _6;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(-1.0f, 0.0f, 1.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 7 : {
			float deltaHeight = _0 - _7;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
		case 8 : {
			float deltaHeight = _0 - _8;
			// cout << "deltaHeight : " << deltaHeight << endl;
			if(deltaHeight < gFactor) return glm::vec3(0.0f);
			return glm::normalize(glm::vec3(-1.0f, 0.0f, -1.0f) + deltaHeight * glm::vec3(0.0f, -1.0f, 0.0f));
		} 
	}
}

float Terrain::getHeight(const int x, const int z) const 
{
	if(x < 0 || x >= _size || z < 0 || z >= _size) return 0;
	return _heightMap[z][x];
}

// glm::vec3 Terrain::getGradient(const float x, const float z) const
// {

// }


float Terrain::getHeight(const float x, const float z) const
{
	int x1 = int(x);
	int x2 = int(x + 0.5f);
	int z1 = int(z);
	int z2 = int(z + 0.5f);

	float y1 = getHeight(x1, z1);
	float y2 = getHeight(x2, z1);
	float y3 = getHeight(x1, z2);
	float y4 = getHeight(x2, z2);

	float y5 = min(y1, y2);
	float y6 = max(y1, y2);
	float y7 = y5 + (y6 - y5) * ((x - float(x1)) / 1.0f);

	float y8 = min(y3, y4);
	float y9 = max(y3, y4);
	float y10 = y8 + (y9 - y8) * ((x - float(x1)) / 1.0f);

	float y11 = min(y7, y10);
	float y12 = max(y7, y10);
	float y13 = y11 + (y12 - y11) * ((z - float(z1)) / 1.0f);

	return y13;
}