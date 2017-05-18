#pragma once

#include "Shape.h"
#include <glm\vec3.hpp>

class PhysicsObject
{
public:
	PhysicsObject();
	PhysicsObject(float _mass);
	~PhysicsObject();

	glm::vec3	GetPosition() const;
	float		GetMass() const;
	glm::vec3	GetVelocity() const;
	bool		GetIsStatic() const;

	void SetPosition(const glm::vec3& _position);
	void SetMass(const float _mass);
	void SetVelocity(const glm::vec3& _velocity);
	void SetIsStatic(const bool _isStatic);

	void AddForce(const glm::vec3& _force);
	void AddAcceleration(const glm::vec3& _acceleration);
	void AddVelocity(const glm::vec3& _velocity);

	void Update(float _deltaTime);

	void SetShape(Shape* _shape);
	void RemoveShape();

	Shape* GetShape() const { return m_shape; }

private:
	bool		m_isStatic;

	glm::vec3	m_position;
	float		m_mass;
	glm::vec3	m_velocity;

	glm::vec3	m_force;
	glm::vec3	m_acceleration;

	Shape* m_shape;
};
