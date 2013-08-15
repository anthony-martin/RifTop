#pragma once
#include "stdafx.h"

class OculusControl
{
public:
	OculusControl(void);
	~OculusControl(void);
	Ogre::Quaternion getOrientation();
	OVR::HMDInfo getDeviceInfo();
};

