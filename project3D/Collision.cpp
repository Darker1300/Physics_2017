#include "Collision.h"

#include <glm\glm.hpp>

#include <numeric>
#include "Body.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

using namespace Physics;
using namespace Collision;
using namespace Test;

bool Collision::TestCollision(const Body * _obj1, const Body * _obj2, CollisionInfo & _info)
{
	auto index = (
		(int)(_obj1->GetShape()->GetType()) * (int)(ShapeType::Size)
		+ (int)(_obj2->GetShape()->GetType()));

	if (CollisionMatrix[index] != nullptr)
		return CollisionMatrix[index](_obj1, _obj2, _info);
	else  _info.collided = false;

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
	// Follows algorithm from http://chrishecker.com/Rigid_body_dynamics Subsection: Physics, Part 4: The Third Dimension. Page: 9/10.

	// Average Restitution
	float e = (_obj1->GetRestitution() + _obj2->GetRestitution()) * 0.5f;

	glm::vec3 relativeVelocity = _obj2->GetVelocity() - _obj1->GetVelocity();

	float jTop = -(1 + e) * glm::dot(relativeVelocity, _info.normal);
	float jBottom = glm::dot(_info.normal, _info.normal) * (1.0f / _obj1->GetMass() + 1.0f / _obj2->GetMass());
	// Impulse Magnitude
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

bool Test::AABBAABB(const Body * _aabbA, const Body * _aabbB, CollisionInfo & _info)
{

	_info.collided = false;
	return _info.collided;
	// TODO

	//using namespace glm;
	//using namespace Helper;

	//static const float maxFloat = std::numeric_limits<float>::max();
	//static const vec3 UnitX = { 1, 0, 0 };
	//static const vec3 UnitY = { 0, 1, 0 };
	//static const vec3 UnitZ = { 0, 0, 1 };

	//AABB* boxA = (AABB*)_aabbA->GetShape();
	//AABB* boxB = (AABB*)_aabbB->GetShape();

	//// [Minimum Translation Vector]
	//float mtvDistance = maxFloat;	// Set current minimum distance (max float value so next value is always less)
	//vec3 mtvAxis;		// Axis along which to travel with the minimum distance

	//// [Axes of potential separation]
	//// • Each shape must be projected on these axes to test for intersection:
	////          
	//// (1, 0, 0)                    A0 (= B0) [X Axis]
	//// (0, 1, 0)                    A1 (= B1) [Y Axis]
	//// (0, 0, 1)                    A1 (= B2) [Z Axis]

	//vec3 boxACenter = _aabbA->GetPosition();
	//vec3 boxBCenter = _aabbB->GetPosition();
	//vec3 boxAMin = boxACenter + boxA->GetMin();
	//vec3 boxAMax = boxACenter + boxA->GetMax();
	//vec3 boxBMin = boxBCenter + boxB->GetMin();
	//vec3 boxBMax = boxBCenter + boxB->GetMax();

	//// [X Axis]
	//if (!AABBAABB_TestAxis(UnitX, boxAMin.x, boxAMax.x, boxBMin.x, boxBMax.x, mtvAxis, mtvDistance))
	//{
	//	_info.collided = false;
	//	return _info.collided;
	//}

	//// [Y Axis]
	//if (!AABBAABB_TestAxis(UnitY, boxAMin.y, boxAMax.y, boxBMin.y, boxBMax.y, mtvAxis, mtvDistance))
	//{
	//	_info.collided = false;
	//	return _info.collided;
	//}

	//// [Z Axis]
	//if (!AABBAABB_TestAxis(UnitZ, boxAMin.z, boxAMax.z, boxBMin.z, boxBMax.z, mtvAxis, mtvDistance))
	//{
	//	_info.collided = false;
	//	return _info.collided;
	//}

	//_info.collided = true;

	//// Calculate Minimum Translation Vector (MTV) [normal * penetration]
	//_info.normal = normalize(mtvAxis);

	//// Multiply the penetration depth by itself plus boxA small increment
	//// When the penetration is resolved using the MTV, it will no longer intersect
	//_info.penetration = glm::sqrt(mtvDistance) * 1.001f;

	//return _info.collided;
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

bool Helper::AABBAABB_TestAxis(const glm::vec3 & axis, float minA, float maxA, float minB, float maxB, glm::vec3 & mtvAxis, float & mtvDistance)
{
	using namespace glm;

	// [Separating Axis Theorem]
	// • Two convex shapes only overlap if they overlap on all axes of separation
	// • In order to create accurate responses we need to find the collision vector (Minimum Translation Vector)   
	// • Find if the two boxes intersect along a single axis 
	// • Compute the intersection interval for that axis
	// • Keep the smallest intersection/penetration value
	float axisLengthSquared = glm::dot(axis, axis);

	// If the axis is degenerate then ignore
	if (axisLengthSquared < 1.0e-8f)
	{
		return true;
	}

	// Calculate the two possible overlap ranges
	// Either we overlap on the left or the right sides
	float d0 = (maxB - minA);   // 'Left' side
	float d1 = (maxA - minB);   // 'Right' side

								// Intervals do not overlap, so no intersection
	if (d0 <= 0.0f || d1 <= 0.0f)
	{
		return false;
	}

	// Find out if we overlap on the 'right' or 'left' of the object.
	float overlap = (d0 < d1) ? d0 : -d1;

	// The mtd vector for that axis
	vec3 sep = axis * (overlap / axisLengthSquared);

	// The mtd vector length squared
	float sepLengthSquared = glm::dot(sep, sep);

	// If that vector is smaller than our computed Minimum Translation Distance use that vector as our current MTV distance
	if (sepLengthSquared < mtvDistance)
	{
		mtvDistance = sepLengthSquared;
		mtvAxis = sep;
	}

	return true;
}
