#include "AABB.h"

#include <Gizmos.h>
#include "Body.h"

using namespace Physics;

void AABB::DrawGizmo(const Body * const _body, const glm::vec4 & _colour) const
{
	aie::Gizmos::addAABBFilled(_body->GetPosition(), GetExtents(), _colour);
}
