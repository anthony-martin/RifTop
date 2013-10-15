#pragma once
#include "SystemWindow.h"
#include "SceneManagerExtensions.h"
#include "CameraController.h"

class SystemWindowManager
{
private: 
	std::vector<SystemWindow*> m_Windows;
	Ogre::SceneManager *m_SceneManager;
	Ogre::RTShader::ShaderGenerator *m_ShaderGenerator;
	CameraController* m_Controller;

	bool m_ThumbnaislActive;
	Ogre::SceneNode* m_ThumbnailNode;

public:
	SystemWindowManager(Ogre::SceneManager *sceneManager,
						Ogre::RTShader::ShaderGenerator *shaderGenerator,
						CameraController *cameraController);
	~SystemWindowManager(void);


	void RefreshWindowHandles();

	void ShowThumbnails();
	void RemoveThumbnails();

	void MoveSelected();
	void ReleaseSelected();
};

