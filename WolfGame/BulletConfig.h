#pragma once
#include "stdafx.h"
#include "CameraController.h"

class BulletConfig
{
public:
	BulletConfig(Ogre::SceneManager* sceneMgr,
				Ogre::AxisAlignedBox bounds);
	~BulletConfig(void);
	void CreateCube(CameraController::Controller * mController);
	void StepPhysics(Ogre::Real elapsedTime);

private:
    Ogre::SceneManager* mSceneMgr;
    OgreBulletDynamics::DynamicsWorld *mWorld;   // OgreBullet World
    OgreBulletCollisions::DebugDrawer *debugDrawer;
    int mNumEntitiesInstanced;
    std::deque<OgreBulletDynamics::RigidBody *>         mBodies;
    std::deque<OgreBulletCollisions::CollisionShape *>  mShapes;
	float mMoveSpeed;
};


