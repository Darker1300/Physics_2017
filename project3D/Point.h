#pragma once

#include "Shape.h"
#include <glm\vec3.hpp>

class Point
	: public glm::vec3, public Shape
{
public:
#pragma region Constructors

	// -- Implicit basic constructor --
	Point() : glm::vec3(), Shape(ShapeType::Point) {}

	// -- Explicit basic constructors --
	template <typename S>
	Point(S const & scalar) : glm::vec3(scalar), Shape(ShapeType::Point) {}

	template <typename A, typename B, typename C>
	Point(A const & a, B const & b, C const & c) : glm::vec3(a, b, c), Shape(ShapeType::Point) {}

#pragma endregion
};

