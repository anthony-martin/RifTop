#include "stdafx.h"
#include "SystemWindow.h"


SystemWindow::SystemWindow(HWND window)
	:m_WindowHandle(window)
{
	m_TextureLoader = new SystemTextureLoader(window);
	int length = GetWindowTextLength(m_WindowHandle);
    LPSTR strWindowName  = new CHAR[length];
    
    GetWindowText(m_WindowHandle, strWindowName, length+1);

	m_Title = Ogre::String(strWindowName);
	delete strWindowName;

	Ogre::TexturePtr ptr = Ogre::TextureManager::getSingleton().createManual("window" +(uint)m_WindowHandle,
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	Ogre::TextureType::TEX_TYPE_2D,
	256,
	256,
	1,
	Ogre::PixelFormat::PF_A8R8G8B8,
	Ogre::TU_STATIC_WRITE_ONLY,
	m_TextureLoader);

	//todo billboard creation here
}


SystemWindow::~SystemWindow(void)
{
	Ogre::TextureManager::getSingleton().remove("window" +(uint)m_WindowHandle);
	delete m_TextureLoader;
}
