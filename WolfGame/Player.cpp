#include "stdafx.h"
#include "Player.h"


Player::Player(Ogre::SceneNode* playerNode)
{
	mPlayerNode = playerNode;
	mJumping = false;
	mInput = Ogre::Vector3::ZERO;
	mJumpDuration=0;
}


Player::~Player(void)
{
}

void Player::addInput(Ogre::Vector3 input)
{
	mInput += input;
}

void Player::processMovement(Ogre::Real timeSinceLastFrame)
{
	if( mJumping)
	{
		if(mJumpDuration == 0)
		{
			mJumpVector = mInput ;
		}
		mJumpDuration += timeSinceLastFrame;

		if(mJumpDuration > 1)
		{
			mJumpDuration = 0;
			mJumping = false;
		}
	}

	Ogre::Vector3 direction;
	if(mJumping)
	{
		// tweak the jump speed Number within the sin must equal 2xPi over the total jump duration
		// or you do not end up where you started at the end of a flat jump
		Ogre::Real multiplier = Ogre::Math::Sin(mJumpDuration * 2 * Ogre::Math::PI) * 1.5;
		direction = mPlayerNode->getOrientation() * mJumpVector + Ogre::Vector3::UNIT_Y *multiplier ;
	}
	else
	{
		direction = mPlayerNode->getOrientation() * mInput;
	}

	//normalisation sounds like a great idea but it plays hell with jumping
	direction = direction *(timeSinceLastFrame * 4);
	mPlayerNode->translate(direction);
}


void Player::jump()
{
	if(!mJumping)
	{
		mJumping = true;
	}
}
