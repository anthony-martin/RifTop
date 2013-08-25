#include "stdafx.h"
#include "WarehouseFloor.h"


//blocks 3x2 bottom centre 2.5, 3.5/ 5.5, 3.5
using namespace Ogre;

void DestroyAllAttachedMovableObjects( SceneNode* i_pSceneNode )
{
   if ( i_pSceneNode )
   {
  
   // Destroy all the attached objects
   SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

   while ( itObject.hasMoreElements() )
   {
      MovableObject* pObject = static_cast<MovableObject*>(itObject.getNext());
      i_pSceneNode->getCreator()->destroyMovableObject( pObject );
   }

   // Recurse to child SceneNodes
   SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

   while ( itChild.hasMoreElements() )
   {
      SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
      DestroyAllAttachedMovableObjects( pChildNode );
   }
   }
}

void WarehouseFloor::createWalls()
{
	Ogre::Entity *ent;
    Ogre::Plane p;
    p.normal = Ogre::Vector3(-1,0,0); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "RightSideWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 15, 9, 1, 1, false, 1, 1, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("RightSideWall", "RightSideWall");

	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	Ogre::SceneNode* headNode = mMainNode->createChildSceneNode("RightSideWall");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3(10,4.5,7.5));

    p.normal = Ogre::Vector3(1,0,0); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "RightSideWallout", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 15.02, 9, 1, 1, false, 1, 1, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("RightSideWallout", "RightSideWallout");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("RightSideWallout");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3(10.01,4.5,7.5));

    p.normal = Ogre::Vector3(1,0,0); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "LeftSideWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 15, 9, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("LeftSideWall", "LeftSideWall");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("LeftSideWall");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3(0,4.5,7.5));

	p.normal = Ogre::Vector3(-1,0,0); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "LeftSideWallout", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 15.02, 9, 1, 1, true, 1, 1, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("LeftSideWallout", "LeftSideWallout");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("LeftSideWallout");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3(-0.01,4.5,7.5));

	p.normal = Ogre::Vector3(0,0,-1); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "backWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 10, 9, 1, 1, true, 1, .66, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("backWall", "backWall");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("backWall");
    headNode->attachObject(ent);;
	headNode->setPosition(Ogre::Vector3(5,4.5,15));

	p.normal = Ogre::Vector3(0,0,1); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "backWallout", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 10.02, 9, 1, 1, true, 1, .66, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("backWallout", "backWallout");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("backWallout");
    headNode->attachObject(ent);;
	headNode->setPosition(Ogre::Vector3(5,4.5,15.01));

	p.normal = Ogre::Vector3(0,0,1); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "frontWall", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 10, 9, 1, 1, true, 1, .66, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("frontWall", "frontWall");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("frontWall");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3(5,4.5,0));

	p.normal = Ogre::Vector3(0,0,-1); p.d = 0;
    Ogre::MeshManager::getSingleton().createPlane(
        "frontWallout", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
		p, 10.02, 9, 1, 1, true, 1, .66, 1, Ogre::Vector3::UNIT_Y);
    ent = mSceneMgr->createEntity("frontWallout", "frontWallout");

	material = Ogre::MaterialManager::getSingleton().getByName("box/WallTripplePanel");
	ent->setMaterial(material);

	headNode = mMainNode->createChildSceneNode("frontWallout");
    headNode->attachObject(ent);
	headNode->setPosition(Ogre::Vector3(5,4.5,-0.01));
}

WarehouseFloor::WarehouseFloor(Ogre::SceneManager* sceneMgr)
{
	mSceneMgr = sceneMgr;
	mMainNode = mSceneMgr->getRootSceneNode()->createChildSceneNode("warehousebasenode");
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
	CreateCube(Vector3(8,2,13.5), Vector3(2,2,1.5), lightSingle);

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
	CreateCube(Vector3(5, 4.5, 11.5), Vector3(5, .1, 3.5), lightSingle);
	
	
	createWalls();
	//stairs
	//	stairs
	//CreateCube(Vector3(11,0.3,6), Ogre::Vector3(2, .3,.3), lightSingle);
	//CreateCube(Vector3(11,0.6,6.3), Ogre::Vector3(2, .3,.3), lightSingle);
	//CreateCube(Vector3(11,0.9,6.9), Ogre::Vector3(2, .3,.3), lightSingle);
	Ogre::Entity* cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "stairs.mesh");
	
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(lightSingle);
	cube->setMaterial(material);
    Ogre::SceneNode* headNode = mMainNode->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(Vector3(.5,0.1,1));
	headNode->setPosition(Vector3(9,0,3));
	headNode->yaw(Radian(Math::PI/2));
	mCount++;

	 cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "roof2.mesh");
	
	 material = Ogre::MaterialManager::getSingleton().getByName("box/roof");
	cube->setMaterial(material);
     headNode = mMainNode->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(Vector3(3.6,5,5.07));
	headNode->setPosition(Vector3(5,8.88,5));
	headNode->yaw(Radian(Math::PI));
	mCount++;

	 cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "beams.mesh");
	
	 material = Ogre::MaterialManager::getSingleton().getByName("box/roof");
	cube->setMaterial(material);
     headNode = mMainNode->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(Vector3(3.6,5,5.05));
	headNode->setPosition(Vector3(5,9,5));
	headNode->yaw(Radian(Math::PI));
	mCount++;
}


WarehouseFloor::~WarehouseFloor(void)
{
	DestroyAllAttachedMovableObjects(mMainNode);
	mMainNode->removeAndDestroyAllChildren();
	mSceneMgr->destroySceneNode(mMainNode);
}

void WarehouseFloor::CreateCube(Ogre::Vector3 position, Ogre::Vector3 scale, std::string texture)
{
	Ogre::Entity* cube = mSceneMgr->createEntity("warehouse_"+ Ogre::StringConverter::toString(mCount), "Boxnk2.mesh");
	
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName(texture);
	cube->setMaterial(material);
    Ogre::SceneNode* headNode = mMainNode->createChildSceneNode("warehouse_"+Ogre::StringConverter::toString(mCount));
    headNode->attachObject(cube);
	headNode->scale(scale);
	headNode->setPosition(position);
	mCount++;
}
