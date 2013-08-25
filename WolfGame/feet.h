#pragma once
class Feet
{
public:
	Feet(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* player);
	~Feet(void);
	void move(Ogre::Vector3 movement);
	Ogre::Vector3 turn(Ogre::Radian turn);
	void setVisible(bool visible);

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

	bool mLeftFootActive;
	bool mRightFootActive;
};

