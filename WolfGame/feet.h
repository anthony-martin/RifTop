#pragma once
#include "CollisionTools.h"

class Feet
{
public:
	Feet(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* player);
	~Feet(void);
	void move(Ogre::Vector3 movement);
	Ogre::Vector3 turn(Ogre::Radian turn);
	void setVisible(bool visible);
	void swapFeet();
	bool onGround();
	float distanceToGround(float travel);

private:
	void moveRightFoot(Ogre::Vector3 movement);
	void moveLeftFoot(Ogre::Vector3 movement);
	void UpdateMaterial();

	Ogre::SceneNode* leftFoot;
	Ogre::SceneNode* rightFoot;
	Ogre::SceneNode* mBody;
	Ogre::Entity* leftMesh;
	Ogre::Entity* rightMesh;
	Ogre::MaterialPtr upMaterial;
	Ogre::MaterialPtr downMaterial;
	MOC::CollisionTools* mCollisionTools;

	bool mLeftFootActive;
	bool mRightFootActive;
};

