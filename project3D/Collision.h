#pragma once

#include <glm\vec3.hpp>

namespace Physics {

	class Body;

	namespace Collision
	{
		struct CollisionInfo {
			bool		collided;
			glm::vec3	normal;
			float		penetration;
		};

		// Function pointer type for our collision functions
		typedef bool(*CollisionFunction)(const Body * _obj1, const Body * _obj2, CollisionInfo & _info);
		extern const CollisionFunction CollisionMatrix[];

		bool TestCollision(const Body* _obj1, const Body* _obj2, CollisionInfo& _info);

		void ResolveCollision(Body* _obj1, Body* _obj2, CollisionInfo& _info);
		void HandleSeperation(Body* _obj1, Body* _obj2, CollisionInfo& _info);
		void HandleVelocityChange(Body* _obj1, Body* _obj2, CollisionInfo& _info);

		namespace Test {
			// ----- Homogeneous

			bool SphereSphere(const Body* _sphereA, const Body* _sphereB, CollisionInfo& _info);
			bool AABBAABB(const Body* _aabbA, const Body* _aabbB, CollisionInfo& _info);

			// ----- Heterogeneous
			bool SpherePlane(const Body* _sphere, const Body* _plane, CollisionInfo& _info);
			bool SphereAABB(const Body* _sphere, const Body* _aabb, CollisionInfo& _info);
			bool AABBPlane(const Body* _aabb, const Body* _plane, CollisionInfo& _info);

			// ----- Heterogeneous Wrappers

			bool PlaneSphere(const Body* _plane, const Body* _sphere, CollisionInfo& _info);
			bool AABBSphere(const Body* _aabb, const Body* _sphere, CollisionInfo& _info);
			bool PlaneAABB(const Body* _plane, const Body* _aabb, CollisionInfo& _info);

			// ----- Helpers
			namespace Helper {
				glm::vec3 AABBPoint_ClosestPoint(const Body* _aabb, const glm::vec3& _point);
			}
		}
	}
}
