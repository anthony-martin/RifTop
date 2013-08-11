#include "CameraController.h"


using namespace CameraController;
Controller::Controller()
{
	mStereoConfig = new OVR::Util::Render::StereoConfig();
}

void Controller::createCameras(Ogre::SceneManager* mSceneMgr)
{
    // Create the camera
    mCamera = mSceneMgr->createCamera("PlayerCam");

    // Position it at 500 in Z direction
    mCamera->setPosition(Ogre::Vector3(7.5,7.5,-15.0));
    // Look back along -Z
    mCamera->lookAt(Ogre::Vector3(0,0,0));
    mCamera->setNearClipDistance(5);

}