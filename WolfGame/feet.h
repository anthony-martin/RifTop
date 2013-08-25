#pragma once
class Feet
{
public:
	Feet(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* player);
	~Feet(void);
	void move(Ogre::Vector3 movement);
	void setVisible(bool visible);

private:
	void moveRightFoot(Ogre::Vector3 movement);
	void moveLeftFoot(Ogre::Vector3 movement);

	Ogre::SceneNode* leftFoot;
	Ogre::SceneNode* rightFoot;
	Ogre::SceneNode* mBody;
	bool mLeftFootActive;
	bool mRightFootActive;
};

