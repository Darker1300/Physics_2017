#include "Collision.h"

#include <glm\glm.hpp>

#include "Body.h"
#include "Sphere.h"
#include "Plane.h"

using namespace Physics;
using namespace Collision;
using namespace Test;

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
	// Follows algorithm from http://chrishecker.com/Rigid_body_dynamics Subsection: Physics, Part 4: The Third Dimension. Page: 9.

	// Bounciness
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

bool Test::SphereSphere(const Body * _sphereA, const Body * _sphereB, CollisionInfo & _info)
{
	// Radius
	const float rCombined = ((Sphere*)_sphereA->GetShape())->GetRadius() + ((Sphere*)_sphereB->GetShape())->GetRadius();
	const float rSquared = rCombined * rCombined;

	// Distance
	const glm::vec3 delta = _sphereB->GetPosition() - _sphereA->GetPosition();
	const float distSquared = glm::dot(delta, delta);

	// Test
	if (distSquared < rSquared) {
		// Collided
		_info.collided = true;
		_info.normal = glm::normalize(delta);
		_info.penetration = glm::sqrt(rSquared - distSquared);
	}
	else {
		// No Collision
		_info.collided = false;
	}

	return _info.collided;
}

// #define RotationResolutionInPlace
#ifdef RotationResolutionInPlace
bool Physics::Collision::Test::PlanePlane(const Body * _planeA, const Body * _planeB, CollisionInfo & _info)
{
	const glm::vec3 crossNormal = glm::cross(((Plane*)_planeA->GetShape())->GetNormal(), ((Plane*)_planeB->GetShape())->GetNormal());
	const float length = crossNormal.length();
	const float det = length * length;

	if (det != 0) {
		// Collision

	}
	else {
		// No Collision, Planes are parallel, det == 0

	}
	_info.collided = false;
	return _info.collided;
}
#endif

bool Test::SpherePlane(const Body * _sphere, const Body * _plane, CollisionInfo & _info)
{
	Sphere* pSphere = (Sphere*)_sphere->GetShape();
	Plane* pPlane = (Plane*)_plane->GetShape();

	// Distance from Plane
	float distance = glm::dot(pPlane->GetNormal(), (_sphere->GetPosition() - _plane->GetPosition()));

	if (distance < pSphere->GetRadius()) {
		// Collided
		_info.normal = -pPlane->GetNormal();
		_info.penetration = pSphere->GetRadius() - distance;
		_info.collided = true;
	}
	else {
		// No Collision
		_info.collided = false;
	}

	return _info.collided;
}

bool Test::PlaneSphere(const Body * _plane, const Body * _sphere, CollisionInfo & _info)
{
	SpherePlane(_sphere, _plane, _info);
	if (_info.collided)
		_info.normal = -_info.normal;
	return _info.collided;
}
