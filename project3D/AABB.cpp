#include "AABB.h"

using namespace Physics;

AABB::AABB() : Shape(ShapeType::AABB)
{
}


AABB::~AABB()
{
}

void Physics::AABB::DrawGizmo(const Body * const _body, const glm::vec4 & _colour) const
{
	// TODO
}
