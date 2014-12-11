/*
 * Triangle.h
 *
 *  Created on: 27-Mar-2011
 *      Author: manoj
 */

#include <btBulletDynamicsCommon.h>

class Plane{

private:

	static short indices [];
	static float vertices [];

	btDiscreteDynamicsWorld* world;
	btCollisionShape* shape;
	btDefaultMotionState* motionState;
	btRigidBody* body;

public:
	Plane(btDiscreteDynamicsWorld* dynamicsWorld, btCollisionShape* shape, float x, float y ,float z);
	~Plane();
	void draw(float x, float y, float z);
};

