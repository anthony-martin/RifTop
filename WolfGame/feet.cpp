#include "stdafx.h"
#include "feet.h"


const float driftTogether = 1.2f;
const float forwardsStep = 0.8f;
const float horizontalStepMax = 0.8f;
const float horizontalStepMin = 0.0f;

Feet::Feet(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* player)
{
	mBody = player;
	mCollisionTools = new MOC::CollisionTools(sceneMgr);
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

void Feet::swapFeet()
{
	mLeftFootActive = !mLeftFootActive;
	UpdateMaterial();
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

	currentPosition.y = distanceToGround(rightFoot);
	if(currentPosition.y > -0.1f && 
		currentPosition.y < 0.1f)
	{
		currentPosition.y = 0.0f;
	}

	if(!mLeftFootActive && 
		(currentPosition.y != -0.0f) &&
		(currentPosition.z < 0.3f || 
		currentPosition.z > -0.3f))
	{
		mLeftFootActive = true;
	}
	currentPosition.y += 0.1;

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
	currentPosition.y = distanceToGround(leftFoot);

	if(currentPosition.y > -0.1f && 
		currentPosition.y < 0.1f)
	{
		currentPosition.y = 0.0f;
	}

	if(!mLeftFootActive && 
		(currentPosition.y != -0.0f) &&
		(currentPosition.z < 0.3f || 
		currentPosition.z > -0.3f))
	{
		mLeftFootActive = true;
	}
	currentPosition.y += 0.1;

	leftFoot->setPosition(currentPosition);
}

void Feet::move(Ogre::Vector3 movement)
{
	if(movement.y >= 0.01f || movement.y <= -0.01f)
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

bool Feet::onGround()
{
	//don't stand on own feet
	setVisible(false);

	Ogre::Vector3 result;
	//Vector3 myResult(0, 0, 0);
	Ogre::Entity* myObject = NULL;
	float distToColl = 0.0f;

	Ogre::Vector3 colisionOrigin;
	if(!mLeftFootActive)
	{
		colisionOrigin = mBody->convertLocalToWorldPosition(leftFoot->getPosition());
	}
	else
	{
		colisionOrigin = mBody->convertLocalToWorldPosition(rightFoot->getPosition());
	}

	colisionOrigin.y += 0.2f;

	Ogre::Vector3 rayOrigins[5] =
	{
		Ogre::Vector3(0.0,0.0, -0.05),
		Ogre::Vector3(0.0,0.0, 0.0),
		Ogre::Vector3(-0.03,0.0, 0.0),
		Ogre::Vector3(0.04,0.0, 0.0),
		Ogre::Vector3(0.0,0.0, 0.08)
	};

	for(int i=0; i<5; i++)
	{
		if(mCollisionTools->raycastFromPoint(colisionOrigin+rayOrigins[i], Ogre::Vector3::NEGATIVE_UNIT_Y, result,myObject,distToColl))
		{
			if(distToColl <= 0.25f)
			{
				return true;
			}
		}
	}
	return false;

	//show feet
	setVisible(true);
}


float Feet::distanceToGround(float travel)
{
	float toGround;
	if(!mLeftFootActive)
	{
		toGround = distanceToGround( leftFoot);
	}
	else
	{
		toGround =distanceToGround(leftFoot);
	}

	if(toGround < -0.05f || 
		toGround > 0.05f) 
	{
		return toGround += 0.05f;
	}

	return travel;
}

float Feet::distanceToGround(Ogre::SceneNode* foot)
{
	Ogre::Vector3 colisionOrigin = mBody->convertLocalToWorldPosition(foot->getPosition());

	colisionOrigin.y += 1.75/2.0 ;

	Ogre::Vector3 result;
	Ogre::Entity* myObject = NULL;
	float distToColl = -1.0f;

	Ogre::Vector3 rayOrigins[5] =
	{
		Ogre::Vector3(0.0,0.0, -0.05),
		Ogre::Vector3(0.0,0.0, 0.0),
		Ogre::Vector3(-0.03,0.0, 0.0),
		Ogre::Vector3(0.04,0.0, 0.0),
		Ogre::Vector3(0.0,0.0, 0.08)
	};

	for(int i=0; i<5; i++)
	{
		if(mCollisionTools->raycastFromPoint(colisionOrigin + rayOrigins[i], Ogre::Vector3::NEGATIVE_UNIT_Y, result,myObject,distToColl))
		{
			//note if you dont check a little extra it jumps around when you walk down a slope
			if(distToColl <  .75*  1.75)
			{
				return 1.75/2.0 - distToColl ;
			}
		}
	}

	return 0;
}




