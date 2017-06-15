#include "Body.h"

#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

using namespace Physics;

Body::Body()
	: Body(1.0f)
{
}

Body::Body(const float _mass)
	: m_isStatic(false)
	, m_position()
	, m_mass(_mass)
	, m_velocity()
	, m_force()
	, m_acceleration()
	, m_restitution(0.25f)
	, m_shape(nullptr)
{
}

Body::~Body()
{
	if (m_shape != nullptr)
		delete m_shape;
}

glm::vec3 Body::GetPosition() const
{
	return m_position;
}

float Body::GetMass() const
{
	return m_mass;
}

glm::vec3 Body::GetVelocity() const
{
	return m_velocity;
}

bool Body::GetIsStatic() const
{
	return m_isStatic;
}

float Body::GetRestitution() const
{
	return m_restitution;
}

void Body::SetPosition(const glm::vec3 & _position)
{
	m_position = _position;
}

void Body::SetMass(const float _mass)
{
	m_mass = _mass;
}

void Body::SetVelocity(const glm::vec3 & _velocity)
{
	m_velocity = _velocity;
}

void Body::SetIsStatic(const bool _isStatic)
{
	m_isStatic = _isStatic;
}

void Body::SetRestitution(const float _restitution)
{
	m_restitution = _restitution;
}

void Body::AddForce(const glm::vec3 & _force)
{
	m_force += _force;
}

void Body::AddAcceleration(const glm::vec3 & _acceleration)
{
	m_acceleration += _acceleration;
}

void Body::AddVelocity(const glm::vec3 & _velocity)
{
	m_velocity += _velocity;
}

void Body::Update(float _deltaTime)
{
	m_acceleration += m_force / m_mass;
	m_velocity = m_velocity + m_acceleration * _deltaTime;
	m_position = m_position + m_velocity * _deltaTime;

	m_force = glm::vec3(0.0f);
	m_acceleration = glm::vec3(0.0f);
}

void Body::SetShape(Shape * _shape)
{
	RemoveShape();
	m_shape = _shape;
}

void Body::RemoveShape()
{
	if (m_shape != nullptr) {
		delete m_shape;
		m_shape = nullptr;
	}
}

void Body::DrawGizmo(const glm::vec4& _colour) const
{
	if (m_shape != nullptr)
		m_shape->DrawGizmo(this, _colour);
}
