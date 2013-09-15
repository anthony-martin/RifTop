#include "stdafx.h"
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
	mInput = Ogre::Vector3::ZERO;
}

Controller::~Controller( )
{
	Ogre::CompositorManager::getSingleton().removeCompositor(mLeftVp, "Oculus");
	Ogre::CompositorManager::getSingleton().removeCompositor(mRightVp, "Oculus");

	mWindow->removeAllViewports();
	
	mCamera->getParentSceneNode()->detachObject(mCamera);
	mCameraRight->getParentSceneNode()->detachObject(mCameraRight);
	mSceneMgr->destroyCamera(mCamera);
	mSceneMgr->destroyCamera(mCameraRight);

	mSceneMgr->destroySceneNode(mRotationNode);
	mSceneMgr->destroySceneNode(mBodyRotationNode);


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
	//mLeftCompositor->setEnabled(true);
	mRightCompositor = Ogre::CompositorManager::getSingleton().addCompositor(mRightVp , "OculusRight");
	//mRightCompositor->setEnabled(true);
}

void Controller::SetupCamera(Ogre::Camera* camera, OVR::Util::Render::StereoConfig *config, float side)
{
	camera->setNearClipDistance(0.02f);
	camera->setFarClipDistance(g_defaultFarClip);
	camera->setAspectRatio(config->GetAspect());
	//Ogre::Radian fovY = 2.0f* Ogre::Math::ATan(0.0935f/(2*config->GetEyeToScreenDistance()));
	//maigc fucking number for vertical fov.....
	//the settings returns too big the docs calc too small
	Ogre::Radian otherFovY = Ogre::Radian(1.90240888f);
	camera->setFOVy(otherFovY);
	
	//camera->setFOVy(Ogre::Radian(config->GetYFOVRadians()));
	//camera->setPosition(mStereoConfig->GetIPD() * 0.5f *side, 0, config->GetEyeToScreenDistance());
			// Oculus requires offset projection, create a custom projection matrix
	Ogre::Matrix4 proj = Ogre::Matrix4::IDENTITY;
	float temp = config->GetProjectionCenterOffset();
	proj.setTrans(Ogre::Vector3(side *config->GetProjectionCenterOffset() , 0, 0));
	camera->setCustomProjectionMatrix(true, proj * camera->getProjectionMatrix());
}

void Controller::createCameras(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	mBodyRotationNode = mSceneMgr->createSceneNode("eyes"); 
	mRotationNode = mBodyRotationNode->createChildSceneNode("Head");
	mBodyRotationNode->setPosition(Ogre::Vector3(0,1.75,0));
    // Create the camera
    mCamera = mSceneMgr->createCamera("LeftCamera");
    SetupCamera(mCamera,mStereoConfig, 1.0f);
	mRotationNode->attachObject(mCamera);
	//left eye position
	mCamera->setPosition(mStereoConfig->GetIPD() * -0.5f, 0, 0);
	
    // Create the camera
    mCameraRight = mSceneMgr->createCamera("RightCamera");
    SetupCamera(mCameraRight,mStereoConfig, -1.0f);

	mRotationNode->attachObject(mCameraRight);

	//right eye position
	mCameraRight->setPosition(mStereoConfig->GetIPD() * 0.5f, 0, 0);
}

void Controller::createViewports(OVR::HMDInfo devinfo)
{
	// Create one viewport, entire window
    mLeftVp = mWindow->addViewport(mCamera, 0, 0.0f,0.0f,0.5f,1.0f);
	mRightVp = mWindow->addViewport(mCameraRight, 1, 0.5f,0.0f,0.5f,1.0f);
	mLeftVp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
	mRightVp->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

    mLeftVp->setBackgroundColour(Ogre::ColourValue(0,0,1));
	mRightVp->setBackgroundColour(Ogre::ColourValue(0,0,1));

	configureCompositors(devinfo);
}




