#ifndef _MENU_H_
#define _MENU_H_

#include "include.h"
#include "gui.h"
#include "text.h"
#include "blur.h"

class Menu
{

public:

	Menu();

	void bindMenuFrameBuffer() const;

	void unbindMenuFrameBuffer() const;

	GLuint getBgTextureRaw() const;

	void render();

private:

	void initMenuList();

	void resetMenuPosition();


private:

	GLuint _menuFBO;

	GLuint _bgColor;
	GLuint _bgDepth;

	GUIPtr _bg;
	GUIRenderer _bgRenderer;

	GaussBlur _blurrer;

	vector<TextPtr> _menuTexts;
	TextRenderer _menuTextRenderer;

}; // class Menu

#endif