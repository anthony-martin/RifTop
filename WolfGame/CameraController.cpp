#include "CameraController.h"


using namespace CameraController;

namespace
{
const float g_defaultFarClip = 10000.0f;
}

Controller::Controller()
{
	mStereoConfig = new OVR::Util::Render::StereoConfig();
}

void SetupCamera(Ogre::Camera* camera, OVR::Util::Render::StereoConfig *config, float side)
{
	camera->setNearClipDistance(config->GetEyeToScreenDistance());
	camera->setFarClipDistance(g_defaultFarClip);
	camera->setAspectRatio(config->GetAspect());
	camera->setFOVy(Ogre::Radian(config->GetYFOVRadians()));
			
			// Oculus requires offset projection, create a custom projection matrix
	Ogre::Matrix4 proj = Ogre::Matrix4::IDENTITY;
	float temp = config->GetProjectionCenterOffset();
	proj.setTrans(Ogre::Vector3(side *config->GetProjectionCenterOffset() , 0, 0));
	camera->setCustomProjectionMatrix(true, proj * camera->getProjectionMatrix());
}

void Controller::createCameras(Ogre::SceneManager* mSceneMgr)
{
	mRotationNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Head");
	mLeftCameraNode = mRotationNode->createChildSceneNode("LeftEye");
	mRightCameraNode = mRotationNode->createChildSceneNode("RightEye");
	mRotationNode->setPosition(Ogre::Vector3(7.5,7.5,-15.0));
    // Create the camera
    mCamera = mSceneMgr->createCamera("LeftCamera");
    SetupCamera(mCamera,mStereoConfig, -1.0f);

	mLeftCameraNode->attachObject(mCamera);
	mLeftCameraNode->setPosition(mStereoConfig->GetIPD() * -0.5f, 0, 0);
	mCamera->lookAt(Ogre::Vector3(0,0,0));

    // Create the camera
    mCameraRight = mSceneMgr->createCamera("RightCamera");
    SetupCamera(mCameraRight,mStereoConfig, 1.0f);

	mRightCameraNode->attachObject(mCameraRight);
	mRightCameraNode->setPosition(mStereoConfig->GetIPD() * 0.5f, 0, 0);
	mCameraRight->lookAt(Ogre::Vector3(0,0,0));

}


