#pragma once
#include "stdafx.h"

class Player
{
public:
	Player(Ogre::SceneNode* playerNode);
	~Player(void);
	void addInput(Ogre::Vector3 input);
	void processMovement(Ogre::Real timeSinceLastFrame);
	void jump();

private:
	Ogre::SceneNode* mPlayerNode;
	Ogre::Vector3 mInput;
	bool mJumping;

	Ogre::Vector3 mJumpVector;
	Ogre::Real mJumpDuration;
};

