#pragma once
#include "stdafx.h"

class MouseCursor
{
private: 
	
	float m_Sensitivity;

	Ogre::SceneNode *m_Cursor;
	Ogre::SceneNode *m_RotationNode;
public: 
	MouseCursor(Ogre::SceneManager *sceneMgr, Ogre::SceneNode* eyeNode);
	void mouseInput(Ogre::Vector2 input);
};
