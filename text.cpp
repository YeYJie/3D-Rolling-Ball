#include "text.h"

static map<int, SDFChar> parsefnt(const char * fnt)
{
	string temp(fnt);
	temp = "./res/" + temp;
	const char * _fnt = temp.c_str();

	cout << "loading fnt : " << _fnt << endl;

	map<int, SDFChar> charMap;

	ifstream fntFile(_fnt, ios::in);
	if(fntFile.is_open())
	{
		string line = "";
		// discard the first line
		getline(fntFile, line);
		// the second line, we can get texture image's width and height
		getline(fntFile, line);
		// discard the third and forth line
		getline(fntFile, line);
		getline(fntFile, line);

		while(true) {
			getline(fntFile, line);

			if(line.empty())
				break;

			vector<string> currentLine = split(line, ' ');
			if(currentLine.size() != 11)
				break;

			// string sid = currentLine[1];
			int id = atoi(currentLine[1].c_str() + 3);
			SDFChar currentChar(
					id,
					atoi(currentLine[2].c_str() + 2),
					atoi(currentLine[3].c_str() + 2),
					atoi(currentLine[4].c_str() + 6),
					atoi(currentLine[5].c_str() + 7),
					atoi(currentLine[6].c_str() + 8),
					atoi(currentLine[7].c_str() + 8),
					atoi(currentLine[8].c_str() + 9)
				);

			charMap.insert(pair<int, SDFChar>(id, currentChar));

		}
	}
	return charMap;
}

TextRenderer::TextRenderer(const char * fnt, const char * png, TEXT_SDF)
	: _textAtlas(new Texture(png))
{
	_useSDF = true;
	_shader = new TextShader("textSDF.vs", "textSDF.fs");
	initGL();
	_SDFcharMap = parsefnt(fnt);
}

static map<char, TTFChar> parsettf(const char * ttf)
{
	map<char, TTFChar> TTFcharMap;
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

    FT_Face face;
    if (FT_New_Face(ft, ttf, 0, &face))
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;

    FT_Set_Pixel_Sizes(face, 0, 20);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    for (GLubyte c = 0; c < 128; c++)
    {
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // cout << "init : " << (int)c << endl;
        GLuint texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED, GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        TTFChar temp = {TexturePtr(new Texture(texture)),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x};
        TTFcharMap.insert(pair<char, TTFChar>(c, temp));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    return TTFcharMap;
}

TextRenderer::TextRenderer(const char * ttf, TEXT_TTF)
{
	_useSDF = false;
	_shader = new TextShader("textTTF.vs", "textTTF.fs");
	initGL();
	_TTFcharMap = parsettf(ttf);
}

void TextRenderer::initGL()
{
	static vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	// GLuint VBO;
	glGenBuffers(1, &_positionVBO);
	glBindBuffer(GL_ARRAY_BUFFER, _positionVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
									vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::render(const vector<TextPtr> & texts)
{
	if(_useSDF)
		renderSDF(texts);
	else
		renderTTF(texts);
}

void TextRenderer::renderTTF(const vector<TextPtr> & texts)
{
	_shader->bindGL();

	glBindVertexArray(_VAO);
	glEnableVertexAttribArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);


	for(auto i : texts)
	{
		glm::vec2 position = i->getPosition();
		glm::vec2 scale = i->getScale();
		string content = i->getContent();

		if(content.empty()) continue;

		// set color
		_shader->setTextColor(i->getColor());

		// calculate offset
		float totalAdvance = 0.0f;
		if(_TTFcharMap[content[0]].bearing.x < 0)
			totalAdvance += _TTFcharMap[content[0]].bearing.x * scale.x * (-1.0);

		float originY = position.y + _TTFcharMap[content[0]].bearing.y * scale.y;

		for(int j = 0; j < content.size(); ++j)
		{
			char c = content[j];
			TTFChar currentChar = _TTFcharMap[c];

			GUI temp(currentChar.texture);

			temp.setPositionAndSize(
					position.x + totalAdvance + currentChar.bearing.x * scale.x,
					originY - currentChar.bearing.y * scale.y,
					currentChar.size.x * scale.x,
					currentChar.size.y * scale.y
				);

			temp.bindGL();
			_shader->setModelMatrix(temp.getModelMatrix());
			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			temp.unbindGL();

			totalAdvance += (currentChar.advance >> 6) * scale.x;
		}
	}

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	glBindVertexArray(0);

	_shader->unbindGL();
}

void TextRenderer::renderSDF(const vector<TextPtr> & texts)
{
	_shader->bindGL();

	glBindVertexArray(_VAO);
	glEnableVertexAttribArray(0);
	// glEnableVertexAttribArray(1);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_TEST);

	// glBindBuffer(GL_ARRAY_BUFFER, _textCoordsVBO);

	for(auto i : texts)
	{
		glm::vec2 position = i->getPosition();
		glm::vec2 scale = i->getScale();
		wstring content = i->getContentW();

		if(content.empty()) continue;

		// set color
		_shader->setTextColor(i->getColor());

		// calculate offset
		float totalAdvance = 0.0f;
		// if(_TTFcharMap[content[0]].bearing.x < 0)
		// 	totalAdvance += _TTFcharMap[content[0]].bearing.x * scale.x * (-1.0);

		// float originY = position.y + _SDFcharMap[content[0]].bearing.y * scale.y;
		float originY = position.y  - _SDFcharMap[(int)content[0]].yoffset;

		for(int j = 0; j < content.size(); ++j)
		{
			int c = content[j];
			SDFChar currentChar = _SDFcharMap[c];

			GUI temp(_textAtlas);


			temp.setPositionAndSize(
					position.x + totalAdvance + currentChar.xoffset * scale.x,
					originY + currentChar.yoffset,
					currentChar.width * scale.x,
					currentChar.height * scale.y
				);

			temp.bindGL();
			_shader->setModelMatrix(temp.getModelMatrix());

			float sx = currentChar.width / 1024.0f;
			float sy = currentChar.height / 1024.0f;
			float mx = currentChar.x / 512.0f + sx;
			float my = (512.0f - currentChar.y - currentChar.height) / 512.0f + sy;

			// printf("x : %d y : %d width : %d height : %d sx : %f sy : %f mx : %f my : %f\n",
			// 	currentChar.x, currentChar.y, currentChar.width, currentChar.height,
			// 	sx, sy, mx, my);

			// fuck ! glm matrix is column-major :-)
			glm::mat3 textTranslateMatrix = glm::mat3(
					sx, 0, 0, 0, sy, 0, mx, my, 1
				);
			_shader->setTextTranslateMatrix(textTranslateMatrix);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

			temp.unbindGL();

			totalAdvance += currentChar.xadvance * scale.x * 0.7;
		}
	}

	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	glDisableVertexAttribArray(0);
	// glDisableVertexAttribArray(1);
	glBindVertexArray(0);

	_shader->unbindGL();
}