#include "stdafx.h"
#include "SceneManagerExtensions.h"

using namespace Ogre;

void  SceneManagerExtensions::DestroyAllAttachedMovableObjects( SceneNode* i_pSceneNode )
{
	if ( i_pSceneNode )
	{
		// Destroy all the attached objects
		SceneNode::ObjectIterator itObject = i_pSceneNode->getAttachedObjectIterator();

		while ( itObject.hasMoreElements() )
		{
			MovableObject* pObject = static_cast<MovableObject*>(itObject.getNext());
			i_pSceneNode->getCreator()->destroyMovableObject( pObject );
		}

		// Recurse to child SceneNodes
		SceneNode::ChildNodeIterator itChild = i_pSceneNode->getChildIterator();

		while ( itChild.hasMoreElements() )
		{
			SceneNode* pChildNode = static_cast<SceneNode*>(itChild.getNext());
			DestroyAllAttachedMovableObjects( pChildNode );
		}
	}
}
