#include "stdafx.h"
#include "WarehouseFloor.h"


//blocks 3x2 bottom centre 2.5, 3.5/ 5.5, 3.5
using namespace Ogre;

WarehouseFloor::WarehouseFloor(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	mCount = 0;

	std::string lightSingle = "box/singlelight";

	Ogre::Vector3 largeCube(1.5,1,1.5);
	// large 3x2 cubes at near the door stacked two high
	CreateCube(Vector3(2.5,1,3.5), largeCube, lightSingle);
	CreateCube(Vector3(2.5,3,3.5), largeCube, lightSingle);
	CreateCube(Vector3(2.5,1,6.5), largeCube, lightSingle);
	CreateCube(Vector3(2.5,3,6.5), largeCube, lightSingle);
	CreateCube(Vector3(2.5,1,9.5), largeCube, lightSingle);
	CreateCube(Vector3(2.5,3,9.5), largeCube, lightSingle);

	std::string doubleboxes = "box/double";
	CreateCube(Vector3(6.5,1,3.5), largeCube, doubleboxes);
	CreateCube(Vector3(6.5,3,3.5), largeCube, doubleboxes);
	CreateCube(Vector3(6.5,1,6.5), largeCube, doubleboxes);
	CreateCube(Vector3(6.5,3,6.5), largeCube, doubleboxes);
	CreateCube(Vector3(6.5,1,9.5), largeCube, doubleboxes);
	CreateCube(Vector3(6.5,3,9.5), largeCube, doubleboxes);

	//CreateCube(Vector3(5.5,1,3.5), Vector3(3,2,3));
	CreateCube(Vector3(8,2.5,13.5), Vector3(2,2.5,1.5), lightSingle);

	Ogre::Vector3 smallCube(.5, .6,.5);

	//CreateCube(Vector3(2,1.8,12.5), Vector3(2, 1.8,.5), "box/tripple");
	CreateCube(Vector3(.5,.6,12.5), smallCube, lightSingle);
	CreateCube(Vector3(1.5,.6,12.5), smallCube, lightSingle);
	CreateCube(Vector3(2.5,.6,12.5), smallCube, lightSingle);

	CreateCube(Vector3(.5,1.8,12.5), smallCube, lightSingle);
	CreateCube(Vector3(1.5,1.8,12.5), smallCube, lightSingle);
	CreateCube(Vector3(2.5,1.8,12.5), smallCube, lightSingle);

	CreateCube(Vector3(.5,3,12.5), smallCube, lightSingle);
	CreateCube(Vector3(1.5,3,12.5), smallCube, lightSingle);
	CreateCube(Vector3(2.5,3,12.5), smallCube, lightSingle);

	CreateCube(Vector3(3.5,.6,12.5), smallCube, lightSingle);
	CreateCube(Vector3(3.5,.6,11.5), smallCube, lightSingle);

	CreateCube(Vector3(5.5,.6,13), smallCube, lightSingle);
	CreateCube(Vector3(5.5,.6,12), smallCube, lightSingle);

	//jump cubes
	CreateCube(Vector3(4.5,.3,8.5), Ogre::Vector3(.5, .3,.5), lightSingle);
	CreateCube(Vector3(4.5,.6, 5), smallCube, lightSingle);
	
	//tripple cubes near back entrace
	CreateCube(Vector3(3.5,.6,14.5), smallCube, lightSingle);
	CreateCube(Vector3(4.5,.6,14.5), smallCube, lightSingle);
	CreateCube(Vector3(5.5,.6,14.5), smallCube, lightSingle);

	std::string singledark = "box/singledark";

	Ogre::Vector3 mediumCube(1.5, 1.2,1);
	CreateCube(Vector3(4.5,2.4,14), mediumCube, singledark);
	CreateCube(Vector3(4.5,2.4,12), mediumCube, singledark);
	//floorplan remove before release
	CreateCube(Vector3(5, 4, 7.5), Vector3(5, .2, 7.5), lightSingle);

	//stairs
	//	stairs
	//CreateCube(Vector3(11,0.3,6), Ogre::Vector3(2, .3,.3), lightSingle);
	//CreateCube(Vector3(11,0.6,6.3), Ogre::Vector3(2, .3,.3), lightSingle);
	//CreateCube(Vector3(11,0.9,6.9), Ogre::Vector3(2, .3,.3), lightSingle);
	Ogre::Entity* cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "Boxnk2.mesh");
	
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(lightSingle);
	cube->setMaterial(material);
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(Vector3(2,0.1,4));
	headNode->setPosition(Vector3(11,1,6));
	headNode->pitch(Radian(-Math::PI/6));
	mCount++;
}


WarehouseFloor::~WarehouseFloor(void)
{
}

void WarehouseFloor::CreateCube(Ogre::Vector3 position, Ogre::Vector3 scale, std::string texture)
{
	Ogre::Entity* cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "Boxnk2.mesh");
	
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(texture);
	cube->setMaterial(material);
    Ogre::SceneNode* headNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(scale);
	headNode->setPosition(position);
	mCount++;
}
