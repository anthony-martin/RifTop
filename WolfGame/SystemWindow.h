#pragma once
#include "SystemTextureLoader.h"

class SystemWindow
{
private:
	HWND m_WindowHandle;
	SystemTextureLoader *m_TextureLoader;
	Ogre::String m_Title;

public:
	SystemWindow(HWND window);
	~SystemWindow(void);
};

