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

		// Define a function pointer type for our collision functions
		typedef bool(*CollisionFunction)(const Body * _obj1, const Body * _obj2, CollisionInfo & _info);
		extern const CollisionFunction CollisionMatrix[];

		bool TestCollision(const Body* _obj1, const Body* _obj2, CollisionInfo& _info);

		void ResolveCollision(Body* _obj1, Body* _obj2, CollisionInfo& _info);
		void HandleSeperation(Body* _obj1, Body* _obj2, CollisionInfo& _info);
		void HandleVelocityChange(Body* _obj1, Body* _obj2, CollisionInfo& _info);

		// Test Collision Functions
		bool TestSpherePlane(const Body* _sphere, const Body* _plane, CollisionInfo& _info);
		bool TestPlaneSphere(const Body* _plane, const Body* _sphere, CollisionInfo& _info);
	}

}
