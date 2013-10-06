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

//-------------------------------------------------------------------------------------
BaseApplication::BaseApplication(void)
    : mRoot(0),
    mSceneMgr(0),
    mWindow(0),
    mResourcesCfg(Ogre::StringUtil::BLANK),
    mPluginsCfg(Ogre::StringUtil::BLANK),
    mCursorWasVisible(false),
    mShutDown(false),
    mInputManager(0),
    mMouse(0),
    mKeyboard(0)
{
}

//-------------------------------------------------------------------------------------
BaseApplication::~BaseApplication(void)
{
	delete mController;
    //Remove ourself as a Window listener
    Ogre::WindowEventUtilities::removeWindowEventListener(mWindow, this);
    windowClosed(mWindow);
    delete mRoot;
}

	HWND mWindowHandle ;
	HWND mExternalWindow = (HWND)(0x000707EC);

LRESULT CALLBACK _WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//note can use with OIS but need to pass more than just the mouse down message
	DWORD dwPID;
	bool posted = false;
	DWORD hThread ;
	hThread = GetWindowThreadProcessId(mExternalWindow, &dwPID);  
	if (dwPID != NULL && hThread!= NULL ) 
	{
		SendMessage(mExternalWindow, msg, wParam, lParam);
	}
	return DefWindowProc( hwnd, msg, wParam, lParam );
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
		WNDCLASS wc = { classStyle, _WndProc, 0, 0, hInst,
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

    mKeyboard = static_cast<OIS::Keyboard*>(mInputManager->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputManager->createInputObject( OIS::OISMouse, true ));

    mMouse->setEventCallback(this);
    mKeyboard->setEventCallback(this);

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
	mShaderGenerator->createShaderBasedTechnique("window/base", Ogre::MaterialManager::DEFAULT_SCHEME_NAME, Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	//Ogre::MaterialManager::getSingleton().addListener();
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

	/*SystemTextureLoader *loader = new SystemTextureLoader();

		Ogre::TexturePtr ptr = Ogre::TextureManager::getSingleton().createManual("window",
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TextureType::TEX_TYPE_2D,
			256,
			256,
			1,
			Ogre::PixelFormat::PF_A8R8G8B8,
			Ogre::TU_STATIC_WRITE_ONLY,
			loader);

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("box/singlelight");
	material->getTechnique(0)->getPass(0)->getTextureUnitState(0)->setTexture(ptr);*/
	SystemWindowManager *window = new SystemWindowManager( mSceneMgr);
	window->RefreshWindowHandles();
	window->DisplayWindow();

	mOculus = new OculusControl();
	mController = new CameraController::Controller(mWindow);
    mController->createCameras(mSceneMgr);

	if(mOculus->isInitialised())
	{
		mController->createViewports(mOculus->getDeviceInfo());
	}
	else
	{
		mController->createViewports();
	}
	

	
	mPlayer = new Player(mSceneMgr, mController->mBodyRotationNode);

	mScene = new WarehouseFloor(mSceneMgr);
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
	


    //Need to capture/update each device
    mKeyboard->capture();
    mMouse->capture();
	//mBullet->StepPhysics(evt.timeSinceLastFrame);
	mController->mRotationNode->setOrientation(mOculus->getOrientation());
	mPlayer->processMovement(evt.timeSinceLastFrame);
    return true;
}
//-------------------------------------------------------------------------------------
bool BaseApplication::keyPressed( const OIS::KeyEvent &arg )
{
	/*if(arg.key == OIS::KC_1)
    {
		if(!mWarehouseShown )
		{
			mScene = new WarehouseFloor(mSceneMgr);
			mWarehouseShown = true;
		}
		else
		{
			delete mScene;
			mWarehouseShown = false;
		}
    }*/


	if(arg.key == OIS::KC_A)
    {
		mPlayer->addKeyboardInput(Ogre::Vector3(-1,0,0));
    }
	else if(arg.key == OIS::KC_D)
    {
        mPlayer->addKeyboardInput(Ogre::Vector3(1,0,0));
    }
	else if(arg.key == OIS::KC_S)
    {
        mPlayer->addKeyboardInput(Ogre::Vector3(0,0,1));
    }
	else if(arg.key == OIS::KC_W)
    {
        mPlayer->addKeyboardInput(Ogre::Vector3(0,0,-1));
    }
    else if(arg.key == OIS::KC_F5)   // refresh all textures
    {
        Ogre::TextureManager::getSingleton().reloadAll();
    }
	else if(arg.key == OIS::KC_SPACE)
    {
        mPlayer->jump();
    }
    else if (arg.key == OIS::KC_SYSRQ)   // take a screenshot
    {
        mWindow->writeContentsToTimestampedFile("screenshot", ".jpg");
    }
    else if (arg.key == OIS::KC_ESCAPE)
    {
        mShutDown = true;
    }
	else if(arg.key == OIS::KC_LCONTROL)
    {
        mPlayer->changeHeight(true);
    }
    return true;
}

bool BaseApplication::keyReleased( const OIS::KeyEvent &arg )
{
	if(arg.key == OIS::KC_A)
    {
		mPlayer->addKeyboardInput(Ogre::Vector3(1,0,0));
    }
	else if(arg.key == OIS::KC_D)
    {
        mPlayer->addKeyboardInput(Ogre::Vector3(-1,0,0));
    }
	else if(arg.key == OIS::KC_S)
    {
        mPlayer->addKeyboardInput(Ogre::Vector3(0,0,-1));
    }
	else if(arg.key == OIS::KC_W)
    {
        mPlayer->addKeyboardInput(Ogre::Vector3(0,0,1));
    }
	else if(arg.key == OIS::KC_LCONTROL)
    {
        mPlayer->changeHeight(false);
    }
	
    return true;
}

bool BaseApplication::mouseMoved( const OIS::MouseEvent &arg )
{
	mPlayer->mouseInput(Ogre::Vector2(arg.state.X.rel, arg.state.Y.rel));
    return true;
}

bool BaseApplication::mousePressed( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
	mPlayer->step();
    return true;
}

bool BaseApplication::mouseReleased( const OIS::MouseEvent &arg, OIS::MouseButtonID id )
{
    return true;
}

//Adjust mouse clipping area
void BaseApplication::windowResized(Ogre::RenderWindow* rw)
{
    unsigned int width, height, depth;
    int left, top;
    rw->getMetrics(width, height, depth, left, top);

    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
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
