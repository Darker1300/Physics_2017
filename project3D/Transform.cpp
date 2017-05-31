#include "DEBUG_NEW_LEAK_DETECT.h"
#include "DEBUG_WINDOWS_ERR_MSG.h"

#include "Transform.h"

#include "Vector3.h"
#include <glm\gtx\transform.hpp>

Transform::Transform(const glm::vec3 & _position, const glm::quat & _rotation, const glm::vec3 & _scale)
	: m_position(_position)
	, m_rotation(_rotation)
	, m_scale(_scale)
{
}

Transform::Transform(const glm::vec3 & _position, const glm::quat & _rotation)
	: Transform(_position, _rotation, Vector3::one)
{
}

Transform::Transform(const glm::vec3 & _position)
	: Transform(_position, glm::quat(), Vector3::one)
{
}

Transform::Transform()
	: Transform(Vector3::zero, glm::quat(), Vector3::one)
{
}

Transform::Transform(const Transform & _other)
	: m_position(_other.m_position)
	, m_rotation(_other.m_rotation)
	, m_scale(_other.m_scale)
{
}

Transform & Transform::operator=(const Transform & _other)
{
	m_position = _other.m_position;
	m_rotation = _other.m_rotation;
	m_scale = _other.m_scale;
	return *this;
}

glm::vec3 Transform::Position() const
{
	return m_position;
}

glm::quat Transform::Rotation() const
{
	return m_rotation;
}

glm::vec3 Transform::EulerAngles() const
{
	return glm::eulerAngles(m_rotation);
}

glm::vec3 Transform::Scale() const
{
	return m_scale;
}

glm::vec3 Transform::Forward() const
{
	return glm::normalize(m_rotation * glm::vec3(0, 0, 1));
}

glm::vec3 Transform::Right() const
{
	return glm::normalize(m_rotation * glm::vec3(1, 0, 0));
}

glm::vec3 Transform::Up() const
{
	return glm::normalize(m_rotation * glm::vec3(0, 1, 0));
}

glm::mat4 Transform::Matrix() const
{
	return glm::translate(m_position) * glm::mat4_cast(m_rotation) * glm::scale(m_scale);
}

glm::mat4 Transform::InverseMatrix() const
{
	return glm::inverse(Matrix());
}

void Transform::SetPosition(const glm::vec3 & _position)
{
	m_position = _position;
}

void Transform::SetRotation(const glm::quat & _rotation)
{
	m_rotation = _rotation;
}

void Transform::SetEulerAngles(const glm::vec3 & _rotation)
{
	m_rotation = glm::quat(_rotation);
}

void Transform::SetScale(const glm::vec3 & _scale)
{
	m_scale = _scale;
}

void Transform::LookAt(const glm::vec3 & _lookTarget)
{
	if (_lookTarget == m_position) return;
	m_rotation = LookAt(m_position, _lookTarget);
}

void Transform::Translate(const glm::vec3 & _position)
{
	m_position += _position;
}

void Transform::Rotate(const glm::quat & _rotation)
{
	m_rotation = _rotation * m_rotation;
}

void Transform::RotateTowards(const glm::quat & _to, float _maxRadiansStep)
{
	m_rotation = RotateTowards(m_rotation, _to, _maxRadiansStep);
}

void Transform::RotateTowards(const glm::vec3 & _lookTarget, float _maxRadiansStep)
{
	m_rotation = RotateTowards(m_rotation, LookAt(m_position, _lookTarget), _maxRadiansStep);
}

void Transform::RotateAround(const glm::vec3 & _originPoint, const glm::vec3 & _axis, const float _radians)
{
	m_position = _originPoint + (glm::angleAxis(_radians, _axis) * (m_position - _originPoint));
}

void Transform::MoveTowards(const glm::vec3 & _targetPoint, const float _maxStep)
{
	glm::vec3 dir;

	if (_targetPoint == m_position && glm::sign(_maxStep) <= 0.0f)
		dir = -Forward();
	else
		dir = glm::normalize(_targetPoint - m_position);

	glm::vec3 dist = (_targetPoint - m_position);
	glm::vec3 step = glm::length(dist) > _maxStep ? _maxStep * dir : dist;
	m_position += step;
}

glm::quat Transform::RotateTowards(glm::quat _from, const glm::quat & _to, float _maxRadiansStep) {

	if (_maxRadiansStep < 0.001f) {
		// No rotation allowed. Prevent dividing by 0 later.
		return _from;
	}

	float cosTheta = glm::dot(_from, _to);

	// _from and _to are already equal.
	// Force _to just to be sure
	if (cosTheta > 0.9999f) {
		return _to;
	}

	// Avoid taking the long path around the sphere
	if (cosTheta < 0) {
		_from = _from * -1.0f;
		cosTheta *= -1.0f;
	}

	float angle = glm::acos(cosTheta);

	// If there is only a 2&deg; difference, and we are allowed 5&deg;,
	// then we arrived.
	if (angle < _maxRadiansStep) {
		return _to;
	}

	float fT = _maxRadiansStep / angle;
	angle = _maxRadiansStep;

	glm::quat res = (glm::sin((1.0f - fT) * angle) * _from + glm::sin(fT * angle) * _to) / glm::sin(angle);
	res = glm::normalize(res);
	return res;

}

glm::quat Transform::LookAt(const glm::vec3 & _from, const glm::vec3 & _to)
{
	glm::mat4 viewMat = glm::lookAt(_from, _to, Vector3::up);
	glm::mat4 worldMat = glm::inverse(viewMat);

	return glm::quat_cast(worldMat);
}
