#include "stdafx.h"
#include "SystemWindow.h"


SystemWindow::SystemWindow(HWND window, Ogre::SceneManager *sceneManager)
	:m_WindowHandle(window),
	m_TextureLoader(window),
	m_SceneManager(sceneManager)
{
	int length = GetWindowTextLength(m_WindowHandle);
    LPSTR strWindowName  = new CHAR[length];
    
    GetWindowText(m_WindowHandle, strWindowName, length+1);

	m_Title = Ogre::String(strWindowName);
	//delete strWindowName;

	Ogre::TexturePtr ptr = Ogre::TextureManager::getSingleton().createManual("window" + (uint)m_WindowHandle ,
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	TEX_TYPE_2D,
	256,
	256,
	1,
	PF_A8R8G8B8,
	TU_STATIC_WRITE_ONLY,
	&m_TextureLoader);


	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("window/base");
	material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(ptr);

	m_WindowSet =  m_SceneManager->createBillboardSet("windows");
	m_WindowSet->setDefaultDimensions(10, 10);
	m_WindowSet->setMaterial(material);
	Billboard* myBillboard = m_WindowSet->createBillboard(Ogre::Vector3(0, 0, 0));
	m_SceneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("windows");
	m_SceneNode->attachObject(m_WindowSet);

	m_SceneNode->setPosition(Ogre::Vector3(5,.1,18));
	//todo billboard creation here
}


SystemWindow::~SystemWindow(void)
{
	Ogre::TextureManager::getSingleton().remove("window" +(uint)m_WindowHandle);
}
