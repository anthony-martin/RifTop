#pragma once
#include "SystemWindow.h"

class SystemWindowManager
{
private: 
	std::vector<SystemWindow*> m_Windows;
	Ogre::SceneManager *m_SceneManager;
	Ogre::RTShader::ShaderGenerator *m_ShaderGenerator;

public:
	SystemWindowManager(Ogre::SceneManager *sceneManager,
						Ogre::RTShader::ShaderGenerator *shaderGenerator);
	~SystemWindowManager(void);


	void RefreshWindowHandles();
};

