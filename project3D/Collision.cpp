#include "Collision.h"

#include <glm\glm.hpp>

#include <numeric>
#include "Body.h"
#include "Sphere.h"
#include "Plane.h"
#include "AABB.h"

namespace Physics {
	namespace Collision {

		bool Collision::TestCollision(const Body * _obj1, const Body * _obj2, CollisionInfo & _info)
		{
			// Select Test Algorithm based on ShapeType
			auto index = (
				(int)(_obj1->GetShape()->GetType()) * (int)(ShapeType::Size)
				+ (int)(_obj2->GetShape()->GetType()));

			if (CollisionMatrix[index] != nullptr) {
				// If Test Function exists, use it
				return CollisionMatrix[index](_obj1, _obj2, _info);
			}
			else  _info.collided = false;

			return _info.collided;
		}

		void Collision::ResolveCollision(Body * _obj1, Body * _obj2, CollisionInfo & _info)
		{
			if (_info.collided && !(_obj1->GetIsStatic() && _obj2->GetIsStatic())) {	// Collided and Not Both Static
				HandleSeperation(_obj1, _obj2, _info);
				HandleVelocityChange(_obj1, _obj2, _info);
			}
		}

		void Collision::HandleSeperation(Body * _obj1, Body * _obj2, CollisionInfo & _info)
		{
			if (_obj1->GetIsStatic() || _obj2->GetIsStatic()) {	// One Side is Dynamic
				if (!_obj1->GetIsStatic()) {	// obj1 is Dynamic
					glm::vec3 obj1NewPos = _obj1->GetPosition() - (_info.penetration * _info.normal);
					_obj1->SetPosition(obj1NewPos);
				}
				else if (!_obj2->GetIsStatic()) {	// obj2 is Dynamic
					glm::vec3 obj2NewPos = _obj2->GetPosition() + (_info.penetration * _info.normal);
					_obj2->SetPosition(obj2NewPos);
				}
			}
			else {	// Both Dynamic
				float totalMass = _obj1->GetMass() + _obj2->GetMass();
				float obj1Offset = _info.penetration * (_obj2->GetMass() / totalMass);
				float obj2Offset = _info.penetration * (_obj1->GetMass() / totalMass);
				glm::vec3 obj1NewPos = _obj1->GetPosition() - (obj1Offset * _info.normal);
				glm::vec3 obj2NewPos = _obj2->GetPosition() + (obj2Offset * _info.normal);
				_obj1->SetPosition(obj1NewPos);
				_obj2->SetPosition(obj2NewPos);
			}
		}

		void Collision::HandleVelocityChange(Body * _obj1, Body * _obj2, CollisionInfo & _info)
		{
			float e;	// Restitution
			if (_obj1->GetIsStatic() || _obj2->GetIsStatic()) {	// One Side is Dynamic
				e = _obj1->GetIsStatic() ? _obj2->GetRestitution() : _obj1->GetRestitution();
			}
			else {	// Both Dynamic
				e = (_obj1->GetRestitution() + _obj2->GetRestitution()) * 0.5f;	// Average Restitution
			}

			glm::vec3 relativeVelocity = _obj2->GetVelocity() - _obj1->GetVelocity();	// Relative Velocity

			// Follows algorithm from http://chrishecker.com/Rigid_body_dynamics Subsection: Physics, Part 4: The Third Dimension. Page: 9/10.
			float jTop = -(1 + e) * glm::dot(relativeVelocity, _info.normal);
			float jBottom = glm::dot(_info.normal, _info.normal) * (1.0f / _obj1->GetMass() + 1.0f / _obj2->GetMass());
			float j = jTop / jBottom; // Impulse Magnitude

			// Apply resulting velocity to Dynamic objects
			if (!_obj1->GetIsStatic()) {
				glm::vec3 obj1NewVelocity = _obj1->GetVelocity() - ((j / _obj1->GetMass()) * _info.normal);
				_obj1->SetVelocity(obj1NewVelocity);
			}
			if (!_obj2->GetIsStatic()) {
				glm::vec3 obj2NewVelocity = _obj2->GetVelocity() + ((j / _obj2->GetMass()) * _info.normal);
				_obj2->SetVelocity(obj2NewVelocity);
			}
		}

		namespace Test {

			bool Test::SphereSphere(const Body * _sphereA, const Body * _sphereB, CollisionInfo & _info)
			{
				const float minDistance =
					((const Sphere*)_sphereA->GetShape())->GetRadius()
					+ ((const Sphere*)_sphereB->GetShape())->GetRadius();

				glm::vec3 offset = _sphereB->GetPosition() - _sphereA->GetPosition();
				float length = glm::length(offset);
				if (length < minDistance)
				{
					_info.collided = true;
					_info.normal = glm::normalize(offset);
					_info.penetration = minDistance - length;
				}
				else
				{
					_info.collided = false;
				}

				return _info.collided;
			}

			bool Test::AABBAABB(const Body * _aabbA, const Body * _aabbB, CollisionInfo & _info)
			{
				const AABB* pAabbA = (const AABB*)_aabbA->GetShape();
				const AABB* pAabbB = (const AABB*)_aabbB->GetShape();

				glm::vec3 minA = _aabbA->GetPosition() - pAabbA->GetExtents();
				glm::vec3 minB = _aabbB->GetPosition() - pAabbB->GetExtents();

				glm::vec3 maxA = _aabbA->GetPosition() + pAabbA->GetExtents();
				glm::vec3 maxB = _aabbB->GetPosition() + pAabbB->GetExtents();

				if (maxA.x < minB.x || minA.x > maxB.x ||
					maxA.y < minB.y || minA.y > maxB.y ||
					maxA.z < minB.z || minA.z > maxB.z)
				{
					_info.collided = false;
					return false;
				}

				glm::vec3 offsetA = maxA - minB;
				glm::vec3 offsetB = minA - maxB;

				glm::vec3 smallestOffset = glm::vec3(0);
				smallestOffset.x =
					(fabs(offsetA.x) < fabs(offsetB.x)) ? offsetA.x : offsetB.x;
				smallestOffset.y =
					(fabs(offsetA.y) < fabs(offsetB.y)) ? offsetA.y : offsetB.y;
				smallestOffset.z =
					(fabs(offsetA.z) < fabs(offsetB.z)) ? offsetA.z : offsetB.z;

				glm::vec3 normal = glm::vec3(1, 0, 0);
				float intercept = smallestOffset.x;
				if (fabs(smallestOffset.y) < fabs(intercept))
				{
					normal = glm::vec3(0, 1, 0);
					intercept = smallestOffset.y;
				}

				if (fabs(smallestOffset.z) < fabs(intercept))
				{
					normal = glm::vec3(0, 0, 1);
					intercept = smallestOffset.z;
				}

				_info.collided = true;
				_info.normal = normal;
				_info.penetration = intercept;

				return true;

			}

			bool Test::SpherePlane(const Body * _sphere, const Body * _plane, CollisionInfo & _info)
			{
				const Sphere* pSphere = (const Sphere*)_sphere->GetShape();
				const Plane* pPlane = (const Plane*)_plane->GetShape();

				float distanceFromPlane =
					(glm::dot(_sphere->GetPosition(), pPlane->GetNormal())) -
					pPlane->GetDistanceToOrigin(_plane->GetPosition());
				if (distanceFromPlane < pSphere->GetRadius())
				{
					// Collision
					_info.normal = -pPlane->GetNormal();
					_info.penetration =
						pSphere->GetRadius() - distanceFromPlane;

					_info.collided = true;
				}
				else
				{
					// No Collision
					_info.collided = false;
				}

				return _info.collided;
			}

			bool Test::SphereAABB(const Body * _sphere, const Body * _aabb, CollisionInfo & _info)
			{
				AABBSphere(_aabb, _sphere, _info);
				if (_info.collided)
					_info.normal = -_info.normal;
				return _info.collided;
			}

			bool Test::AABBPlane(const Body * _aabb, const Body * _plane, CollisionInfo & _info)
			{
				PlaneAABB(_plane, _aabb, _info);
				if (_info.collided)
					_info.normal = -_info.normal;
				return _info.collided;
			}

			bool Test::PlaneSphere(const Body * _plane, const Body * _sphere, CollisionInfo & _info)
			{
				SpherePlane(_sphere, _plane, _info);
				if (_info.collided)
					_info.normal = -_info.normal;
				return _info.collided;
			}

			bool Test::AABBSphere(const Body * _aabb, const Body * _sphere, CollisionInfo & _info)
			{
				const AABB* pAabb = (const AABB*)_aabb->GetShape();
				const Sphere* pSphere = (const Sphere*)_sphere->GetShape();

				glm::vec3 offset = _sphere->GetPosition() - _aabb->GetPosition();
				glm::vec3 extents = pAabb->GetExtents();

				glm::vec3 edge = glm::vec3(0);
				edge.x =
					glm::clamp(glm::dot(offset, glm::vec3(1, 0, 0)), -extents.x, extents.x);
				edge.y =
					glm::clamp(glm::dot(offset, glm::vec3(0, 1, 0)), -extents.y, extents.y);
				edge.z =
					glm::clamp(glm::dot(offset, glm::vec3(0, 0, 1)), -extents.z, extents.z);

				glm::vec3 aabbClosestPoint = _aabb->GetPosition() + edge;

				offset = _sphere->GetPosition() - aabbClosestPoint;
				float length = glm::length(offset);
				if (length < pSphere->GetRadius() && length > 0)
				{
					_info.collided = true;
					_info.normal = glm::normalize(offset);
					_info.penetration = pSphere->GetRadius() - length;
				}
				else
				{
					_info.collided = false;
				}
				return _info.collided;
			}

			bool Test::PlaneAABB(const Body * _plane, const Body * _aabb, CollisionInfo & _info)
			{
				const AABB* pAABB = (const AABB*)_aabb->GetShape();
				const Plane* pPlane = (const Plane*)_plane->GetShape();

				glm::vec3 aabbMin = _aabb->GetPosition() - pAABB->GetExtents();
				glm::vec3 aabbMax = _aabb->GetPosition() + pAABB->GetExtents();

				float distanceMin = glm::dot(pPlane->GetNormal(), aabbMin) -
					pPlane->GetDistanceToOrigin(_plane->GetPosition());
				float distanceMax = glm::dot(pPlane->GetNormal(), aabbMax) -
					pPlane->GetDistanceToOrigin(_plane->GetPosition());

				float smallestDistance =
					(distanceMin < distanceMax) ? distanceMin : distanceMax;

				if (smallestDistance < 0)
				{
					_info.penetration = -smallestDistance;
					_info.normal = pPlane->GetNormal();
					_info.collided = true;
				}
				else
				{
					_info.collided = false;
				}

				return _info.collided;
			}

			namespace Helper {

				glm::vec3 Helper::AABBPoint_ClosestPoint(const Body * _aabb, const glm::vec3 & _point)
				{
					const AABB* pAabb = (const AABB*)_aabb->GetShape();
					glm::vec3 extents = pAabb->GetExtents();
					glm::vec3 offset = _point - _aabb->GetPosition();

					auto edge = glm::vec3(0);
					edge.x =
						glm::clamp(glm::dot(offset, glm::vec3(1, 0, 0)), -extents.x, extents.x);
					edge.y =
						glm::clamp(glm::dot(offset, glm::vec3(0, 1, 0)), -extents.y, extents.y);
					edge.z =
						glm::clamp(glm::dot(offset, glm::vec3(0, 0, 1)), -extents.z, extents.z);

					return _aabb->GetPosition() + edge;
				}
			}
		}
	}
}
