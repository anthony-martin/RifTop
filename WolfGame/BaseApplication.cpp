/*
-----------------------------------------------------------------------------
Filename:    BaseApplication.cpp
-----------------------------------------------------------------------------

This source file is part of the
   ___                 __    __ _ _    _ 
  /___\__ _ _ __ ___  / / /\ \ (_) | _(_)
 //  // _` | '__/ _ \ \ \/  \/ / | |/ / |
/ \_// (_| | | |  __/  \  /\  /| |   <| |
\___/ \__, |_|  \___|   \/  \/ |_|_|\_\_|
      |___/                              
      Tutorial Framework
      http://www.ogre3d.org/tikiwiki/
-----------------------------------------------------------------------------
*/
#include "stdafx.h"
#include "BaseApplication.h"

static bool mShutDown(false);

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mCursorWasVisible(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0),
	m_MoveWindow(true),
	m_cursorPos(NULL)
{
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	delete mController;
	delete m_Windows;
	delete m_WindowInput;

    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

	HWND mWindowHandle ;

LRESULT BaseApplication::Handle(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//note can use with OIS but need to pass more than just the mouse down message

	switch (msg) 
    { 
	case WM_KEYDOWN:
		{
			if(wParam == VK_ESCAPE)
			{
				mShutDown = true;
			}
			break;
		}
	//case WM_MOUSEMOVE:
	//	{
	//		if(m_cursorPos)
	//		{
	//			//also add method for this 
	//			int xPos = GET_X_LPARAM(lParam);
	//			float movement = 0.0f;

	//			movement = m_cursorPos - xPos -3;// magic number that makes the border align

	//			if(movement != 0.0f)
	//			{
	//				Ogre::Radian turn((movement / -200.0f));
	//				mController->mBodyRotationNode->yaw(turn);
	//			}

	//			return 1;
	//		}
	//	}
		default:
			return 0;
	}


	return 0;
}

//-------------------------------------------------------------------------------------
bool BaseApplication::configure(void)
{
    // Show the configuration dialog and initialise the system
    // You can skip this and use root.restoreConfig() to load configuration
    // settings if you were sure there are valid ones saved in ogre.cfg
    if(mRoot->showConfigDialog())
    {
		// If returned true, user clicked OK so initialise
			// Here we choose to let the system create a default rendering window by passing 'true'
			//mWindow = mRoot->initialise(true, "Riftop");
		mRoot->initialise(false);
		UINT classStyle = 0;

		HINSTANCE hInst = NULL;

		// Register the window class
		// NB allow 4 bytes of window data for D3D11RenderWindow pointer
		WNDCLASS wc = { classStyle, MessagePump::_WndProc, 0, 0, hInst,
			LoadIcon(0, IDI_APPLICATION), LoadCursor(NULL, IDC_ARROW),
			(HBRUSH)GetStockObject(BLACK_BRUSH), 0, "rwnd" };	

		RegisterClass(&wc);
		
		DWORD dwStyle =  WS_VISIBLE | WS_CLIPCHILDREN;

		// Create our main window
		// Pass pointer to self
		mWindowHandle = CreateWindowA("rwnd", "Riftop", dwStyle,
			0, 0, 1280, 800, NULL, 0, hInst, this);
		

		NameValuePairList misc;
		misc["externalWindowHandle"] = StringConverter::toString((int)mWindowHandle);
		//misc["fullScreen"] = true;
		mWindow = mRoot->createRenderWindow("Main RenderWindow", 1280, 800, false, &misc);
		//Ogre::WindowEventUtilities::_addRenderWindow(mWindow);
		RECT rect;
		if(GetClientRect(mWindowHandle, &rect))
		{
			ClipCursor(&rect);
		}
		ShowCursor(true);

        return true;
    }
    else
    {
        return false;
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::chooseSceneManager(void)
{
    // Get the SceneManager, in this case a generic one
    mSceneMgr = mRoot->createSceneManager(Ogre::ST_GENERIC);
}
//-------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------
void BaseApplication::createFrameListener(void)
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Initializing OIS ***");
    OIS::ParamList pl;
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;

    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    windowHndStr << windowHnd;
    pl.insert(std::make_pair(std::string("WINDOW"), windowHndStr.str()));

    mInputManager = OIS::InputManager::createInputSystem( pl );

    //mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
   // mKeyboard->setEventCallback(this);

    //Set initial mouse clipping size
    windowResized(mWindow);

    //Register as a Window listener
    Ogre::WindowEventUtilities::addWindowEventListener(mWindow, this);

    // create a params panel for displaying sample details
    Ogre::StringVector items;
    items.push_back("cam.pX");
    items.push_back("cam.pY");
    items.push_back("cam.pZ");
    items.push_back("");
    items.push_back("cam.oW");
    items.push_back("cam.oX");
    items.push_back("cam.oY");
    items.push_back("cam.oZ");
    items.push_back("");
    items.push_back("Filtering");
    items.push_back("Poly Mode");

    mRoot->addFrameListener(this);
}
//-------------------------------------------------------------------------------------
void BaseApplication::destroyScene(void)
{
}
//-------------------------------------------------------------------------------------
void BaseApplication::createViewports(void)
{
	
}
//-------------------------------------------------------------------------------------
void BaseApplication::setupResources(void)
{
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcesCfg);

    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();

    Ogre::String secName, typeName, archName;
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            Ogre::ResourceGroupManager::getSingleton().addResourceLocation(
                archName, typeName, secName);
        }
    }
}
//-------------------------------------------------------------------------------------
void BaseApplication::createResourceListener(void)
{

}
//-------------------------------------------------------------------------------------
void BaseApplication::loadResources(void)
{
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
    Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	if (Ogre::RTShader::ShaderGenerator::initialize())
	{
		// Grab the shader generator pointer.
		mShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
 
		// Add the shader libs resource location.
		Ogre::ResourceGroupManager::getSingleton().addResourceLocation("D:\\Project\\Ogre\\OgreSource\\Samples\\Media\\RTShaderLib\\Cg", "FileSystem");
 
		// Set shader cache path.
		mShaderGenerator->setShaderCachePath("D:\\Project\\WolfGame\\WolfGame\\Debug\\shaders");		
		//mShaderGenerator->
		// Set the scene manager.
		mShaderGenerator->addSceneManager(mSceneMgr);
 
		// Add a specialized sub-render (per-pixel lighting) state to the default scheme render state
		Ogre::RTShader::RenderState* pMainRenderState = 
			mShaderGenerator->createOrRetrieveRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME).first;
		pMainRenderState->reset();
 
		//mShaderGenerator->addSubRenderStateFactory(new Ogre::RTShader::PerPixelLightingFactory);
		pMainRenderState->addTemplateSubRenderState(
			mShaderGenerator->createSubRenderState(Ogre::RTShader::FFPTexturing::Type));	
 
		//return true;
	}
	
	mShaderGenerator->createShaderBasedTechnique("box/singlelight", Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	
}
//-------------------------------------------------------------------------------------
void BaseApplication::go(void)
{
#ifdef _DEBUG
    mResourcesCfg = "D:\\Project\\WolfGame\\WolfGame\\Debug\\resources_d.cfg";
    mPluginsCfg = "D:\\Project\\WolfGame\\WolfGame\\Debug\\plugins_d.cfg";
#else
    mResourcesCfg = ".\\resources.cfg";
    mPluginsCfg = ".\\plugins.cfg";
#endif

    if (!setup())
        return;

    mRoot->startRendering();

    // clean up
    destroyScene();
}
//-------------------------------------------------------------------------------------
bool BaseApplication::setup(void)
{
    mRoot = new Ogre::Root(mPluginsCfg);

    setupResources();

    bool carryOn = configure();
    if (!carryOn) return false;

    chooseSceneManager();

	// Set default mipmap level (NB some APIs ignore this)
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);

    // Create any resource listeners (for loading screens)
    createResourceListener();
    // Load resources
    loadResources();

	mOculus = new OculusControl();
	mController = new CameraController(mWindow);
    mController->createCameras(mSceneMgr);

	

	if(mOculus->isInitialised())
	{
		mController->createViewports(mOculus->getDeviceInfo());
	}
	else
	{
		mController->createViewports();
	}

	m_MosueCursor = new MouseCursor(mSceneMgr, mController->mBodyRotationNode);

	m_Windows = new SystemWindowManager( mSceneMgr, mShaderGenerator, mController, m_MosueCursor);
	m_Windows->RefreshWindowHandles();

	m_WindowInput = new WindowInputController(m_Windows);

	MessagePump::Subscribe(m_WindowInput);
	MessagePump::Subscribe(this);

	//mPlayer = new Player(mSceneMgr, mController->mBodyRotationNode);

	

	//mScene = new WarehouseFloor(mSceneMgr);
	mWarehouseShown = true;
    createScene();

    createFrameListener();

    return true;
};
//-------------------------------------------------------------------------------------
bool BaseApplication::frameRenderingQueued(const Ogre::FrameEvent& evt)
{
    if(mWindow->isClosed())
        return false;

    if(mShutDown)
        return false;
	/*RECT rect;
	if(GetWindowRect(mWindowHandle, &rect))
	{
		int deltaX = rect.right - rect.left;
		int deltsY = rect.bottom - rect.top;
		m_cursorPos = (deltaX/2);
		SetCursorPos(rect.left + m_cursorPos,rect.top + deltsY/2 );
	}*/

	

    //Need to capture/update each device
    //mKeyboard->capture();
    mMouse->capture();
	mController->mRotationNode->setOrientation(mOculus->getOrientation());
	//mPlayer->processMovement(evt.timeSinceLastFrame);
    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	if(arg.key == OIS::KC_SPACE)
    {
		m_Windows->ShowThumbnails();
    }
	else if (arg.key == OIS::KC_ESCAPE)
	{
		mShutDown = true;
	}
	else if (arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT)
	{
		m_Windows->SetZoomActive(true);
	}
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	if(arg.key == OIS::KC_SPACE)
    {
		m_Windows->RemoveThumbnails();
    }
	else if (arg.key == OIS::KC_LSHIFT || arg.key == OIS::KC_RSHIFT)
	{
		m_Windows->SetZoomActive(false);
	}
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	m_MosueCursor->mouseInput(Ogre::Vector2(arg.state.X.rel, arg.state.Y.rel));
	Vector2 relMousePos;
	if(m_Windows->CheckWindowCollision( false, &relMousePos))
	{
		m_Windows->PostMessageSelected(WM_MOUSEMOVE, 0, relMousePos);
	}
	// turn the body
	//mPlayer->mouseInput(Ogre::Vector2(arg.state.X.rel, arg.state.Y.rel));
	// scale the selected window
	//m_Windows->OnMouseMoved(Ogre::Vector3(arg.state.X.rel, arg.state.Y.rel,arg.state.Z.rel));
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Vector2 relMousePos;
	if(m_Windows->CheckWindowCollision( false, &relMousePos))
	{
		LPARAM MouseActive = WM_LBUTTONDOWN<<16|HTCLIENT;
		if(id == OIS::MouseButtonID::MB_Left)
		{
			m_Windows->SendMessageSelected(WM_SETCURSOR, NULL, false);
			m_Windows->PostMessageSelected(WM_LBUTTONDOWN, MK_LBUTTON, relMousePos);
			m_Windows->SendMessageSelected(WM_NCHITTEST, relMousePos);
		}
	}
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	Vector2 relMousePos;
	if(m_Windows->CheckWindowCollision( false, &relMousePos))
	{
		LPARAM MouseActive = WM_LBUTTONUP<<16|HTCLIENT;
		if(id == OIS::MouseButtonID::MB_Left)
		{
			m_Windows->SendMessageSelected(WM_SETCURSOR, NULL, MouseActive);
			m_Windows->PostMessageSelected(WM_LBUTTONUP, MK_LBUTTON, relMousePos);
			m_Windows->SendMessageSelected(WM_NCHITTEST, relMousePos);
		}
	}
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    //const OIS::MouseState &ms = mMouse->getMouseState();
    //ms.width = width;
   // ms.height = height;
}

//Unattach OIS before window shutdown (very important under Linux)
void BaseApplication::windowClosed(Ogre::RenderWindow* rw)
{
    //Only close for window that created OIS (the main window in these demos)
    if( rw == mWindow )
    {
        if( mInputManager )
        {
            //mInputManager->destroyInputObject( mMouse );
            //mInputManager->destroyInputObject( mKeyboard );

            OIS::InputManager::destroyInputSystem(mInputManager);
            mInputManager = 0;
        }
    }
}
