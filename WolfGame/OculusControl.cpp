#include "OculusControl.h"

using namespace OVR;
Ptr<DeviceManager>	pManager;
Ptr<HMDDevice>		pHMD;
Ptr<SensorDevice>	pSensor;
SensorFusion		FusionResult;
HMDInfo			Info;
bool			InfoLoaded;
OculusControl::OculusControl(void)
{
	System::Init();

	pManager = *DeviceManager::Create();

	pHMD = *pManager->EnumerateDevices<HMDDevice>().CreateDevice();

	if (pHMD)
        {
           InfoLoaded = pHMD->GetDeviceInfo(&Info);

	   pSensor = *pHMD->GetSensor();
	}
	else
	{
	   pSensor = *pManager->EnumerateDevices<SensorDevice>().CreateDevice();
	}

	if (pSensor)
	{
	   FusionResult.AttachToSensor(pSensor);
	}
}


OculusControl::~OculusControl(void)
{
	pSensor.Clear();
    pHMD.Clear();
	pManager.Clear();

	System::Destroy();
}
