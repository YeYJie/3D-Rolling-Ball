#ifndef _MENU_H_
#define _MENU_H_

#include "include.h"
#include "gui.h"
#include "text.h"

class Menu
{

public:

	Menu();

	void bindMenuFrameBuffer() const;

	void unbindMenuFrameBuffer() const;

	GLuint getBgTexture() const;

	void render();

private:

	void initMenuList();

private:

	static const int MENUFRAMEBUFFERWIDTH = 1024;
	static const int MENUFRAMEBUFFERHEIGHT = 1024;

	GLuint _menuFBO;

	GLuint _bgColor;
	GLuint _bgDepth;

	GUIPtr _bg;
	GUIRenderer _bgRenderer;

	vector<TextPtr> _menuTexts;
	TextRenderer _menuTextRenderer;

}; // class Menu

#endif