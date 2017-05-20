#include "textRenderer.h"

TextRenderer::TextRenderer(const char * ttf)
{
	_shader = new Shader("text.vs", "text.fs");
	initGL();

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

        Char temp = {Texture(texture),
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            (GLuint)face->glyph->advance.x};
        _charMap.insert(pair<char, Char>(c, temp));
    }

    FT_Done_Face(face);
    FT_Done_FreeType(ft);
}

void TextRenderer::initGL()
{
	static vector<float> vertices = {-1, 1, -1, -1, 1, 1, 1, -1};

	glGenVertexArrays(1, &_VAO);
	glBindVertexArray(_VAO);

	GLuint VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), 
									vertices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void TextRenderer::render(const vector<Text*> texts)
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
		_shader->setUniform3f("textColor", i->getColor());

		// calculate offset
		float totalAdvance = 0.0f;
		if(_charMap[content[0]].bearing.x < 0)
			totalAdvance += _charMap[content[0]].bearing.x * scale.x * (-1.0);

		float originY = position.y + _charMap[content[0]].bearing.y * scale.y;

		for(int j = 0; j < content.size(); ++j) 
		{
			char c = content[j];
			Char currentChar = _charMap[c];

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