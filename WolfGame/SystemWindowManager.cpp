#include "stdafx.h"
#include "SystemWindowManager.h"


SystemWindowManager::SystemWindowManager(Ogre::SceneManager *sceneManager,
										Ogre::RTShader::ShaderGenerator *shaderGenerator)
	:m_Windows(),
	m_SceneManager(sceneManager),
	m_ShaderGenerator(shaderGenerator),
	m_ThumbnaislActive(false)
{
	
    Ogre::Plane p;
    p.normal = Ogre::Vector3(0,0,1); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "windowPreview", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p, .2, .2, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
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

	//m_Windows.at(0)->DisplayWindow();

}

void SystemWindowManager::ShowThumbnails()
{
	//simple way of checking for double calls
	if(m_ThumbnaislActive)
	{
		return;
	}
	m_ThumbnaislActive = true;

	// this provides a single node from which we can cleanup the rest.
	m_ThumbnailNode = m_SceneManager->getRootSceneNode()->createChildSceneNode("thumbnails");
	Ogre::Entity *ent;
	int count =0;
	for (std::vector<SystemWindow*>::iterator it = m_Windows.begin(); it != m_Windows.end(); ++it)
	{
		Ogre::String name =Ogre::StringConverter::toString(count);
		ent = m_SceneManager->createEntity(name, "windowPreview");

		Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName((*it)->GetMaterialName());
		ent->setMaterial(material);
		// we rotate this guy so we don't need to figure out anything fancy to position the window
		Ogre::SceneNode* rotationNode = m_ThumbnailNode->createChildSceneNode("rotation" + name);
		rotationNode->setPosition(Ogre::Vector3(0,2,0));
		Ogre::SceneNode* positionNode = rotationNode->createChildSceneNode("position" + name);
		positionNode->attachObject(ent);
		positionNode->setPosition(Ogre::Vector3(0,0,-.6));

		//create rows of thumbnails 5 wide and a new row every 5 windows
		rotationNode->yaw(Ogre::Radian(Ogre::Degree(40 - ((float)(count % 5) * 20.0))));
		rotationNode->pitch((Ogre::Radian(Ogre::Degree(10 -((float)(count /5) * 20.0)))));
		count++;
	}
}

void SystemWindowManager::RemoveThumbnails()
{
	if(!m_ThumbnaislActive)
	{
		return;
	}
	m_ThumbnaislActive = false;

	// clean up all the scene nodes.
	SceneManagerExtensions::DestroyAllAttachedMovableObjects(m_ThumbnailNode);
	m_ThumbnailNode->removeAndDestroyAllChildren();
	m_SceneManager->destroySceneNode(m_ThumbnailNode);
}
//todo expose window list

//first window

