#pragma once
#include "stdafx.h"
#include "SystemTextureLoader.h"

class SystemWindow
{
private:

	Ogre::RTShader::ShaderGenerator *m_ShaderGenerator;
	Ogre::SceneManager *m_SceneManager;

	HWND m_WindowHandle;
	SystemTextureLoader *m_TextureLoader;
	Ogre::SceneNode* m_SceneNode;

	Ogre::String m_Title;
	Ogre::String m_MaterialName;
	Ogre::String m_TextureName;

public:
	SystemWindow(HWND window, 
				Ogre::SceneManager *sceneManager, 
				Ogre::RTShader::ShaderGenerator *shaderGenerator);
	~SystemWindow(void);

	void DisplayWindow();

	Ogre::Entity* GetAsEntity();
};

