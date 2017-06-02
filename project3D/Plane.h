#pragma once

#include "Shape.h"
#include <glm\vec3.hpp>

namespace Physics {

	class Plane :
		public Shape
	{
	public:

		// -- Implicit basic constructor --

		Plane() : Shape(ShapeType::Plane), m_normal(glm::vec3(0, 1, 0)) {}

		// -- Explicit basic constructor --

		Plane(const glm::vec3& _normal) : Shape(ShapeType::Plane), m_normal(_normal) {}

		~Plane() {}

		glm::vec3 GetNormal() const { return m_normal; }
		void SetNormal(const glm::vec3& _normal) { m_normal = _normal; }
		
		void DrawGizmo(const Body* const _body, const glm::vec4& _colour = glm::vec4(0, 1, 0, 0.1f)) const;

		float GetDistanceToOrigin(const glm::vec3& _position) const;
	private:
		glm::vec3 m_normal;
	};
}
