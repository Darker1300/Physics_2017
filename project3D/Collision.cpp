#include "Collision.h"

#include <glm\glm.hpp>

#include "Body.h"
#include "Sphere.h"
#include "Plane.h"

using namespace Physics;

bool Collision::TestCollision(const Body * _obj1, const Body * _obj2, CollisionInfo & _info)
{
	auto index = (
		(int)(_obj1->GetShape()->GetType())
		* (int)(ShapeType::Size)
		+ (int)(_obj2->GetShape()->GetType()));

	if (CollisionMatrix[index] != nullptr)
		return CollisionMatrix[index](_obj1, _obj2, _info);

	return _info.collided;
}

void Collision::ResolveCollision(Body * _obj1, Body * _obj2, CollisionInfo & _info)
{
	if (_info.collided) {
		HandleSeperation(_obj1, _obj2, _info);
		HandleVelocityChange(_obj1, _obj2, _info);
	}
}

void Collision::HandleSeperation(Body * _obj1, Body * _obj2, CollisionInfo & _info)
{
	float totalMass = _obj1->GetMass() + _obj2->GetMass();
	float obj1Offset = _info.penetration * (_obj2->GetMass() / totalMass);
	float obj2Offset = _info.penetration * (_obj1->GetMass() / totalMass);

	glm::vec3 obj1NewPos = _obj1->GetPosition() - (obj1Offset * _info.normal);
	glm::vec3 obj2NewPos = _obj2->GetPosition() + (obj2Offset * _info.normal);

	_obj1->SetPosition(obj1NewPos);
	_obj2->SetPosition(obj2NewPos);
}

void Collision::HandleVelocityChange(Body * _obj1, Body * _obj2, CollisionInfo & _info)
{
	float e = 0.7f;
	glm::vec3 relativeVelocity = _obj2->GetVelocity() - _obj1->GetVelocity();

	float jTop = -(1 + e) * glm::dot(relativeVelocity, _info.normal);
	float jBottom = glm::dot(_info.normal, _info.normal) * (1.0f / _obj1->GetMass() + 1.0f / _obj2->GetMass());
	float j = jTop / jBottom;

	glm::vec3 obj1NewVelocity = _obj1->GetVelocity() - ((j / _obj1->GetMass()) * _info.normal);
	glm::vec3 obj2NewVelocity = _obj2->GetVelocity() + ((j / _obj2->GetMass()) * _info.normal);

	_obj1->SetVelocity(obj1NewVelocity);
	_obj2->SetVelocity(obj2NewVelocity);
}

bool Collision::TestSpherePlane(const Body * _sphere, const Body * _plane, CollisionInfo & _info)
{
	Sphere* pSphere = (Sphere*)_sphere->GetShape();
	Plane* pPlane = (Plane*)_plane->GetShape();

	float distFromPlane = glm::dot(pPlane->GetNormal(), (_sphere->GetPosition() - _plane->GetPosition()));

	if (distFromPlane < pSphere->GetRadius()) {
		// Collided
		_info.normal = -pPlane->GetNormal();
		_info.penetration = pSphere->GetRadius() - distFromPlane;
		_info.collided = true;
	}
	else {
		// No Collision
		_info.collided = false;
	}

	return _info.collided;
}

bool Collision::TestPlaneSphere(const Body * _plane, const Body * _sphere, CollisionInfo & _info)
{
	TestSpherePlane(_sphere, _plane, _info);
	if (_info.collided)
		_info.normal = -_info.normal;
	return _info.collided;
}
