#include "Sphere.h"

#include "Body.h"
#include <Gizmos.h>

void Physics::Sphere::DrawGizmo(const Body * const _body, const glm::vec4 & _colour) const
{
	aie::Gizmos::addSphere(_body->GetPosition(), GetRadius(), 12, 12, _colour);
}
