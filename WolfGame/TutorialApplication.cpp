/*
-----------------------------------------------------------------------------
Filename:    TutorialApplication.cpp
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
#include "TutorialApplication.h"

//-------------------------------------------------------------------------------------
TutorialApplication::TutorialApplication(void)
{
}
//-------------------------------------------------------------------------------------
TutorialApplication::~TutorialApplication(void)
{
}

//-------------------------------------------------------------------------------------
void TutorialApplication::createScene(void)
{
   // Set the scene's ambient light
    mSceneMgr->setAmbientLight(Ogre::ColourValue(0.5f, 0.5f, 0.5f));
 //
     //Create an Entity
	Ogre::Entity *ent;
            Ogre::Plane p;
            p.normal = Ogre::Vector3(0,1,0); p.d = 0;
            Ogre::MeshManager::getSingleton().createPlane(
                "FloorPlane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                p, 200000, 200000, 20, 20, true, 1, 9000, 9000, Ogre::Vector3::UNIT_Z);
            // Create an entity (the floor)
           ent = mSceneMgr->createEntity("floor", "FloorPlane");
 //   Ogre::Entity* ogreHead = mSceneMgr->createEntity("Head", "Plane.mesh");

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("box/singlelight");//,Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	ent->setMaterial(material);

	//Ogre::Entity* bridge = mSceneMgr->createEntity("Bridge", "TestBridge.mesh");
	//bridge->setMaterial(material);

 //   // Create a SceneNode and attach the Entity to it
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("HeadNode");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3::ZERO);
	//headNode->scale(10,10,10);

	//Ogre::SceneNode* BridgeNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("bridge");
 //   BridgeNode->attachObject(bridge);
	//BridgeNode->setPosition(0,0,1);
	//BridgeNode->yaw(Ogre::Radian(Ogre::Math::PI/2));
	//
	//Ogre::Entity* bush1 = mSceneMgr->createEntity("Busg", "bush.mesh");
	//Ogre::SceneNode* bush1Node = mSceneMgr->getRootSceneNode()->createChildSceneNode("bush");
 //   bush1Node->attachObject(bush1);
	//bush1Node->scale(.5f,1,20);
	//bush1Node->setPosition(12,0,10);
	//bush1Node->yaw(Ogre::Radian(Ogre::Math::PI/2));


    // Create a Light and set its position
    Ogre::Light* light = mSceneMgr->createLight("MainLight");


    light->setPosition(20.0f, 280.0f, 50.0f);
}



#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT )
#else
    int main(int argc, char *argv[])
#endif
    {
        // Create application object
        TutorialApplication app;

        try {
			

            app.go();
        } catch( Ogre::Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            MessageBox( NULL, e.getFullDescription().c_str(), "An exception has occured!", MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
            std::cerr << "An exception has occured: " <<
                e.getFullDescription().c_str() << std::endl;
#endif
        }

        return 0;
    }

#ifdef __cplusplus
}
#endif
