#pragma once
#include "stdafx.h"

class WarehouseFloor
{
public:
	WarehouseFloor(Ogre::SceneManager* sceneMgr);
	~WarehouseFloor(void);

private:
	void CreateCube(Ogre::Vector3 position, Ogre::Vector3 scale);
	Ogre::SceneManager* mSceneMgr;
	int mCount;
};

