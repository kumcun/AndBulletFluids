#include "Plane.h"

short Plane::indices [] = {
		0, 1, 2,
		2, 3, 0
};
float Plane::vertices []= {
		100.0f, 0.0f, 100.0f,
		-100.0f, 0.0f, 100.0f,
		-100.0f, 0.0f, -100.0f,
		100.0f, 0.0f, -100.0f,
};


Plane::Plane(btDiscreteDynamicsWorld* dynamicsWorld,btCollisionShape* shape1, float x, float y ,float z){
	shape = shape1;

	btVector3 localInertia(0,0,0);
	shape->calculateLocalInertia(0,localInertia);

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin( btVector3(0,-10,0) );
	groundTransform.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI*0.045));

	motionState = new btDefaultMotionState(groundTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(0.f,motionState,shape,localInertia);
	body = new btRigidBody(rbInfo);


	this->world = dynamicsWorld;
	this->world->addRigidBody(body);
}

Plane::~Plane(){
	world->removeRigidBody(body);
	delete motionState;
	delete body;
	delete shape;
}

void Plane::draw(float x, float y, float z){
/*
	glPushMatrix();
	btTransform trans;

	//world->removeRigidBody(body);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1), btVector3(0,y,0)));
	body->setMotionState(motionState);
	motionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));

	//body->setAngularVelocity(btVector3(x,0,z));
	body->getMotionState()->getWorldTransform(trans);

	trans.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI*0.01*x));

	btTransform groundTransform;
	groundTransform.setIdentity();
	groundTransform.setOrigin( btVector3(0,-10,0) );
	groundTransform.setRotation(btQuaternion(btVector3(0,0,1),SIMD_PI*0.01 * x));
	motionState = new btDefaultMotionState(groundTransform);
	body->setMotionState(motionState);

	float matrix[16] ;
	groundTransform.getOpenGLMatrix(matrix);
	glMultMatrixf (matrix);
	glFrontFace(GL_CW);

	//vertices[1] = x;
	//vertices[10] = x;
	//vertices[7] = z;
	//vertices[13] = z;
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glEnableClientState(GL_VERTEX_ARRAY);

	glColor4f(0.87f,0.72f,0.53f,255.0f);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, &indices[0]);

	glDisableClientState(GL_VERTEX_ARRAY);
	glPopMatrix();

//*/
}
