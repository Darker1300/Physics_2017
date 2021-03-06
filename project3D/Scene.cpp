#include "Scene.h"

#include "DEBUG_NEW_LEAK_DETECT.h"

#include <glm\glm.hpp>

#include <Gizmos.h>
#include "Body.h"
#include "Collision.h"
#include "Plane.h"
#include "Sphere.h"
#include "AABB.h"

using namespace Physics;

Scene::Scene()
	: m_objects()
	, m_gravity(0, -9.8f, 0)
{
	Start();
}

Scene::~Scene()
{
	Clear();
}

void Physics::Scene::Start()
{
	{ // Ball A
		Body* ballBody = new Body();
		ballBody->SetMass(50);
		ballBody->SetShape(new Sphere(0.6f));
		ballBody->SetPosition({ 1,2,3 });
		AddBody(ballBody);
	}

	{ // Ball B
		Body* ballBodyB = new Body();
		ballBodyB->SetMass(50);
		ballBodyB->SetShape(new Sphere(0.6f));
		ballBodyB->SetPosition({ 1,5,3 });
		AddBody(ballBodyB);
	}

	{ // Box Small
		Body* boxBodyB = new Body();
		boxBodyB->SetMass(75);
		boxBodyB->SetShape(new AABB(0.5f));
		boxBodyB->SetPosition({ 3.5f,9,0.5f });
		boxBodyB->SetPosition({ 1.3,3,3 });
		AddBody(boxBodyB);
	}

	{ // Box Large
		Body* boxBodyFloor = new Body();
		boxBodyFloor->SetMass(FLT_MAX);
		boxBodyFloor->SetShape(new AABB(10));
		boxBodyFloor->SetIsStatic(true);
		boxBodyFloor->SetPosition({ 0,-4,0 });
		AddBody(boxBodyFloor);
	}

	{ // Floor Plane
		Body* planeBody = new Body();
		planeBody->SetMass(FLT_MAX);
		planeBody->SetShape(new Plane(glm::normalize(glm::vec3(0, 1, 0.4f))));
		planeBody->SetIsStatic(true);
		AddBody(planeBody);
	}

	{ // Directional Planes
		static const glm::vec3 dirs[4]{ {1, 0, 0}, { -1,0,0 }, { 0,0,1 }, { 0,0,-1 } };
		static const float dist = 10.0f;
		for (int i = 0; i < 4; i++)
		{
			Body* planeBody = new Body();
			planeBody->SetMass(FLT_MAX);
			Plane* planeShape = new Plane(dirs[i]);
			planeBody->SetShape(planeShape);
			planeBody->SetIsStatic(true);
			planeBody->SetPosition(-dirs[i] * dist);
			AddBody(planeBody);
		}
	}
}

void Scene::Update(float _deltaTime)
{
	// Gravity & Update Non-Static
	for (Body* obj : m_objects) {
		if (!obj->GetIsStatic()) {
			obj->AddAcceleration(m_gravity);
			obj->Update(_deltaTime);
		}
	}

	Collision::CollisionInfo collisionInfo;
	for (auto objIter1 = m_objects.begin();
		objIter1 != m_objects.end();
		objIter1++) {
		for (auto objIter2 = std::next(objIter1);
			objIter2 != m_objects.end();
			objIter2++)
		{
			// Test Collision
			if (Collision::TestCollision(*objIter1, *objIter2, collisionInfo)) {
				// Handle Collision
				Collision::ResolveCollision(*objIter1, *objIter2, collisionInfo);
			}
		}
	}
}

void Physics::Scene::DrawGizmos() const
{
	// draw a transform at world origin
	aie::Gizmos::addTransform(glm::mat4(1));

	// draw a simple grid at world origin
	glm::vec4 white(0.4f);
	glm::vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		aie::Gizmos::addLine(glm::vec3(-10 + i, 0, 10),
			glm::vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		aie::Gizmos::addLine(glm::vec3(10, 0, -10 + i),
			glm::vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	// Select color for gizmo
	for each (Physics::Body* obj in m_objects) {
		static glm::vec4 col = glm::vec4(0, 0, 0, 0.25f);

		switch (obj->GetShape()->GetType())
		{
		case Physics::ShapeType::Point:
			col = { 0, 1, 0, 1 };
			break;
		case Physics::ShapeType::Plane:
			col = { 1, 0, 0, 0.1f };
			break;
		case Physics::ShapeType::Sphere:
			col = { 0, 0, 1, 0.9f };
			break;
		case Physics::ShapeType::AABB:
			col = { 1, 204 / 255.0f,0 , 0.9f };
			break;
		default:
			col = { 0, 0, 0, 0.25f };
			break;
		}
		// Draw Gizmo
		obj->DrawGizmo(col);
	}
}

void Physics::Scene::Clear()
{
	for (Body* obj : m_objects) {
		delete obj;
	}
	m_objects.clear();
}

void Physics::Scene::AddBody(Body * _body)
{
	m_objects.push_back(_body);
}
