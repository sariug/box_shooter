#include <vector>
#include "box.h"

#include "Window.h"
#include "Camera.h"
#include "btBulletDynamicsCommon.h"

GLfloat deltaTime = .0f;
GLfloat lastTime = .0f;

Window *mainwindow = new Window;
Camera *camera;
std::vector<Box *> shapes;
/*  Bullet World*/
btDefaultCollisionConfiguration *collisionConfiguration;
btDiscreteDynamicsWorld *dynamicsWorld;
btBroadphaseInterface *overlappingPairCache;
btSequentialImpulseConstraintSolver *solver;
btCollisionDispatcher *dispatcher;
//make sure to re-use collision shapes among rigid bodies whenever possible!
btAlignedObjectArray<btCollisionShape *> collisionShapes;
void check_collisions();
void process_keys();
void init_physics();
void init_visual_shapes();
void init_collision_shapes();
void delete_physics();
int main()
{

	mainwindow->initialize();

	init_physics();
	init_visual_shapes();
	init_collision_shapes();

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	camera = new Camera(glm::vec3(-20.0f, 20.0f, -20.0f), glm::vec3(.0f, 1.0f, .0f), 45.0f, -30.0f, 5.0f, .5f);

	glm::mat4 projection = glm::perspective(45.0f, mainwindow->getBufferWidth() / mainwindow->getBufferHeight(), 0.01f, 100.0f);
	// Loop until it is closed
	while (!mainwindow->shouldWindowClose())
	{
		check_collisions();

		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		lastTime = now;
		// Get and handle user inputs
		glfwPollEvents();
		process_keys();
		camera->mouseControl(glm::vec2(mainwindow->getXchange(), mainwindow->getYchange()));
		// Clear window
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		for (auto &s : shapes)
			s->draw(projection, camera->calculateViewMatrix());

		mainwindow->swapBuffers();
	}
	// clean
	delete_physics();
	return 0;
}
void check_collisions()
{
	dynamicsWorld->stepSimulation(deltaTime, 10);

	//print positions of all objects
	for (int j = dynamicsWorld->getNumCollisionObjects() - 1; j >= 0; j--)
	{
		btCollisionObject *obj = dynamicsWorld->getCollisionObjectArray()[j];
		btRigidBody *body = btRigidBody::upcast(obj);
		btTransform trans;
		if (body && body->getMotionState())
		{
			body->getMotionState()->getWorldTransform(trans);
		}
		else
		{
			trans = obj->getWorldTransform();
		}
		trans.getOpenGLMatrix(glm::value_ptr(shapes.at(j)->set_transformation()));
		//	printf("world pos object %d = %f,%f,%f\n", j, float(trans.getOrigin().getX()), float(trans.getOrigin().getY()), float(trans.getOrigin().getZ()));
		if (float(trans.getOrigin().getY()) > -100.0)
			continue;

		// Delete from bullet
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		dynamicsWorld->removeCollisionObject(obj);
		delete obj;
		btCollisionShape *shape = collisionShapes[j];
		collisionShapes[j] = 0;
		//delete shape;
		// Delete from my list
		shapes.erase(std::begin(shapes) + j);
	}
}

void process_keys()
{
	int posx, posy;
	bool mouse_clicked;
	std::tie(posx, posy, mouse_clicked) = mainwindow->mouse_feedback();
	if (mouse_clicked)
	{

		shapes.push_back(new Box(-20.0f, 20.0f, -20.0f));
		auto origin = shapes.back()->getOrigin();
		btCollisionShape *colShape = new btBoxShape(btVector3(1, 1, 1));
		collisionShapes.push_back(colShape);

		/// Create Dynamic Objects
		btTransform startTransform;
		startTransform.setIdentity();

		btScalar mass(1.f);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			colShape->calculateLocalInertia(mass, localInertia);

		startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
		btRigidBody *body = new btRigidBody(rbInfo);
		btVector3 direction;
		auto camera_dir = camera->get_front();

		
		std::cout << camera_dir.x << " " << camera_dir.y << " " << camera_dir.z << std::endl;
		glm::vec3 mouse_dir = glm::normalize(glm::vec3(posx - 400, 300 - posy, 0));

		direction.setX(camera_dir.x);
		direction.setY(camera_dir.y);
		direction.setZ(camera_dir.z);
		body->setLinearVelocity(direction * 25);
		dynamicsWorld->addRigidBody(body);
	}
}

void init_visual_shapes()
{
	// Items
	// Ground
	Box *ground = new Box(2, -2, 2);
	ground->scaleBox(glm::vec3(10.0f, 1.0f, 10.0f));

	shapes.push_back(ground);
	// Other
	for (int i = 0; i < 5; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			for (int k = 0; k < 5; k++)
			{
				shapes.push_back(new Box(i, j, k));
			}
		}
	}
}
void init_collision_shapes()
{
	///create a few basic rigid bodies
	{
		btCollisionShape *groundShape = new btBoxShape(btVector3(btScalar(10.), btScalar(1.), btScalar(10.)));

		collisionShapes.push_back(groundShape);

		btTransform groundTransform;
		groundTransform.setIdentity();
		auto origin = shapes[0]->getOrigin();
		groundTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

		btScalar mass(0.);

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.f);

		btVector3 localInertia(0, 0, 0);
		if (isDynamic)
			groundShape->calculateLocalInertia(mass, localInertia);

		//using motionstate is optional, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState *myMotionState = new btDefaultMotionState(groundTransform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, groundShape, localInertia);
		btRigidBody *body = new btRigidBody(rbInfo);

		//add the body to the dynamics world
		dynamicsWorld->addRigidBody(body);
	}

	{
		//create a dynamic rigidbody

		for (auto shape : shapes)
		{
			if (shape->getOrigin().y < 0) // Skip the ground.
				continue;
			auto origin = shape->getOrigin();
			btCollisionShape *colShape = new btBoxShape(btVector3(1, 1, 1));
			collisionShapes.push_back(colShape);

			/// Create Dynamic Objects
			btTransform startTransform;
			startTransform.setIdentity();

			btScalar mass(1.f);

			//rigidbody is dynamic if and only if mass is non zero, otherwise static
			bool isDynamic = (mass != 0.f);

			btVector3 localInertia(0, 0, 0);
			if (isDynamic)
				colShape->calculateLocalInertia(mass, localInertia);

			startTransform.setOrigin(btVector3(origin.x, origin.y, origin.z));

			//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
			btDefaultMotionState *myMotionState = new btDefaultMotionState(startTransform);
			btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, myMotionState, colShape, localInertia);
			btRigidBody *body = new btRigidBody(rbInfo);

			dynamicsWorld->addRigidBody(body);
		}
	}
}

void init_physics()
{
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	overlappingPairCache = new btDbvtBroadphase();
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
}
void delete_physics()
{
	//delete dynamics world
	delete dynamicsWorld;

	//delete solver
	delete solver;

	//delete broadphase
	delete overlappingPairCache;

	//delete dispatcher
	delete dispatcher;

	delete collisionConfiguration;

	//next line is optional: it will be cleared by the destructor when the array goes out of scope
	collisionShapes.clear();
}