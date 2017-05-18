#include "Scene.h"

#include "PhysicsObject.h"
#include "Collision.h"

Scene::Scene()
	: m_objects()
	, m_gravity(0, -9, 0)
{
}


Scene::~Scene()
{
}

void Scene::Update(float _deltaTime)
{
	// Gravity & Update Non-Static
	for (PhysicsObject* obj : m_objects) {
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
			bool collided = Collision::TestCollision(*objIter1, *objIter2, collisionInfo);
			if (collided) {
				// Handle Collision
				Collision::ResolveCollision(*objIter1, *objIter2, collisionInfo);
			}
		}
	}
}
