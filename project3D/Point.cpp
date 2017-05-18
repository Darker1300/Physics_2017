#include "Point.h"

#include <Gizmos.h>
#include "Body.h"

void Physics::Point::DrawGizmo(const Body * const _body, const glm::vec4 & _colour) const
{
	aie::Gizmos::addSphere(_body->GetPosition(), 0.1f, 12, 12, _colour);
}
