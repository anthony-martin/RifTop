#include "stdafx.h"
#include "SystemWindow.h"


SystemWindow::SystemWindow(HWND window, Ogre::SceneManager *sceneManager)
	:m_WindowHandle(window),
	m_SceneManager(sceneManager)
{
	m_TextureLoader = new SystemTextureLoader(window);
	int length = GetWindowTextLength(m_WindowHandle);
    LPSTR strWindowName  = new CHAR[length];
    
    GetWindowText(m_WindowHandle, strWindowName, length+1);

	m_Title = Ogre::String(strWindowName);

	Ogre::TexturePtr ptr = Ogre::TextureManager::getSingleton().createManual("window" ,
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	TEX_TYPE_2D,
	256,
	256,
	1,
	PF_A8R8G8B8,
	TU_STATIC_WRITE_ONLY,
	m_TextureLoader);


	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("window/base");
	material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(ptr);


	//todo billboard creation here
}


SystemWindow::~SystemWindow(void)
{
	Ogre::TextureManager::getSingleton().remove("window" );
}
