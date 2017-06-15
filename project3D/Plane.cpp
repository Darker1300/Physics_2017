#include "Plane.h"

#include <Gizmos.h>
#include "Body.h"
#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>
#include <glm\gtc\matrix_transform.hpp>

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

	const glm::mat4 transform = { glm::vec4(right, 0.0f), glm::vec4(up, 0.0f), glm::vec4(forward, 0.0f), glm::vec4(_body->GetPosition() * 0.5f, 1.0f) }; // _body->GetPosition()

	const int totalSize = 5;
	const int colCount = 8;
	const int rowCount = 8;
	const float colExtent = (float)totalSize / colCount;
	const float rowExtent = (float)totalSize / rowCount;

	for (int x = -colCount; x <= colCount; x++) {
		for (int y = -rowCount; y <= rowCount; y++) {
			glm::vec4 offset = glm::vec4(x * (colExtent * 2.0f), y * (rowExtent * 2.0f), 0.0f, 1);
			glm::vec3 p = glm::vec3(transform * offset);
			glm::vec4 col = glm::vec4(glm::normalize(glm::abs(glm::vec3(offset))), 0.85f);
			aie::Gizmos::addAABBFilled(p, { colExtent, rowExtent, 0.01f }, { 0,0,0,0.95f }, &transform);
		}
	}

	aie::Gizmos::addTransform(glm::translate(glm::mat4(1), _body->GetPosition()));
}

float Physics::Plane::GetDistanceToOrigin(const glm::vec3 & _position) const
{
	return glm::dot(m_normal, (_position));
}

