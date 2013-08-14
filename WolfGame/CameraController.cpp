#include "CameraController.h"


using namespace CameraController;

namespace
{
const float g_defaultFarClip = 10000.0f;
}

Controller::Controller( Ogre::RenderWindow* window)
{
	mWindow = window;
	mStereoConfig = new OVR::Util::Render::StereoConfig();
}

void Controller::configureCompositors(OVR::HMDInfo devinfo)
{
	Ogre::MaterialPtr matLeft = Ogre::MaterialManager::getSingleton().getByName("Ogre/Compositor/Oculus");
	
	Ogre::MaterialPtr matRight = matLeft->clone("Ogre/Compositor/Oculus/Right");
	Ogre::GpuProgramParametersSharedPtr pParamsLeft = matLeft->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	Ogre::GpuProgramParametersSharedPtr pParamsRight = matRight->getTechnique(0)->getPass(0)->getFragmentProgramParameters();
	
	Ogre::Vector4 hmdwarp = Ogre::Vector4(mStereoConfig->GetDistortionK(0),
							mStereoConfig->GetDistortionK(1),
							mStereoConfig->GetDistortionK(2),
							mStereoConfig->GetDistortionK(3));

	pParamsLeft->setNamedConstant("HmdWarpParam", hmdwarp);
	pParamsRight->setNamedConstant("HmdWarpParam", hmdwarp);

	Ogre::Vector4 ChromAb;
	ChromAb = Ogre::Vector4(devinfo.ChromaAbCorrection[0],
								devinfo.ChromaAbCorrection[1],
								devinfo.ChromaAbCorrection[2],
								devinfo.ChromaAbCorrection[3]);

	pParamsLeft->setNamedConstant("ChromAbParam", ChromAb);
	pParamsRight->setNamedConstant("ChromAbParam", ChromAb);


	pParamsLeft->setNamedConstant("LensCentre", 0.5f+(mStereoConfig->GetProjectionCenterOffset()/2.0f));
	pParamsRight->setNamedConstant("LensCentre", 0.5f-(mStereoConfig->GetProjectionCenterOffset()/2.0f));

	Ogre::CompositorPtr comp = Ogre::CompositorManager::getSingleton().getByName("OculusRight");
	comp->getTechnique(0)->getOutputTargetPass()->getPass(0)->setMaterialName("Ogre/Compositor/Oculus/Right");

	mLeftCompositor = Ogre::CompositorManager::getSingleton().addCompositor(mLeftVp , "OculusLeft");
	mLeftCompositor->setEnabled(true);
	mRightCompositor = Ogre::CompositorManager::getSingleton().addCompositor(mRightVp , "OculusRight");
	mRightCompositor->setEnabled(true);
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
	mBodyRotationNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("Body");
	mRotationNode = mBodyRotationNode->createChildSceneNode("Head");
	//mLeftCameraNode = mRotationNode->createChildSceneNode("LeftEye");
	//mRightCameraNode = mRotationNode->createChildSceneNode("RightEye");
	mBodyRotationNode->setPosition(Ogre::Vector3(7.5,0,-15.0));
    // Create the camera
    mCamera = mSceneMgr->createCamera("LeftCamera");
    SetupCamera(mCamera,mStereoConfig, 1.0f);

	mRotationNode->attachObject(mCamera);
	mCamera->setPosition(mStereoConfig->GetIPD() * 0.5f, 0, 0);

    // Create the camera
    mCameraRight = mSceneMgr->createCamera("RightCamera");
    SetupCamera(mCameraRight,mStereoConfig, -1.0f);

	mRotationNode->attachObject(mCameraRight);
	mCameraRight->setPosition(mStereoConfig->GetIPD() * -0.5f, 0, 0);


}

void Controller::createViewports(OVR::HMDInfo devinfo)
{
	// Create one viewport, entire window
    mLeftVp = mWindow->addViewport(mCamera, 0, 0.0f,0.0f,0.5f,1.0f);
	mRightVp = mWindow->addViewport(mCameraRight, 1, 0.5f,0.0f,0.5f,1.0f);

    mLeftVp->setBackgroundColour(Ogre::ColourValue(0,1,0));
	mRightVp->setBackgroundColour(Ogre::ColourValue(0,1,0));

	configureCompositors(devinfo);
}




