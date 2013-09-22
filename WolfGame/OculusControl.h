#pragma once
#include "stdafx.h"

class OculusControl
{
private:
	bool m_Initialised;

public:
	OculusControl(void);
	~OculusControl(void);
	Ogre::Quaternion getOrientation();
	OVR::HMDInfo getDeviceInfo();
	bool isInitialised();
};

