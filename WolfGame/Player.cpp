#include "stdafx.h"
#include "Player.h"

const float minHeight = 0.5f;
const float maxHeight = 1.75f;

Player::Player(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* eyeNode)
{
	Ogre::SceneNode* playerNode = sceneMgr->getRootSceneNode()->createChildSceneNode("bodyNode");
	mCollisionTools = new MOC::CollisionTools(sceneMgr);

	playerNode->addChild(eyeNode);
	mPlayerNode = playerNode;
	mPlayerNode->setPosition(Ogre::Vector3(7.5,0,-15));
	mEyeNode = eyeNode;
	mJumping = false;
	mDoubleJumping = false;
	mChangeHeight = false;
	mInput = Ogre::Vector3::ZERO;
	mJumpDuration=0;
}


Player::~Player(void)
{
}

void Player::addKeyboardInput(Ogre::Vector3 input)
{
	mInput += input;
}

void Player::mouseInput(Ogre::Vector2 input)
{
	mEyeNode->yaw(Ogre::Radian(input.x / -80.0));
	if(mChangeHeight)
	{
		Ogre::Vector3 eyePosition = mEyeNode->getPosition();
		float height =  eyePosition.y;
		height -= input.y/200.0;

		if(height < minHeight)
		{
			height = minHeight;
		}
		if(height > maxHeight)
		{
			height = maxHeight;
		}

		mEyeNode->setPosition(Ogre::Vector3(eyePosition.x, height, eyePosition.z));
	}
}

void Player::processJump(bool onGround, Ogre::Real timeSinceLastFrame)
{
	if( mJumping)
	{
		if(mJumpDuration == 0)
		{
			mJumpVector = mInput + Ogre::Vector3::UNIT_Y * 4.0f;
		}
		mJumpDuration += timeSinceLastFrame;

		//the jump duration
		if(mJumpVector.y <= 0.0f)
		{
			mJumping = false;
		}
	}
	else
	{
		if(!onGround && mJumpDuration == 0)
		{
			//clearly falling if we get here keep the movement
			mJumpVector = mInput + + Ogre::Vector3::NEGATIVE_UNIT_Y * 3.0f;
		}
	}

	if(!mJumping && onGround)
	{
		mJumpDuration = 0;
		mDoubleJumping = false;
		//mJumpVector = Ogre::Vector3::ZERO;
	}
}

void Player::processMovement(Ogre::Real timeSinceLastFrame)
{
	bool onGround = OnGround();
	if(onGround && !mJumping && mInput == Ogre::Vector3::ZERO)
	{
		//break early no movement happening so perform no updates
		// this means currently we cannot collide with moving objects while standing still
		return;
	}
	
	processJump(onGround, timeSinceLastFrame);

	Ogre::Vector3 direction;
	if(mJumping || !onGround)
	{
		if(mJumpVector.y > -3.0f)
		{
			mJumpVector.y -= 3.0f * mJumpDuration;
		}
		direction = mEyeNode->getOrientation() * mJumpVector ;
	}
	else
	{
		direction = mEyeNode->getOrientation() * mInput;
	}

	//normalisation sounds like a great idea but it plays hell with jumping
	direction = direction *(timeSinceLastFrame * 4.0f);
	
	bool up = direction.y > 0;
	float verticalClearance = checkVerticalClearance(up, direction.y );

	//we are moving up but do not have the remaining space for it
	if((mJumping || !onGround))
	{
		if(up && verticalClearance != direction.y)
		{
			mJumpVector.y = 0.0f;
		}
	}

	direction.y = verticalClearance;

    Ogre::Vector3 colisionNormal = Ogre::Vector3( direction);
	colisionNormal.y = 0;

	if(checkHorizontalColisions(colisionNormal))
	{
		direction.x = 0.0f;
		direction.z = 0.0f;
	}

	mPlayerNode->translate(direction);
}

float Player::checkVerticalClearance(bool up, float travel)
{
	Ogre::Vector3 result;
	Ogre::Entity* myObject = NULL;
	float distToColl = -1.0f;

	Ogre::Vector3 origin;
	Ogre::Vector3 normal;


	float eyeHeight = mEyeNode->getPosition().y;
	if(up)
	{
		origin = mPlayerNode->getPosition();
		origin.y += travel;
		normal = Ogre::Vector3::UNIT_Y;
	}
	else
	{
		origin = mPlayerNode->getPosition();
		origin.y += eyeHeight/2 ;

		normal = Ogre::Vector3::NEGATIVE_UNIT_Y;
	}

	if(mCollisionTools->raycastFromPoint(origin, normal, result,myObject,distToColl))
	{
		if(up && distToColl  < eyeHeight + 0.1f)
		{
			return  0.0f;
		}
		else
		{
			if(distToColl <  .75* eyeHeight)
			{
				return eyeHeight/2 - distToColl;
			}
		}
	}

	return travel;
}

bool Player::checkHorizontalColisions( Ogre::Vector3 normal)
{
	Ogre::Vector3 result;
	Ogre::Entity* myObject = NULL;
	float distToColl = -1.0f;



	float eyeHeight =  mEyeNode->getPosition().y;
	
	//5 rays
	Ogre::Vector3 leftEye = mPlayerNode->getPosition();
	leftEye.y += eyeHeight;
	leftEye.z -= -0.1f;
	Ogre::Vector3 rightEye= mPlayerNode->getPosition();
	rightEye.y += eyeHeight;
	rightEye.z -= 0.1f;
	Ogre::Vector3 chest= mPlayerNode->getPosition();
	chest.y +=.75 * eyeHeight;
	Ogre::Vector3 waist= mPlayerNode->getPosition();
	waist.y += .4f *eyeHeight ;
	// 10cm above the eyes for top colision 
	Ogre::Vector3 headTop = mPlayerNode->getPosition();
	headTop.y += eyeHeight + 0.1f;

	Ogre::Vector3 rays[5] = 
	{
		leftEye,
		rightEye,
		chest,
		waist,
		headTop,
	};

	for(int i = 0; i < 5; i++)
	{
		if(mCollisionTools->raycastFromPoint(rays[i], normal, result,myObject,distToColl))
		{
			if(distToColl <= 3.0f)
			{
				//mPlayerNode->translate(Ogre::Vector3(0, direction.y, 0));
				return true;
			}
		}
	}

	return false;
	
}

//todo return clearence so we don't fall through the floor as often
bool Player::OnGround()
{
	Ogre::Vector3 result;
	//Vector3 myResult(0, 0, 0);
	Ogre::Entity* myObject = NULL;
	float distToColl = 0.0f;

	Ogre::Vector3 colisionOrigin = mPlayerNode->getPosition();
	colisionOrigin.y += 0.2f;

	if(mCollisionTools->raycastFromPoint(colisionOrigin, Ogre::Vector3::NEGATIVE_UNIT_Y, result,myObject,distToColl))
	{
		if(distToColl <= 0.25f)
		{
			return true;
		}
	}
	return false;
}

void Player::jump()
{
	if(mJumpDuration != 0 && !mDoubleJumping)
	{
  		mDoubleJumping = true;
		mJumpDuration = 0;
		//mJumpVector.y += 2.0f;
	}

	if(mJumpDuration == 0)
	{
		mJumping = true;
	}
	
}


void Player::changeHeight(bool change)
{
	mChangeHeight = change;
}
