#pragma once
#include "stdafx.h"
#include <string>

class WarehouseFloor
{
public:
	WarehouseFloor(Ogre::SceneManager* sceneMgr);
	~WarehouseFloor(void);

private:
	void CreateCube(Ogre::Vector3 position, Ogre::Vector3 scale,std::string texture);
	Ogre::SceneManager* mSceneMgr;
	int mCount;
};

