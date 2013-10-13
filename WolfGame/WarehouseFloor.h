#pragma once
#include "stdafx.h"
#include <string>
#include "SceneManagerExtensions.h"

class WarehouseFloor
{
public:
	WarehouseFloor(Ogre::SceneManager* sceneMgr);
	~WarehouseFloor(void);

private:
	void CreateCube(Ogre::Vector3 position, Ogre::Vector3 scale,std::string texture);
	Ogre::SceneManager* mSceneMgr;
	Ogre::SceneNode* mMainNode;
	void createWalls();
	void addSteps();
	int mCount;
};

