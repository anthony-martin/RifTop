#include "stdafx.h"
#include "feet.h"


Feet::Feet(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* player)
{
	mBody = player;
	Ogre::Entity* cube = sceneMgr->createEntity("left foot", "LeftFoot.mesh");
	
	Ogre::MaterialPtr material = Ogre::MaterialManager::getSingleton().getByName("player/feet");
	cube->setMaterial(material);
    leftFoot = player->createChildSceneNode("left foot");
    leftFoot->attachObject(cube);
	leftFoot->scale(Ogre::Vector3(0.2,1,0.2));
	leftFoot->setPosition(Ogre::Vector3(0.1,0.1, 0));
	leftFoot->yaw(Ogre::Radian(-Ogre::Math::PI/2));


	cube = sceneMgr->createEntity("right foot", "Rightfoot.mesh");
	
	material = Ogre::MaterialManager::getSingleton().getByName("player/feet");
	cube->setMaterial(material);
    rightFoot = player->createChildSceneNode("right foot");
    rightFoot->attachObject(cube);
	rightFoot->scale(Ogre::Vector3(0.2,1,0.2));
	rightFoot->setPosition(Ogre::Vector3(-0.1,0.1, 0));
	rightFoot->yaw(Ogre::Radian(-Ogre::Math::PI/2));

	mLeftFootActive = false;
	mRightFootActive = true;
}


Feet::~Feet(void)
{
}

void Feet::setVisible(bool visible)
{
	leftFoot->setVisible(visible);
	rightFoot->setVisible(visible);
}

void Feet::moveRightFoot(Ogre::Vector3 movement)
{
	Ogre::Vector3 localMovement;
	if(!mRightFootActive)
	{
		localMovement = -movement;
	}
	else
	{
		localMovement = movement;
	}

	Ogre::Vector3 currentPosition = rightFoot->getPosition();
	currentPosition += localMovement;

	if(currentPosition.z > .8f)
	{
		currentPosition.z += (.8f - currentPosition.z);
		mRightFootActive = movement.z <=0;
	}

	if(currentPosition.z < -.8f)
	{
		currentPosition.z += (-.8f - currentPosition.z);
		mRightFootActive = movement.z >= 0;
	}

	rightFoot->setPosition(currentPosition);
}

void Feet::moveLeftFoot(Ogre::Vector3 movement)
{
	Ogre::Vector3 localMovement;
	if(!mLeftFootActive)
	{
		localMovement = -movement;
	}
	else
	{
		localMovement = movement;
	}

	Ogre::Vector3 currentPosition = leftFoot->getPosition();
	currentPosition += localMovement;

	if(currentPosition.z > .8f)
	{
		currentPosition.z += (.8f - currentPosition.z);
		mLeftFootActive = movement.z <=0;
	}

	if(currentPosition.z < -.8f)
	{
		currentPosition.z += (-.8f - currentPosition.z);
		mLeftFootActive = movement.z >= 0;
	}

	leftFoot->setPosition(currentPosition);
}

void Feet::move(Ogre::Vector3 movement)
{
	//changing height is for later
	movement.y = 0;
	movement.x = 0;

	moveRightFoot(movement);
	moveLeftFoot(movement);
	/*movement =  mBody->getOrientation() * movement;
	if(mLeftFootActive)
	{
		leftFoot->translate(movement);
		rightFoot->translate(-movement);
	}
	else
	{
		leftFoot->translate(-movement);
		rightFoot->translate(movement);
	}

	Ogre::Vector3 spaceing = leftFoot->getPosition() -  rightFoot->getPosition();
	float spacing = spaceing.squaredLength();

	if( spacing> 2.0f)
	{
		mLeftFootActive = !mLeftFootActive;
	}*/
	//return Ogre::Vector3::ZERO;
}



