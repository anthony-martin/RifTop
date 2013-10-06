#include "stdafx.h"
#include "SystemWindowManager.h"


SystemWindowManager::SystemWindowManager(Ogre::SceneManager *sceneManager)
	:m_Windows(),
	m_SceneManager(sceneManager)
{
}


SystemWindowManager::~SystemWindowManager(void)
{
}


void SystemWindowManager::RefreshWindowHandles()
{
	HANDLE texHandle = NULL;
    DWORD	d3dFormat;
	ID3D11Texture2D *pTexture = NULL;

	HWND win = GetWindow(GetDesktopWindow(), GW_CHILD);
	do
	{
		if(IsWindowVisible(win))
        {
            RECT clientRect;
            GetClientRect(win, &clientRect);

            HWND hwndParent = GetParent(win);

            DWORD exStyles = (DWORD)GetWindowLongPtr(win, GWL_EXSTYLE);
            DWORD styles = (DWORD)GetWindowLongPtr(win, GWL_STYLE);

            if( (exStyles & WS_EX_TOOLWINDOW) == 0 && (styles & WS_CHILD) == 0 /*&& hwndParent == NULL*/)
            {
                //-------

                DWORD processID;
                GetWindowThreadProcessId(win, &processID);
                if(processID == GetCurrentProcessId())
                    continue;

				//todo see if this actually works
				m_Windows.push_back(SystemWindow(win, m_SceneManager));
			}
		}
		
	}
	while(win = GetWindow(win, GW_HWNDNEXT));
}

void SystemWindowManager::DisplayWindow()
{
	//temp testing to see if windows load properly
	Ogre::BillboardSet*	m_WindowSet =  m_SceneManager->createBillboardSet("windows");
	m_WindowSet->setDefaultDimensions(1, 1);
	m_WindowSet->setMaterialName("window/base");

	Billboard* myBillboard = m_WindowSet->createBillboard(Ogre::Vector3(0, 0, 0));
	Ogre::SceneNode* m_SceneNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("windows");
	m_SceneNode->attachObject(m_WindowSet);

	m_SceneNode->setPosition(Ogre::Vector3(0,1.5,-.5));
}
//todo expose window list

//first window

