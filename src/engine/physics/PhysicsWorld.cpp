#include "PhysicsWorld.hpp"

namespace lei3d
{
    PhysicsWorld::PhysicsWorld() {
    }

    void PhysicsWorld::Create()
    {
        //This doesn't work and I need to go to bed so fuck it.
        
        // this is just the default setup, we can make this fancier if we want
        collisionConfiguration = std::make_unique<btDefaultCollisionConfiguration>();
        dispatcher = std::make_unique<btCollisionDispatcher>(collisionConfiguration.get());
        // good general purpose broadphase, we cna also use btAxis3Sweep
        overlappingPairCache = std::make_unique<btDbvtBroadphase>();
        solver = std::make_unique<btSequentialImpulseConstraintSolver>();
        dynamicsWorld = std::make_unique<btDiscreteDynamicsWorld>(dispatcher.get(), overlappingPairCache.get(), solver.get(), collisionConfiguration.get());

        dynamicsWorld->setGravity(btVector3(0, -1, 0));

        //CHARACTER--------------------
        //std::unique_ptr<btCollisionShape> character = std::make_unique<btCapsuleShape>(btScalar{1.0f}, btScalar{3.0f});
        btCollisionShape* character = new btCapsuleShape(btScalar{1.0f}, btScalar{3.0f});
        btTransform startTransform;
        startTransform.setIdentity();

        btScalar mass{1.f};
        btVector3 localInertia{0.0f, 0.0f, 0.0f};
        character->calculateLocalInertia(mass, localInertia);
        startTransform.setOrigin(btVector3{3.0f, 50.0f, 0.0f});
        
        //THIS IS A MEMORY LEAK, FIX!!
        btDefaultMotionState* charMotionState = new btDefaultMotionState(startTransform);
        btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, charMotionState, character, localInertia);
        btRigidBody* characterBody = new btRigidBody(rbInfo);
        dynamicsWorld->addRigidBody(characterBody);

        collisionShapes.push_back(character);

        //GROUND--------------------
        //std::unique_ptr<btCollisionShape> groundShape = std::make_unique<btBoxShape>(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
        btCollisionShape* groundShape = new btBoxShape(btVector3(btScalar(50.), btScalar(50.), btScalar(50.)));
        
        btTransform groundTransform;
        groundTransform.setIdentity();
        groundTransform.setOrigin(btVector3(0, -70, 0));

        btScalar floorMass = 0.0f;
        btVector3 floorLocalInertia{0.0f, 0.0f, 0.0f}; // list initialization makes my brain happy with structs. I'm sorry for the sudden switch haha
        
        //THIS IS A MEMORY LEAK, FIX!!
        btDefaultMotionState* myMotionState = new btDefaultMotionState(groundTransform);
        btRigidBody::btRigidBodyConstructionInfo rbFloorInfo{floorMass, myMotionState, groundShape, floorLocalInertia};
        btRigidBody* floorBody = new btRigidBody(rbFloorInfo);
        dynamicsWorld->addRigidBody(floorBody);

        collisionShapes.push_back(groundShape);
    }

    void PhysicsWorld::Step(float deltaTime)
    {
        dynamicsWorld->stepSimulation(deltaTime, 10);

        // Move every object
		for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
		{
			btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[j];
			btRigidBody* body = btRigidBody::upcast(obj);
			btTransform trans;
			if (body && body->getMotionState())
			{
				body->getMotionState()->getWorldTransform(trans);
			}
			else
			{
				trans = obj->getWorldTransform();
			}
		}
    }

    glm::vec3 PhysicsWorld::GetFirstColliderPosition()
    {
        btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[0];
        btRigidBody* body = btRigidBody::upcast(obj);
        btTransform trans;
        if (body && body->getMotionState())
        {
            body->getMotionState()->getWorldTransform(trans);
        }
        else
        {
            trans = obj->getWorldTransform();
        }
        glm::vec3 position = glm::vec3(float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
        return position;
    }


}