#pragma once

#include <glm\glm.hpp>

class Body
{
public:
	Body();
	virtual ~Body();

	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_velocity;

	float GetMass();
	float GetMassInverse();

	void SetMass(float _mass);

protected:
	float m_mass, m_massInverse;
};

