#pragma once

#include "Shape.h"

class Sphere :
	public Shape
{
public:
	Sphere();
	~Sphere();

	float GetRadius() const { return m_radius; }

private:
	float m_radius;
};

