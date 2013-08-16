#pragma once
#include "stdafx.h"

class Player
{
public:
	Player(void);
	~Player(void);

private:
	Ogre::SceneNode* mPlayerNode;
	Ogre::Vector3 mInput;
	btKinematicCharacterController* m_character;
	btPairCachingGhostObject * m_ghostObject;
};

