#include "stdafx.h"
#include "SystemWindowManager.h"

using namespace Ogre;

SystemWindowManager::SystemWindowManager(SceneManager *sceneManager,
										RTShader::ShaderGenerator *shaderGenerator,
										CameraController *cameraController)
	:m_Windows(),
	m_SceneManager(sceneManager),
	m_ShaderGenerator(shaderGenerator),
	m_Controller(cameraController),
	m_ThumbnaislActive(false),
	m_CollisionTools(sceneManager),
	m_HighlightedNode(NULL)
{
	
    Plane p;
    p.normal = Vector3(0,0,1); p.d = 0;
    MeshManager::getSingleton().createPlane(
        "windowPreview", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p, .2, .2, 1, 1, true, 1, 1, 1, Vector3::UNIT_Y);

    p.normal = Ogre::Vector3(0,0,1); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "window", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
        p, 1, 1, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
}


SystemWindowManager::~SystemWindowManager(void)
{
	for (std::vector<SystemWindow*>::iterator it = m_Windows.begin(); it != m_Windows.end(); ++it)
	{
		delete *it;
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

	m_SelectedWindow = m_Windows.at(0);
	m_SelectedWindow->DisplayWindow();
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
	Entity *ent;
	int count =0;
	for (std::vector<SystemWindow*>::iterator it = m_Windows.begin(); it != m_Windows.end(); ++it)
	{
		String name =StringConverter::toString(count);
		ent = m_SceneManager->createEntity(name, "windowPreview");

		MaterialPtr material = MaterialManager::getSingleton().getByName((*it)->GetMaterialName());
		ent->setMaterial(material);
		// we rotate this guy so we don't need to figure out anything fancy to position the window
		SceneNode* rotationNode = m_ThumbnailNode->createChildSceneNode(name);
		rotationNode->setPosition(Vector3(0,2,0));
		SceneNode* positionNode = rotationNode->createChildSceneNode("position" + name);
		positionNode->attachObject(ent);
		positionNode->setPosition(Vector3(0,0,-.6));

		//create rows of thumbnails 5 wide and a new row every 5 windows
		rotationNode->yaw(Radian(Degree(40.0f - ((float)(count % 5) * 20.0f))));
		rotationNode->pitch((Radian(Degree(10.0f -((float)(count /5) * 20.0f)))));
		count++;
	}
}

void SystemWindowManager::RemoveThumbnails()
{
	if(!m_ThumbnaislActive)
	{
		return;
	}

	if(m_HighlightedNode)
	{
		int windowIndex = StringConverter::parseInt(m_HighlightedNode->getName());
		m_SelectedWindow = m_Windows.at(windowIndex);
		m_SelectedWindow->DisplayWindow();
	}
	RemoveHighlightedThumbnail();
	m_ThumbnaislActive = false;

	// clean up all the scene nodes.
	SceneManagerExtensions::DestroyAllAttachedMovableObjects(m_ThumbnailNode);
	m_ThumbnailNode->removeAndDestroyAllChildren();
	m_SceneManager->destroySceneNode(m_ThumbnailNode);
}

void SystemWindowManager::MoveSelected()
{
	m_SelectedWindow->AttachTo(m_Controller->mRotationNode);
}
void SystemWindowManager::ReleaseSelected()
{
	m_SelectedWindow->DetatchFrom(m_Controller->mRotationNode);
}

void SystemWindowManager::ScaleSelected(float scale)
{
	if(scale > 0)
	{
		if(m_ZoomMode)
		{
			m_SelectedWindow->ScaleUp();
		}
		else
		{
			m_SelectedWindow->MoveFurtherFromCamera();
		}
			
	}
	else if(scale < 0)
	{
		if(m_ZoomMode)
		{
			m_SelectedWindow->ScaleDown();
		}
		else
		{
			m_SelectedWindow->MoveCloserToCamera();
		}
	}
}


void SystemWindowManager::SetZoomActive(bool zoomAcitve)
{
	m_ZoomMode = zoomAcitve;
}

void SystemWindowManager::OnMouseMoved(Vector3 mouseMovement)
{

	ScaleSelected(mouseMovement.z );
	
	if(mouseMovement.x != 0.0f || mouseMovement.y != 0.0f)
	{
		if(m_ThumbnaislActive)
		{
			CheckActiveThumbnail();
		}
	}
}

void SystemWindowManager::CheckActiveThumbnail()
{
	Vector3 origin = m_Controller->mRotationNode->convertLocalToWorldPosition(Vector3::ZERO);
	Quaternion normal = m_Controller->mRotationNode->convertLocalToWorldOrientation(Quaternion::IDENTITY);
	//convert to a vector 3 going into the screen
	Vector3 other = normal * Vector3::NEGATIVE_UNIT_Z;

	Vector3 result;
	Entity* thumbnail = NULL;
	float distToColl = -1.0f;

	m_CollisionTools.raycastFromPoint(origin, other, result, thumbnail, distToColl);

 	if(thumbnail)
	{
		ChangeHighlightedThumbnail(thumbnail->getName());
	}
	else
	{
		RemoveHighlightedThumbnail();
	}
}


void SystemWindowManager::ChangeHighlightedThumbnail( String name)
{
	if(StringConverter::isNumber(name))
	{
		//int windowIndex = StringConverter::parseInt(name);
		//m_Windows.at(windowIndex);

		//come up with a better way to detect this
		SceneNode* child =static_cast<SceneNode*>(m_ThumbnailNode->getChild(name));

		if(m_HighlightedNode)
		{
			if(m_HighlightedNode->getName() != child->getName())
			{
				// don't set z it will move the window
				m_HighlightedNode->setScale(Vector3(1.0f,1.0f,1.0f));
				child->setScale(Vector3(1.1f,1.1f,1.0f));
				m_HighlightedNode = child;
			}
		}
		else
		{
			child->setScale(Vector3(1.1f,1.1f,1.0f));
			m_HighlightedNode = child;
		}
	}
}

void SystemWindowManager::RemoveHighlightedThumbnail()
{
	if(m_HighlightedNode)
	{
		m_HighlightedNode->setScale(Vector3(1.0f,1.0f,1.0f));
		m_HighlightedNode = NULL;
	}
}