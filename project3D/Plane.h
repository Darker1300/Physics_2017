#pragma once

#include "Shape.h"
#include <glm\vec3.hpp>

class Plane :
	public Shape
{
public:
	Plane();
	~Plane();

	glm::vec3 GetNormal() const { return m_normal; }

private:
	glm::vec3 m_normal;
};

