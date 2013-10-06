#pragma once
#include "SystemWindow.h"

class SystemWindowManager
{
private: 
	std::vector<SystemWindow> m_Windows;
	Ogre::SceneManager *m_SceneManager;

public:
	SystemWindowManager(Ogre::SceneManager *sceneManager);
	~SystemWindowManager(void);


	void RefreshWindowHandles();

	void DisplayWindow();
};

