#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OVR.h>

namespace CameraController
{
	class Controller
	{
	public:
		Controller();
		void createCameras(Ogre::SceneManager* mSceneMgr);
		Ogre::Camera* mCamera;
		Ogre::Camera* mCameraRight;
		Ogre::SceneNode* mRotationNode;
		
	protected:
		OVR::Util::Render::StereoConfig *mStereoConfig; 
		Ogre::SceneNode* mLeftCameraNode;
		Ogre::SceneNode* mRightCameraNode;
	};
}
