#pragma once

#include "Shape.h"
#include <glm\vec3.hpp>

namespace Physics {

	class Point
		: public glm::vec3, public Shape
	{
	public:
#pragma region Constructors

		// -- Implicit basic constructor --

		Point() : glm::vec3(), Shape(ShapeType::Point) {}

		// -- Explicit basic constructors --

		template <typename T>
		Point(T const & _type) : glm::vec3(_type), Shape(ShapeType::Point) {}

		template <typename A, typename B, typename C>
		Point(A const & _a, B const & _b, C const & _c) : glm::vec3(_a, _b, _c), Shape(ShapeType::Point) {}

#pragma endregion

		void DrawGizmo(const Body* const _body, const glm::vec4& _colour = glm::vec4(0, 1, 0, 1)) const;

	};

}
