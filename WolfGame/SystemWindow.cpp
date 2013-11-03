#include "stdafx.h"
#include "SystemWindow.h"


SystemWindow::SystemWindow(HWND window,
							Ogre::SceneManager *sceneManager,
							Ogre::RTShader::ShaderGenerator *shaderGenerator)
	:m_WindowHandle(window),
	m_SceneManager(sceneManager),
	m_ShaderGenerator(shaderGenerator),
	m_WindowVisible(false)
{
	m_TextureLoader = new SystemTextureLoader(window);
	int length = GetWindowTextLength(m_WindowHandle);
    LPSTR strWindowName  = new CHAR[length];
    
    GetWindowText(m_WindowHandle, strWindowName, length+1);

	m_Title = Ogre::String(strWindowName);

	m_TextureName = "window" +Ogre::StringConverter::toString( (int)m_WindowHandle);
	m_MaterialName = "window/base" + Ogre::StringConverter::toString( (uint)m_WindowHandle);

	Ogre::TexturePtr ptr = Ogre::TextureManager::getSingleton().createManual(m_TextureName,
	Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
	TEX_TYPE_2D,
	256,
	256,
	1,
	PF_A8R8G8B8,
	TU_STATIC_WRITE_ONLY,
	m_TextureLoader);


	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("window/base",Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	//clone the material then add our own texture to it
	//clone does not seem to clone the shaders generated by the RTSS
	Ogre::MaterialPtr windowMaterial = material->clone(m_MaterialName);
	windowMaterial->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(ptr);
	windowMaterial->load();

	m_ShaderGenerator->createShaderBasedTechnique(m_MaterialName, Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
}


SystemWindow::~SystemWindow(void)
{
	Ogre::TextureManager::getSingleton().remove(m_TextureName);
	Ogre::MaterialManager::getSingleton().remove(m_MaterialName);

	if(!m_WindowVisible)
	{
		return;
	}
	SceneManagerExtensions::DestroyAllAttachedMovableObjects(m_PositionNode);
	m_PositionNode->removeAndDestroyAllChildren();
	m_SceneManager->destroySceneNode(m_PositionNode);

	//done in this order in case 
	SceneManagerExtensions::DestroyAllAttachedMovableObjects(m_RotationNode);
	m_RotationNode->removeAndDestroyAllChildren();
	m_SceneManager->destroySceneNode(m_RotationNode);
}


void SystemWindow::DisplayWindow()
{
	if(m_WindowVisible)
	{
		return;
	}
	
	//using geometry instead of billboards as the rotation towards the camera is not so cool
	// will look into instanced geometry later

	Ogre::Entity *ent;
    ent = m_SceneManager->createEntity(m_MaterialName, "window");

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(m_MaterialName);
	ent->setMaterial(material);
	m_RotationNode = m_SceneManager->getRootSceneNode()->createChildSceneNode(m_MaterialName);
	m_RotationNode->setPosition(Ogre::Vector3(0,1.75,0));
	m_PositionNode = m_RotationNode->createChildSceneNode("position" + m_MaterialName);
	m_PositionNode->attachObject(ent);
	m_PositionNode->setPosition(Ogre::Vector3(0,0,-2));

	RECT rect;
	bool sucess = GetClientRect(m_WindowHandle, &rect);
	DWORD error;
	if(sucess)
	{
		float width = rect.right - rect.left;
		float height = rect.top - rect.bottom;

		float aspectRatio = Math::Abs(width / height);

		m_PositionNode->scale( Vector3(1.0f * aspectRatio, 1.0f, 1.0f));
	}
	else
	{
		error = GetLastError();
	}
	SendMessage(m_WindowHandle , WM_ACTIVATE, WA_ACTIVE, (LPARAM)m_WindowHandle);
	SendMessage(m_WindowHandle , WM_SETFOCUS, 0, 0);
	m_WindowVisible = true;
}

Ogre::String SystemWindow::GetMaterialName()
{
	return m_MaterialName;
}


void SystemWindow::AttachTo(Ogre::SceneNode* parent)
{
	if(!m_WindowVisible)
	{
		return;
	}

	m_RotationNode->removeChild(m_PositionNode);

	parent->addChild(m_PositionNode);
}

void SystemWindow::DetatchFrom(Ogre::SceneNode* parent)
{
	if(!m_WindowVisible)
	{
		return;
	}
	parent->removeChild(m_PositionNode);
	// get the orientation of the camera in the world reference frame rather than relative to it's parent
	Ogre::Quaternion eyeOrientation = parent->convertLocalToWorldOrientation(Ogre::Quaternion::IDENTITY);
	// set this to the world orientation since we have no other parents
	m_RotationNode->addChild(m_PositionNode);
	m_RotationNode->setOrientation(eyeOrientation);
}

void SystemWindow::ScaleUp()
{
	if(!m_WindowVisible)
	{
		return;
	}
	Vector3 scale = m_PositionNode->getScale();
	if(scale.length() < 50.0f)
	{
		scale.x = scale.x*1.1f;
		scale.y = scale.y*1.1f;
	}
	m_PositionNode->setScale(scale);
}

void SystemWindow::ScaleDown()
{
	if(!m_WindowVisible)
	{
		return;
	}
	Vector3 scale = m_PositionNode->getScale();
	if(scale.length() > 0.001f)
	{
		scale.x = scale.x/1.1f;
		scale.y = scale.y/1.1f;
	}
	m_PositionNode->setScale(scale);
}

void SystemWindow::MoveCloserToCamera()
{
	if(!m_WindowVisible)
	{
		return;
	}
	Ogre::Vector3 position = m_PositionNode->getPosition();
	if(position.z < -0.2f)
	{
		// positive Z is out from the screen
		position.z += 0.1f;
	}

	m_PositionNode->setPosition(position);
}
void SystemWindow::MoveFurtherFromCamera()
{
	if(!m_WindowVisible)
	{
		return;
	}

	Ogre::Vector3 position = m_PositionNode->getPosition();
	if(position.z> -40.0f )
	{
		// negative Z is into the screen 
		position.z -= 0.1f;
	}

	m_PositionNode->setPosition(position);
}

void SystemWindow::Message(UINT msg, WPARAM wParam, LPARAM lParam)
{
	//if(msg == 127 ||
	//	msg == 70 ||
	//	msg == 71||
	//	msg == 28||
	//	msg == 134 ||
	//	msg == 6||
	//	msg == 641||
	//	msg == 642||
	//	msg == 7||
	//	msg == 132||
	//	msg == 32||
	//	msg == 8||
	//	msg == 512||
	//	msg == 513||
	//	msg == 799 ||
	//	msg == 15 ||
	//	msg == 49320)

	//	//note message 6 WM_ACTIVATE is the one we need to send to the other window.
	//{
	//	return;
	//}

	LRESULT result = PostMessage(m_WindowHandle , msg, wParam, lParam);

	if(result != S_OK)
	{
		MoveFurtherFromCamera();
		MoveCloserToCamera();
	}
}