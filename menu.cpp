#include "menu.h"

extern const int WIDTH;
extern const int HEIGHT;

Menu::Menu()
	: _menuTextRenderer("arial.fnt", "arial.png", TEXT_SDF()),
	  _bgRenderer("menu.vs", "menu.fs")
{
	// init FBO
	glGenFramebuffers(1, &_menuFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, _menuFBO);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	// color buffer
	glGenTextures(1, &_bgColor);
	glBindTexture(GL_TEXTURE_2D, _bgColor);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 
					MENUFRAMEBUFFERWIDTH, MENUFRAMEBUFFERHEIGHT,
					0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
							GL_TEXTURE_2D, _bgColor, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	// depth buffer
	glGenRenderbuffers(1, &_bgDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, _bgDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
							MENUFRAMEBUFFERWIDTH, MENUFRAMEBUFFERHEIGHT);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
	                          GL_RENDERBUFFER, _bgDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// others
	_bg.setTexture(Texture(_bgColor));
	_bg.setPositionAndSize(0, 0, WIDTH, HEIGHT);

	initMenuList();
}

void Menu::initMenuList()
{
	_menuTexts.push_back(new Text(L"Rolling Ball",
								200, 200, 1.0, 1.0));
	_menuTexts.push_back(new Text(L"Level 1", 250, 300, 1.0, 1.0));
	_menuTexts.push_back(new Text(L"Level 2", 250, 400, 1.0, 1.0));
	_menuTexts.push_back(new Text(L"Level 3", 250, 500, 1.0, 1.0));
}

void Menu::bindMenuFrameBuffer() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, _menuFBO);
	glViewport(0, 0, MENUFRAMEBUFFERWIDTH, MENUFRAMEBUFFERHEIGHT);	
}

void Menu::unbindMenuFrameBuffer() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, WIDTH, HEIGHT);
}

GLuint Menu::getBgTexture() const
{
	return _bgColor;
}

extern int mouseX;
extern int mouseY;

void Menu::render()
{
	_menuTexts[1]->setScale(1, 1);
	_menuTexts[2]->setScale(1, 1);
	_menuTexts[3]->setScale(1, 1);
	if(mouseX >= 200 && mouseX <= 800) {
		if(mouseY >= 300 && mouseY < 400)
			_menuTexts[1]->setScale(1.05, 1.05);
		else if(mouseY >= 400 && mouseY < 500)
			_menuTexts[2]->setScale(1.05, 1.05);
		else if(mouseY >= 500 && mouseY < 600)
			_menuTexts[3]->setScale(1.05, 1.05);
	}
	_bgRenderer.render(_bg);
	_menuTextRenderer.render(_menuTexts);
}