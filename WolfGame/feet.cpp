#include "stdafx.h"
#include "feet.h"


const float driftTogether = 1.2f;
const float forwardsStep = 0.8f;
const float horizontalStepMax = 0.5f;
const float horizontalStepMin = 0.1f;

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
	if(mLeftFootActive)
	{
		localMovement = -movement;
	}
	else
	{
		localMovement = movement;
	}

	Ogre::Vector3 currentPosition = rightFoot->getPosition();
	currentPosition += localMovement;

	if(currentPosition.z > forwardsStep)
	{
		currentPosition.z += (forwardsStep - currentPosition.z);
		mLeftFootActive = movement.z <=0;
	}
	else if(currentPosition.z < -forwardsStep)
	{
		currentPosition.z += (-forwardsStep - currentPosition.z);
		mLeftFootActive = movement.z >= 0;
	}
	if(currentPosition.x > horizontalStepMax)
	{
		currentPosition.x += (horizontalStepMax - currentPosition.x);
		mLeftFootActive = movement.x <=0;
	}
	else if(currentPosition.x < -horizontalStepMin)
	{
		currentPosition.x += (-horizontalStepMin - currentPosition.x);
		mLeftFootActive = movement.x >= 0;
	}

	if(!mLeftFootActive)
	{
		if(movement.z == 0.0f)
		{
			currentPosition.z /=driftTogether;
		}

		if(movement.x == 0.0f)
		{
			currentPosition.x /=driftTogether;
		}
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

	if(currentPosition.z > forwardsStep)
	{
		currentPosition.z += (forwardsStep - currentPosition.z);
		mLeftFootActive = movement.z <=0;
	}
	else if(currentPosition.z < -forwardsStep)
	{
		currentPosition.z += (-forwardsStep - currentPosition.z);
		mLeftFootActive = movement.z >= 0;
	}

	if(currentPosition.x < -horizontalStepMax)
	{
		currentPosition.x += (-horizontalStepMax - currentPosition.x);
		mLeftFootActive = movement.x >=0;
	}
	else if(currentPosition.x > horizontalStepMin)
	{
		currentPosition.x += (horizontalStepMin - currentPosition.x);
		mLeftFootActive = movement.x <= 0;
	}
	if(mLeftFootActive)
	{
		if(movement.z == 0.0f)
		{
			currentPosition.z /=driftTogether;
		}

		if(movement.x == 0.0f)
		{
			currentPosition.x /=driftTogether;
		}
	}

	leftFoot->setPosition(currentPosition);
}

void Feet::move(Ogre::Vector3 movement)
{
	movement.y = 0;

	movement.x /=2;

	moveRightFoot(movement);
	moveLeftFoot(movement);

}



