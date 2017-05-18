#pragma once

#include <glm\vec3.hpp>

class PhysicsObject;

namespace Collision 
{
	struct CollisionInfo {
		bool		collided;
		glm::vec3	normal;
		float		penetration;
	};

	// Define a function pointer type for our collision functions
	typedef bool(*CollisionFunction)(const PhysicsObject * _obj1, const PhysicsObject * _obj2, CollisionInfo & _info);
	extern const CollisionFunction CollisionMatrix[];

	bool TestCollision(const PhysicsObject* _obj1, const PhysicsObject* _obj2, CollisionInfo& _info);

	void ResolveCollision(PhysicsObject* _obj1, PhysicsObject* _obj2, CollisionInfo& _info);
	void HandleSeperation(PhysicsObject* _obj1, PhysicsObject* _obj2, CollisionInfo& _info);
	void HandleVelocityChange(PhysicsObject* _obj1, PhysicsObject* _obj2, CollisionInfo& _info);

	// Test Collision Functions
	bool TestSpherePlane(const PhysicsObject* _sphere, const PhysicsObject* _plane, CollisionInfo& _info);
	bool TestPlaneSphere(const PhysicsObject* _plane, const PhysicsObject* _sphere, CollisionInfo& _info);
}
