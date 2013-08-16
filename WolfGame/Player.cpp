#include "stdafx.h"
#include "Player.h"


Player::Player(void)
{
	//btTransform startTransform;
	//startTransform.setIdentity ();
	////startTransform.setOrigin (btVector3(0.0, 4.0, 0.0));
	//startTransform.setOrigin (btVector3(0,1.75,0));

	//btVector3 worldMin(-1000,-1000,-1000);
	//btVector3 worldMax(1000,1000,1000);
	//btAxisSweep3* sweepBP = new btAxisSweep3(worldMin,worldMax);

	//m_ghostObject = new btPairCachingGhostObject();
	//m_ghostObject->setWorldTransform(startTransform);
	//sweepBP->getOverlappingPairCache()->setInternalGhostPairCallback(new btGhostPairCallback());
	//btScalar characterHeight=1.75;
	//btScalar characterWidth =1.75;
	//btConvexShape* capsule = new btCapsuleShape(characterWidth,characterHeight);
	//
	////Ogre version of the capsule
	////OgreBulletCollisions::CapsuleCollisionShape* OgreCapsule = new OgreBulletCollisions::CapsuleCollisionShape(capsule);
	//m_ghostObject->setCollisionShape (capsule);
	//m_ghostObject->setCollisionFlags (btCollisionObject::CF_CHARACTER_OBJECT);

	//btScalar stepHeight = btScalar(0.35);
	//m_character = new btKinematicCharacterController (m_ghostObject,capsule,stepHeight);
	//m_ghostObject->getWorldTransform().
}


Player::~Player(void)
{
}
