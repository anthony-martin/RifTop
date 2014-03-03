#pragma once
#include "stdafx.h"

class MouseCursor
{
private: 
	
	float m_Sensitivity;

	Ogre::SceneNode *m_Cursor;
	Ogre::SceneNode *m_RotationNode;
public: 
	MouseCursor(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* eyeNode, Ogre::SceneNode* orientationNode);
	void mouseInput(Ogre::Vector2 input);
	Ogre::Vector3 GetPosition();

	void SetVisible(bool visible);
};
