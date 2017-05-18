#include "Plane.h"

#include <Gizmos.h>
#include "Body.h"
#include <glm\glm.hpp>

void Physics::Plane::DrawGizmo(const Body * const _body, const glm::vec4 & _colour) const
{
	static const glm::vec3 worldUp = { 0,1,0 };
	static const glm::vec3 worldRight = { 1,0,0 };
	static const glm::vec3 worldForward = { 0,0,1 };

	//static const glm::vec3 planeSize = { 0.25f,1000.0f,1000.0f };

	glm::vec3 right, forward, up;

	float fDot = glm::dot(worldUp, GetNormal());
	if (glm::abs(fDot) < 0.99f) {
		right = GetNormal();
		forward = glm::cross(right, worldUp);
		up = glm::cross(forward, worldRight);
	}
	else {
		right = GetNormal();
		up = glm::cross(worldForward, right);
		forward = glm::cross(right, up);
	}

	glm::mat4 transform(1);
	transform[0] = glm::vec4(right, 0);
	transform[1] = glm::vec4(up, 0);
	transform[2] = glm::vec4(forward, 0);
	transform[3] = glm::vec4(_body->GetPosition() * GetNormal(), 1);

	//aie::Gizmos::addAABBFilled(_body->GetPosition(), planeSize, _colour, &transform);

	static glm::vec4 white(1);
	static glm::vec4 black(0, 0, 0, 1);
	static const int planeLineCount = 21;
	static const float planeSize = 10.0f;

	for (int i = 0; i < planeLineCount; ++i) {
		aie::Gizmos::addLine(
			glm::vec3(transform * glm::vec4(0, -planeSize + i, planeSize, 1)),
			glm::vec3(transform * glm::vec4(0, -planeSize + i, -planeSize, 1)),
			i == 10 ? white : _colour);
		aie::Gizmos::addLine(
			glm::vec3(transform * glm::vec4(0, planeSize, -planeSize + i, 1)),
			glm::vec3(transform * glm::vec4(0, -planeSize, -planeSize + i, 1)),
			i == 10 ? white : _colour);
	}
}
