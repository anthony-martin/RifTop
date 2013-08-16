#include "stdafx.h"
#include "BulletConfig.h"


BulletConfig::BulletConfig(Ogre::SceneManager* sceneMgr,
							Ogre::AxisAlignedBox bounds)
{
	mMoveSpeed = 50;   // defined in ExampleFrameListener
    mNumEntitiesInstanced = 0; // how many shapes are created
    mSceneMgr = sceneMgr;
    // Start Bullet
	mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, 3*Ogre::Vector3::NEGATIVE_UNIT_Y);
    // add Debug info display tool
    debugDrawer = new OgreBulletCollisions::DebugDrawer();
    debugDrawer->setDrawWireframe(true);   // we want to see the Bullet containers
    mWorld->setDebugDrawer(debugDrawer);
    mWorld->setShowDebugShapes(true);      // enable it if you want to see the Bullet containers
    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    node->attachObject(static_cast <Ogre::SimpleRenderable *> (debugDrawer));

	OgreBulletCollisions::CollisionShape *Shape;
          Shape = new OgreBulletCollisions::StaticPlaneCollisionShape(Ogre::Vector3(0,1,0), 0); // (normal vector, distance)
          OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody(
                   "BasePlane",
                   mWorld);
          defaultPlaneBody->setStaticShape(Shape, 0.1, 0.8); // (shape, restitution, friction)
		  
		  mShapes.push_back(Shape);
          mBodies.push_back(defaultPlaneBody);


		  OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(Ogre::Vector3(4,4,4));
			// and the Bullet rigid body
            OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
                "defaultBoxRigid" + Ogre::StringConverter::toString(mNumEntitiesInstanced),
                mWorld);
            defaultBody->setShape(   node,
                            sceneBoxShape,
                            1.0f,         // dynamic body restitution
                            1.0f,         // dynamic body friction
                            100.0f,          // dynamic bodymass
							Ogre::Vector3::ZERO,      // starting position of the box
                            Ogre::Quaternion(0,0,0,1));// orientation of the box
            mNumEntitiesInstanced++;            
			defaultBody->setKinematicObject(true);
                // push the created objects to the deques
            mShapes.push_back(sceneBoxShape);
            mBodies.push_back(defaultBody);            
}


BulletConfig::~BulletConfig(void)
{
	// OgreBullet physic delete - RigidBodies
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody)
    {   
        delete *itBody;
        ++itBody;
    }   
    // OgreBullet physic delete - Shapes
    std::deque<OgreBulletCollisions::CollisionShape *>::iterator itShape = mShapes.begin();
    while (mShapes.end() != itShape)
    {   
        delete *itShape;
        ++itShape;
    }
    mBodies.clear();
    mShapes.clear();
    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;
}

void BulletConfig::CreateCube(CameraController::Controller * mController)
{
	Ogre::Vector3 size = Ogre::Vector3::ZERO;   // size of the box
             // starting position of the box
			 Ogre::Vector3 position = (mController->mBodyRotationNode->getPosition() + mController->mCamera->getDerivedDirection().normalisedCopy() * 5);
             // create an ordinary, Ogre mesh with texture
              Ogre::Entity *entity = mSceneMgr->createEntity(
                   "Box" + Ogre::StringConverter::toString(mNumEntitiesInstanced),
                   "cube.mesh");            
             entity->setCastShadows(true);
             // we need the bounding box of the box to be able to set the size of the Bullet-box
             Ogre::AxisAlignedBox boundingB = entity->getBoundingBox();
             size = boundingB.getSize(); size /= 2.0f; // only the half needed
             size *= 0.96f;   // Bullet margin is a bit bigger so we need a smaller size
                               // (Bullet 2.76 Physics SDK Manual page 18)
             //entity->setMaterialName("Examples/BumpyMetal");
             Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
             node->attachObject(entity);
             node->scale(0.01f, 0.01f, 0.01f);   // the cube is too big for us
             size *= 0.01f;                  // don't forget to scale down the Bullet-box too
             // after that create the Bullet shape with the calculated size
             OgreBulletCollisions::BoxCollisionShape *sceneBoxShape = new OgreBulletCollisions::BoxCollisionShape(size);
			 // and the Bullet rigid body
             OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody(
                   "defaultBoxRigid" + Ogre::StringConverter::toString(mNumEntitiesInstanced),
                   mWorld);
             defaultBody->setShape(   node,
                               sceneBoxShape,
                               0.6f,         // dynamic body restitution
                               0.6f,         // dynamic body friction
                               1.0f,          // dynamic bodymass
                               position,      // starting position of the box
                               Ogre::Quaternion(0,0,0,1));// orientation of the box
             mNumEntitiesInstanced++;            
             defaultBody->setLinearVelocity(
                      mController->mCamera->getDerivedDirection().normalisedCopy() * 7.0f ); // shooting speed
                   // push the created objects to the deques
             mShapes.push_back(sceneBoxShape);
             mBodies.push_back(defaultBody);            
             //mTimeUntilNextToggle = 0.5;
}


void BulletConfig::StepPhysics(Ogre::Real elapsedTime)
{
	mWorld->stepSimulation(elapsedTime);
}