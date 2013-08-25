#include "stdafx.h"
#include "feet.h"


const float driftTogether = 1.2f;
const float forwardsStep = 0.8f;
const float horizontalStepMax = 0.5f;
const float horizontalStepMin = 0.0f;

Feet::Feet(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* player)
{
	mBody = player;
	leftMesh = sceneMgr->createEntity("left foot", "LeftFoot.mesh");
	
	upMaterial = Ogre::MaterialManager::getSingleton().getByName("player/feet");
	leftMesh->setMaterial(upMaterial);
    leftFoot = player->createChildSceneNode("left foot");
    leftFoot->attachObject(leftMesh);
	leftFoot->scale(Ogre::Vector3(0.2,1,0.2));
	leftFoot->setPosition(Ogre::Vector3(-.25,0.1, 0));
	leftFoot->yaw(Ogre::Radian(-Ogre::Math::PI/2));

	rightMesh = sceneMgr->createEntity("right foot", "Rightfoot.mesh");
	
	downMaterial = Ogre::MaterialManager::getSingleton().getByName("player/Activefoot");
	rightMesh->setMaterial(upMaterial);
    rightFoot = player->createChildSceneNode("right foot");
    rightFoot->attachObject(rightMesh);
	rightFoot->scale(Ogre::Vector3(0.2,1,0.2));
	rightFoot->setPosition(Ogre::Vector3(.25, 0.1, 0));
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
			if(currentPosition.x < 0.25)
			{
				currentPosition.x /=driftTogether;
			}
			else
			{
				currentPosition.x =0.25;
			}
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
			if(currentPosition.x >-0.25)
			{
				currentPosition.x /=driftTogether;
			}
			else
			{
				currentPosition.x = -0.25;
			}
		}
	}

	leftFoot->setPosition(currentPosition);
}

void Feet::move(Ogre::Vector3 movement)
{
	if(movement.y != 0.0f)
	{
		Ogre::Vector3 currentPosition = leftFoot->getPosition();
		if(currentPosition.x < -0.25)
		{
			currentPosition.x /=driftTogether;
		}
		else
		{
			currentPosition.x = -0.25;
		}

		currentPosition.z /=driftTogether;
		
		leftFoot->setPosition(currentPosition);

		currentPosition = rightFoot->getPosition();
		if(currentPosition.x > 0.25)
		{
			currentPosition.x /=driftTogether;
		}
		else
		{
			currentPosition.x = 0.25;
		}
		currentPosition.z /=driftTogether;
		rightFoot->setPosition(currentPosition);
	}
	else
	{
		movement.y = 0;
		//movement.x /=2;

		moveRightFoot(movement);
		moveLeftFoot(movement);
	}

	UpdateMaterial();
}

Ogre::Vector3 Feet::turn(Ogre::Radian turn)
{
	Ogre::Vector3 referencePosition;
	if(!mLeftFootActive)
	{
		referencePosition = mBody->convertLocalToWorldPosition(leftFoot->getPosition());
	}
	else
	{
		referencePosition = mBody->convertLocalToWorldPosition(rightFoot->getPosition());
	}

	mBody->yaw(turn);

	Ogre::Vector3 newPosition;
	if(!mLeftFootActive)
	{
		newPosition = mBody->convertLocalToWorldPosition(leftFoot->getPosition());
	}
	else
	{
		newPosition = mBody->convertLocalToWorldPosition(rightFoot->getPosition());
	}

	return referencePosition - newPosition;
}

void Feet::UpdateMaterial()
{
	if(!mLeftFootActive)
	{
		leftMesh->setMaterial(downMaterial);
		rightMesh->setMaterial(upMaterial);
	}
	else
	{
		leftMesh->setMaterial(upMaterial);
		rightMesh->setMaterial(downMaterial);
	}
}



