#pragma once

#include "glm/vec3.hpp"

namespace Vector3 {
	// 0, 0, -1
	extern const glm::vec3 backward;
	// 0, 0, 1
	extern const glm::vec3 forward;
	// 0, 1, 0
	extern const glm::vec3 up;
	// 0, -1, 0
	extern const glm::vec3 down;
	// 1, 0, 0
	extern const glm::vec3 right;
	// -1, 0, 0
	extern const glm::vec3 left;
	// 1, 1, 1
	extern const glm::vec3 one;
	// 0, 0, 0
	extern const glm::vec3 zero;
}
