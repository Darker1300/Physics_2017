#include "PhysicsObject.h"

#include "DEBUG_WINDOWS_ERR_MSG.h"

PhysicsObject::PhysicsObject()
	: PhysicsObject(1.0f)
{
}

PhysicsObject::PhysicsObject(float _mass)
	: m_isStatic(false)
	, m_position()
	, m_mass()
	, m_velocity()
	, m_force()
	, m_acceleration()
	, m_shape(nullptr)
{
}

PhysicsObject::~PhysicsObject()
{
}

glm::vec3 PhysicsObject::GetPosition() const
{
	return m_position;
}

float PhysicsObject::GetMass() const
{
	return m_mass;
}

glm::vec3 PhysicsObject::GetVelocity() const
{
	return m_velocity;
}

bool PhysicsObject::GetIsStatic() const
{
	return m_isStatic;
}

void PhysicsObject::SetPosition(const glm::vec3 & _position)
{
	m_position = _position;
}

void PhysicsObject::SetMass(const float _mass)
{
	m_mass = _mass;
}

void PhysicsObject::SetVelocity(const glm::vec3 & _velocity)
{
	m_velocity = _velocity;
}

void PhysicsObject::SetIsStatic(const bool _isStatic)
{
	m_isStatic = _isStatic;
}

void PhysicsObject::AddForce(const glm::vec3 & _force)
{
	m_force += _force;
}

void PhysicsObject::AddAcceleration(const glm::vec3 & _acceleration)
{
	m_acceleration += _acceleration;
}

void PhysicsObject::AddVelocity(const glm::vec3 & _velocity)
{
	m_velocity += _velocity;
}

void PhysicsObject::Update(float _deltaTime)
{
	m_acceleration += m_force / m_mass;
	m_velocity = m_velocity = m_acceleration * _deltaTime;
	m_position = m_position + m_velocity * _deltaTime;

	m_force = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);
}

void PhysicsObject::SetShape(Shape * _shape)
{
	RemoveShape();
	m_shape = _shape;
}

void PhysicsObject::RemoveShape()
{
	if (m_shape != nullptr) {
		delete m_shape;
		m_shape = nullptr;
	}
}
