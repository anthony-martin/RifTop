#include "stdafx.h"
#include "SystemWindowManager.h"


SystemWindowManager::SystemWindowManager(Ogre::SceneManager *sceneManager,
										Ogre::RTShader::ShaderGenerator *shaderGenerator)
	:m_Windows(),
	m_SceneManager(sceneManager),
	m_ShaderGenerator(shaderGenerator)
{
}


SystemWindowManager::~SystemWindowManager(void)
{
	for (std::vector<SystemWindow*>::iterator it = m_Windows.begin(); it != m_Windows.end(); ++it)
	{
		delete &it;
	}
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
				
				m_Windows.push_back(new SystemWindow(win, m_SceneManager, m_ShaderGenerator));
			}
		}
		
	}
	while(win = GetWindow(win, GW_HWNDNEXT));

	m_Windows.at(0)->DisplayWindow();
}


//todo expose window list

//first window

