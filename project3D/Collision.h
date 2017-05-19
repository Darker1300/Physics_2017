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

			//TODO\\ bool PlanePlane(const Body* _planeA, const Body* _planeB, CollisionInfo& _info);

			// ----- Heterogeneous

			bool SpherePlane(const Body* _sphere, const Body* _plane, CollisionInfo& _info);

			// ----- Heterogeneous Wrappers

			bool PlaneSphere(const Body* _plane, const Body* _sphere, CollisionInfo& _info);

			// ----- Helpers
			namespace Helper {
				bool AABBAABB_TestAxis(const glm::vec3& axis, float minA, float maxA, float minB, float maxB, glm::vec3& mtvAxis, float& mtvDistance);
			}
		}
	}
}
