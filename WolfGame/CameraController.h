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
	protected:
		OVR::Util::Render::StereoConfig *mStereoConfig; 
	};
}
