#include "stdafx.h"

namespace CameraController
{
	class Controller
	{
	public:
		Controller( Ogre::RenderWindow* window);
		~Controller();
		void createCameras(Ogre::SceneManager* mSceneMgr);
		void createViewports(OVR::HMDInfo devinfo);
		Ogre::Camera* mCamera;
		Ogre::Camera* mCameraRight;
		Ogre::SceneNode* mRotationNode;
		Ogre::SceneNode* mBodyRotationNode;
		void createViewports();

	protected:
		OVR::Util::Render::StereoConfig *mStereoConfig; 
		Ogre::RenderWindow* mWindow;
		Ogre::Viewport* mLeftVp;
		Ogre::Viewport* mRightVp;
		Ogre::CompositorInstance *mLeftCompositor;
		Ogre::CompositorInstance *mRightCompositor;
		Ogre::Vector3 mInput;

	private:
		Ogre::SceneManager* mSceneMgr;
		void SetupCamera(Ogre::Camera* camera, OVR::Util::Render::StereoConfig *config, float side);
		void configureCompositors(OVR::HMDInfo devinfo);
	};
}
