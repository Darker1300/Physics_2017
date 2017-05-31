#pragma once

#include "glm/vec3.hpp"
#include "glm/gtc/quaternion.hpp"

class Transform
{
public:
	Transform(const glm::vec3& _position, const glm::quat& _rotation, const glm::vec3& _scale);
	Transform(const glm::vec3& _position, const glm::quat& _rotation);
	Transform(const glm::vec3& _position);
	Transform();
	Transform(const Transform& _other);	// Copy constructor
	Transform& operator = (const Transform& _other);	// Copy assignment

	glm::vec3 Position() const;
	glm::quat Rotation() const;
	glm::vec3 EulerAngles() const;
	glm::vec3 Scale() const;

	glm::vec3 Forward() const;
	glm::vec3 Right() const;
	glm::vec3 Up() const;

	glm::mat4 Matrix() const;
	glm::mat4 InverseMatrix() const;

	void SetPosition(const glm::vec3& _position);
	void SetRotation(const glm::quat& _rotation);
	void SetEulerAngles(const glm::vec3& _rotation);
	void SetScale(const glm::vec3& _scale);

	void LookAt(const glm::vec3& _lookTarget);

	void Translate(const glm::vec3& _position);
	void Rotate(const glm::quat& _rotation);

	void RotateTowards(const glm::quat& _to, float _maxRadiansStep);
	void RotateTowards(const glm::vec3& _lookTarget, float _maxRadiansStep);

	void RotateAround(const glm::vec3& _originPoint, const glm::vec3& _axis, const float _radians);
	void MoveTowards(const glm::vec3& _targetPoint, const float _maxStep);

private:
	glm::vec3 m_position;
	glm::quat m_rotation;
	glm::vec3 m_scale;

	static glm::quat RotateTowards(glm::quat _from, const glm::quat& _to, float _maxRadiansStep);
	static glm::quat LookAt(const glm::vec3& _from, const glm::vec3& _to);
};
