#include "stdafx.h"

namespace CameraController
{
	class Controller
	{
	public:
		Controller( Ogre::RenderWindow* window);
		void createCameras(Ogre::SceneManager* mSceneMgr);
		void createViewports(OVR::HMDInfo devinfo);
		void addInput(Ogre::Vector3 input);
		void processMovement();
		Ogre::Camera* mCamera;
		Ogre::Camera* mCameraRight;
		Ogre::SceneNode* mRotationNode;
		Ogre::SceneNode* mBodyRotationNode;

	protected:
		OVR::Util::Render::StereoConfig *mStereoConfig; 
		Ogre::RenderWindow* mWindow;
		Ogre::Viewport* mLeftVp;
		Ogre::Viewport* mRightVp;
		Ogre::CompositorInstance *mLeftCompositor;
		Ogre::CompositorInstance *mRightCompositor;
		Ogre::Vector3 mInput;

	private:
		void configureCompositors(OVR::HMDInfo devinfo);
	};
}
