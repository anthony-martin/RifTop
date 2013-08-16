#include "stdafx.h"
#include "WarehouseFloor.h"


//blocks 3x2 bottom centre 2.5, 3.5/ 5.5, 3.5
using namespace Ogre;

WarehouseFloor::WarehouseFloor(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	mCount = 0;
	// large 3x2 cubes at near the door stacked two high
	CreateCube(Vector3(2.5,1,3.5), Vector3(1.5,1,1.5));
	CreateCube(Vector3(2.5,1,6.5), Vector3(1.5,1,1.5));
	CreateCube(Vector3(2.5,3,3.5), Vector3(1.5,1,1.5));
	CreateCube(Vector3(2.5,3,6.5), Vector3(1.5,1,1.5));


	//CreateCube(Vector3(5.5,1,3.5), Vector3(3,2,3));
	CreateCube(Vector3(8,2.5,13.5), Vector3(2,2.5,1.5));


	CreateCube(Vector3(2,1.8,12.5), Vector3(2, 1.8,.5));
	CreateCube(Vector3(4.5,.6,14.5), Vector3(1.5, .6,.5));
	CreateCube(Vector3(4.5,2.4,14), Vector3(1.5, 1.2,1));
	
	//floorplan remove before release
	CreateCube(Vector3(5, 4, 7.5), Vector3(5, .2, 7.5));
}


WarehouseFloor::~WarehouseFloor(void)
{
}

void WarehouseFloor::CreateCube(Ogre::Vector3 position, Ogre::Vector3 scale)
{
	Ogre::Entity* cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "Boxnk2.mesh");
	
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("box/single");
	cube->setMaterial(material);
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(scale);
	headNode->setPosition(position);
	mCount++;
}
