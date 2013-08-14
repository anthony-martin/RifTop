#pragma once

#include <OVR.h>
#include "OgreQuaternion.h"

class OculusControl
{
public:
	OculusControl(void);
	~OculusControl(void);
	Ogre::Quaternion getOrientation();
	OVR::HMDInfo getDeviceInfo();
};

