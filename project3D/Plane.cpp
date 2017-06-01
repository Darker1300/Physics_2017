#include "Plane.h"

#include <Gizmos.h>
#include "Body.h"
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

void Physics::Plane::DrawGizmo(const Body * const _body, const glm::vec4 & _colour) const
{
	const glm::vec3 forward = -m_normal;
	glm::vec3 up = { 0.0f, 1.0f, 0.0f };
	if (glm::abs(glm::dot(forward, up)) >= 0.99f)
	{
		up = { 0.0f, 0.0f, 1.0f };
	}
	glm::vec3 right = glm::cross(forward, up);
	up = glm::cross(right, forward);

	const glm::mat4 transform = { glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(_body->GetPosition(), 1.0f) };

	const int totalSize = 5;
	const int colCount = 8;
	const int rowCount = 8;
	const float colExtent = (float)totalSize / colCount;
	const float rowExtent = (float)totalSize / rowCount;

	for (int x = -colCount; x <= colCount; x++) {
		for (int y = -rowCount; y <= rowCount; y++) {
			aie::Gizmos::addAABB({ x * (colExtent * 2.0f), 0.0f, y * (rowExtent * 2.0f) }, { colExtent, rowExtent, 0.01f }, _colour, &transform);
		}
	}
}

