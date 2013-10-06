#pragma once
#include "stdafx.h"
#include "SystemTextureLoader.h"

class SystemWindow
{
private:
	HWND m_WindowHandle;
	SystemTextureLoader *m_TextureLoader;
	Ogre::String m_Title;
	Ogre::SceneManager *m_SceneManager;
	Ogre::BillboardSet* m_WindowSet;
	Ogre::SceneNode* m_SceneNode;

public:
	SystemWindow(HWND window, Ogre::SceneManager *sceneManager);
	~SystemWindow(void);
};

