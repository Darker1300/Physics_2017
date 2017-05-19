#pragma once

#include "Shape.h"

namespace Physics {

	class Sphere :
		public Shape
	{
	public:
		// -- Implicit basic constructor --

		Sphere() : m_radius(), Shape(ShapeType::Sphere) {}

		// -- Explicit basic constructor --

		Sphere(const float _radius) : m_radius(_radius), Shape(ShapeType::Sphere) {}

		~Sphere() {}

		float GetRadius() const { return m_radius; }

		void DrawGizmo(const Body* const _body, const glm::vec4& _colour = glm::vec4(1, 0, 0, 0.5f)) const;

	private:
		float m_radius;
	};

}
