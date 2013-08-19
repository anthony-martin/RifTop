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

	if(!mJumping && onGround)
	{
		mJumpDuration = 0;
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
		// tweak the jump speed 
		// Numbers within the sin must equal 2xPi over the total jump duration
		// or you do not end up where you started at the end of a flat jump
		//Ogre::Real curve = Ogre::Math::Sin(mJumpDuration * 2.6666666666f * Ogre::Math::PI);
		// take into account the difference between crouching and standing height
		//float jumpHeight =  2.0f + (maxHeight - mEyeNode->getPosition().y)/1.5;
		
		//Ogre::Real multiplier =  curve * jumpHeight;
		direction = mEyeNode->getOrientation() * mJumpVector ;//+ Ogre::Vector3::UNIT_Y *multiplier ;
	}
	else
	{
		direction = mEyeNode->getOrientation() * mInput;
	}

	//normalisation sounds like a great idea but it plays hell with jumping
	direction = direction *(timeSinceLastFrame * 4);

	Ogre::Vector3 result;
	Ogre::Entity* myObject = NULL;
	float distToColl = 0.0f;

	Ogre::Vector3 colisionOrigin = mPlayerNode->getPosition();
	// 10cm above the eyes for top colision 
	colisionOrigin.y += mEyeNode->getPosition().y + 0.1f;

    Ogre::Vector3 colisionNormal = Ogre::Vector3( direction);
	colisionNormal.y = 0;

	if(checkHorizontalColisions(colisionNormal))
	{
		mPlayerNode->translate(Ogre::Vector3(0, direction.y, 0));
		return;
	}

	mPlayerNode->translate(direction);
}


bool Player::checkHorizontalColisions( Ogre::Vector3 normal)
{
	Ogre::Vector3 result;
	Ogre::Entity* myObject = NULL;
	float distToColl = -1.0f;

	float eyeHeight = mPlayerNode->getPosition().y + mEyeNode->getPosition().y;
	
	//5 rays
	Ogre::Vector3 leftEye = mPlayerNode->getPosition();
	leftEye.y = eyeHeight;
	leftEye.z = -0.1f;
	Ogre::Vector3 rightEye= mPlayerNode->getPosition();
	rightEye.y = eyeHeight;
	rightEye.z = 0.1f;
	Ogre::Vector3 chest= mPlayerNode->getPosition();
	chest.y = .75 * eyeHeight;
	Ogre::Vector3 waist= mPlayerNode->getPosition();
	waist.y = .5f *eyeHeight ;
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
			if(distToColl <= 6.0f)
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
	if(!mJumping && mOnGround)
	{
		mJumping = true;
	}
}

void Player::stopJump()
{
	mJumping = false;
}


void Player::changeHeight(bool change)
{
	mChangeHeight = change;
}
