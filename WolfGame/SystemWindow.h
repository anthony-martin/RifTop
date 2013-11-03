#pragma once
#include "stdafx.h"
#include "SystemTextureLoader.h"
#include "SceneManagerExtensions.h"

class SystemWindow
{
private:

	Ogre::RTShader::ShaderGenerator *m_ShaderGenerator;
	Ogre::SceneManager *m_SceneManager;

	HWND m_WindowHandle;
	SystemTextureLoader *m_TextureLoader;
	Ogre::SceneNode* m_RotationNode;
	Ogre::SceneNode* m_PositionNode;

	Ogre::String m_Title;
	Ogre::String m_MaterialName;
	Ogre::String m_TextureName;

	bool m_WindowVisible;

public:
	SystemWindow(HWND window, 
				Ogre::SceneManager *sceneManager, 
				Ogre::RTShader::ShaderGenerator *shaderGenerator);
	~SystemWindow(void);

	void DisplayWindow();
	Ogre::String  GetMaterialName();

	//Attach the window to a new parent so it inherits their orenintation
	void AttachTo(Ogre::SceneNode* parent);
	//Detach the window from the parent and fix the orientation in world space
	void DetatchFrom(Ogre::SceneNode* parent);

	void ScaleUp();
	void ScaleDown();
	
	void MoveCloserToCamera();
	void MoveFurtherFromCamera();
	void Message(UINT msg, WPARAM wParam, LPARAM lParam);
};

