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

			if (!_obj1->GetIsStatic())
				_obj1->SetPosition(obj1NewPos);
			if (!_obj2->GetIsStatic())
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

			if (!_obj1->GetIsStatic())
			_obj1->SetVelocity(obj1NewVelocity);
			if (!_obj2->GetIsStatic())
			_obj2->SetVelocity(obj2NewVelocity);
		}

		namespace Test {

			bool Test::SphereSphere(const Body * _sphereA, const Body * _sphereB, CollisionInfo & _info)
			{
				// Radius
				const float rCombined =
					((const Sphere*)_sphereA->GetShape())->GetRadius()
					+ ((const Sphere*)_sphereB->GetShape())->GetRadius();
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
				glm::vec3 centerA = _aabbA->GetPosition();
				glm::vec3 centerB = _aabbB->GetPosition();
				glm::vec3 extentsA = ((const AABB*)_aabbA->GetShape())->GetExtents();
				glm::vec3 extentsB = ((const AABB*)_aabbB->GetShape())->GetExtents();

				glm::vec3 overlap = glm::vec3();
				for (int axis = 0; axis < 3; axis++) {
					// d = | centreB - centreA | -(halfExtentsA + halfExtentsB)
					overlap[axis] = (glm::abs(centerB[axis] - centerA[axis])) - (extentsA[axis] + extentsB[axis]);
					if (overlap[axis] >= 0.0f) {
						_info.collided = false;
						return _info.collided;
					}
				}

				int closestAxis = 0;
				for (int axis = 1; axis < 3; axis++) {
					if (overlap[axis] > overlap[closestAxis]) closestAxis = axis;
				}

				for (int axis = 0; axis < 3; axis++) {
					if (axis == closestAxis) continue;
					else overlap[axis] = 0.0f;
				}

				_info.collided = true;
				_info.normal = glm::normalize(overlap);
				_info.penetration = overlap[closestAxis];
				return _info.collided;

			}

			bool Test::SpherePlane(const Body * _sphere, const Body * _plane, CollisionInfo & _info)
			{
				const Sphere* pSphere = (const Sphere*)_sphere->GetShape();
				const Plane* pPlane = (const Plane*)_plane->GetShape();

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

			bool SphereAABB(const Body * _sphere, const Body * _aabb, CollisionInfo & _info)
			{
				//DO OPPERSITE

				const Sphere* pSphere = (const Sphere*)_sphere->GetShape();
				const AABB* pAabb = (const AABB*)_aabb->GetShape();

				float squaredDst = Helper::AABBPoint_ClosestPoint_SquaredDist(_aabb, _sphere->GetPosition());
				float radSqrd = (pSphere->GetRadius() * pSphere->GetRadius());
				if (squaredDst > radSqrd) {
					_info.collided = false;
					return _info.collided;
				}

				_info.normal = glm::normalize(_aabb->GetPosition() - _sphere->GetPosition());
				_info.penetration = glm::sqrt(squaredDst);
				_info.collided = true;
				//_info.collided = false;
				return _info.collided;
			}

			bool AABBPlane(const Body * _aabb, const Body * _plane, CollisionInfo & _info)
			{
				const AABB* aabb = (const AABB*)_aabb->GetShape();
				const Plane* plane = (const Plane*)_plane->GetShape();
				float planeDistance = plane->GetDistanceToOrigin(_plane->GetPosition());

				const float r = glm::dot(aabb->GetExtents(), glm::abs(plane->GetNormal()));
				const float s = glm::dot(plane->GetNormal(), _aabb->GetPosition()) - planeDistance;

				if (glm::abs(s) < r)
				{
					_info.penetration = glm::abs(s - r);
					_info.normal = -plane->GetNormal();
					_info.collided = true;
					return _info.collided;
				}

				_info.collided = false;
				return _info.collided;
			}

			bool Test::PlaneSphere(const Body * _plane, const Body * _sphere, CollisionInfo & _info)
			{
				SpherePlane(_sphere, _plane, _info);
				if (_info.collided)
					_info.normal = -_info.normal;
				return _info.collided;
			}

			bool AABBSphere(const Body * _aabb, const Body * _sphere, CollisionInfo & _info)
			{
				SphereAABB(_sphere, _aabb, _info);
				if (_info.collided)
					_info.normal = -_info.normal;
				return _info.collided;
			}

			bool PlaneAABB(const Body * _plane, const Body * _aabb, CollisionInfo & _info)
			{
				AABBPlane(_aabb, _plane, _info);
				if (_info.collided)
					_info.normal = -_info.normal;
				return _info.collided;
			}

			namespace Helper {

				glm::vec3 Helper::AABBPoint_ClosestPoint(const Body * _aabbA, const glm::vec3 & _point)
				{
					glm::vec3 center = _aabbA->GetPosition();
					AABB* aabb = (AABB*)(_aabbA->GetShape());
					glm::vec3 min = center - aabb->GetExtents();
					glm::vec3 max = center + aabb->GetExtents();
					glm::vec3 closest = glm::vec3(
						AABBPoint_ClosestPoint_Axis(min.x, max.x, _point.x),
						AABBPoint_ClosestPoint_Axis(min.y, max.y, _point.y),
						AABBPoint_ClosestPoint_Axis(min.z, max.z, _point.z)
					);
					return closest;
				}

				float AABBPoint_ClosestPoint_Axis(const float _min, const float _max, const float _point)
				{
					float result = 0;
					if (_point > _max)
						result = _max;
					else if (_point < _min)
						result = _min;
					else result = _point;
					return result;
				}
				float AABBPoint_ClosestPoint_SquaredDist(const Body * _aabb, const glm::vec3 & _point)
				{
					const AABB* pAabb = (const AABB*)_aabb->GetShape();
					glm::vec3 center = _aabb->GetPosition();
					glm::vec3 bmin = center - pAabb->GetExtents();
					glm::vec3 bmax = center - pAabb->GetExtents();

					glm::vec3 out = glm::vec3();

					for (int axis = 0; axis < 3; axis++) {

						if (_point[axis] < bmin[axis])
						{
							float val = (bmin[axis] - _point[axis]);
							out[axis] += val * val;
						}

						if (_point[axis] > bmax[axis])
						{
							float val = (_point[axis] - bmax[axis]);
							out[axis] += val * val;
						}
					}

					return out.x + out.y + out.z;
				}
			}
		}
	}
}
