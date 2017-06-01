#pragma once

#include "Shape.h"
#include <glm\vec3.hpp>

namespace Physics {

	class AABB :
		public Shape
	{
	public:

		// -- Implicit basic constructor --

		AABB() : m_extents(0.0f), Shape(ShapeType::AABB) {}

		// -- Explicit basic constructor --

		AABB(const float _size) : m_extents(_size * 0.5f), Shape(ShapeType::AABB) {}

		~AABB() {}

		glm::vec3 GetExtents() const { return m_extents; }
		glm::vec3 GetSize() const { return m_extents * 2.0f; }
		glm::vec3 GetMin() const { return -m_extents; }
		glm::vec3 GetMax() const { return m_extents; }

		void SetSize(const glm::vec3& _size) { m_extents = _size * 0.5f; }
		void SetExtents(const glm::vec3& _extents) { m_extents = _extents; }

		void DrawGizmo(const Body* const _body, const glm::vec4& _colour = glm::vec4(1, 0, 0, 1)) const;

	private:
		glm::vec3 m_extents;
	};
}
