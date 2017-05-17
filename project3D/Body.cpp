#include "Body.h"



Body::Body()
{
}


Body::~Body()
{
}

float Body::GetMass()
{
	return m_mass;
}

float Body::GetMassInverse()
{
	return m_massInverse;
}

void Body::SetMass(float _mass)
{
	m_mass = _mass;
	m_massInverse = m_mass == 0 ? 0 : 1 / m_mass;
}
