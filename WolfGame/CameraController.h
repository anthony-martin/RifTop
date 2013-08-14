#include <OgreCamera.h>
#include <OgreSceneManager.h>
#include <OgreRenderWindow.h>
#include <OgreViewport.h>
#include <OgreVector3.h>
#include <OVR.h>
#include <OgreCompositorManager.h>
#include <OgreCompositorInstance.h>
#include <OgreCompositionTargetPass.h>
#include <OgreCompositionPass.h>

namespace CameraController
{
	class Controller
	{
	public:
		Controller( Ogre::RenderWindow* window);
		void createCameras(Ogre::SceneManager* mSceneMgr);
		void createViewports(OVR::HMDInfo devinfo);
		Ogre::Camera* mCamera;
		Ogre::Camera* mCameraRight;
		Ogre::SceneNode* mRotationNode;
		Ogre::SceneNode* mBodyRotationNode;
		
	protected:
		OVR::Util::Render::StereoConfig *mStereoConfig; 
		Ogre::RenderWindow* mWindow;
		Ogre::SceneNode* mLeftCameraNode;
		Ogre::SceneNode* mRightCameraNode;
		Ogre::Viewport* mLeftVp;
		Ogre::Viewport* mRightVp;
		Ogre::CompositorInstance *mLeftCompositor;
		Ogre::CompositorInstance *mRightCompositor;

	private:
		void configureCompositors(OVR::HMDInfo devinfo);
	};
}
