#include "Scene.h"

#include "DEBUG_NEW_LEAK_DETECT.h"

#include "Body.h"
#include "Collision.h"
#include "Plane.h"
#include "Sphere.h"
#include "Application3D.h"

using namespace Physics;

Scene::Scene(Application3D* _app)
	: m_objects()
	, m_gravity(0, -9.8f, 0)
	, m_app(_app)
{
	Start();
}

Scene::~Scene()
{
	Clear();
}

void Physics::Scene::Start()
{
	Body* planeBody = new Body();
	planeBody->SetMass(100000.0f);
	planeBody->SetShape(new Plane());
	planeBody->SetIsStatic(true);

	AddBody(planeBody);

	Body* ballBody = new Body(50);
	ballBody->SetShape(new Sphere(0.6f));
	ballBody->SetPosition({ 1,2,3 });
	AddBody(ballBody);
}

void Scene::Update(float _deltaTime)
{
	// Gravity & Update Non-Static
	for (Body* obj : m_objects) {
		if (!obj->GetIsStatic()) {
			obj->AddAcceleration(m_gravity);
			obj->Update(_deltaTime);
		}

		// Move plane
		if (obj->GetShape()->GetType() == ShapeType::Plane) {
			auto pos = obj->GetPosition();
			pos.y = (-1.0f + (1.0f - -1.0f) * sinf(m_app->getTime()));
			obj->SetPosition(pos);//  m_normal.x = (-0.15f + (0.25f - -0.15f) * sinf(m_app->getTime()));
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
			bool collided = Collision::TestCollision(*objIter1, *objIter2, collisionInfo);
			if (collided) {
				// Handle Collision
				Collision::ResolveCollision(*objIter1, *objIter2, collisionInfo);
			}
		}
	}
}

void Physics::Scene::DrawGizmos() const
{
	for each (Physics::Body* obj in m_objects) {
		static glm::vec4 col = glm::vec4(0, 0, 0, 0.25f);

		switch (obj->GetShape()->GetType())
		{
		case Physics::ShapeType::Point:
			col = { 0, 1, 0, 0.25f };
			break;
		case Physics::ShapeType::Plane:
			col = { 1, 0, 0, 0.25f };
			break;
		case Physics::ShapeType::Sphere:
			col = { 0, 0, 1, 0.25f };
			break;
		default:
			col = { 0, 0, 0, 0.25f };
			break;
		}

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
