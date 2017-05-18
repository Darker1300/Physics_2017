#pragma once

#include <glm\vec4.hpp>

namespace Physics {

	class Body;

	enum class ShapeType
	{
		Invalid = -1,
		Point,
		Plane,
		Sphere,
		AABB,
		Box,
		Capsule,
		Ray,
		Mesh,
		MeshConvex,
		Size
	};

	class Shape
	{
	public:
		Shape() = delete;
		Shape(ShapeType _type = ShapeType::Invalid);
		virtual ~Shape();

		ShapeType GetType() const { return m_type; }
		bool isType(const Shape& _other) const { return m_type == _other.m_type; }

		virtual void DrawGizmo(const Body* const _body, const glm::vec4& _colour = glm::vec4(1, 0, 0, 1)) const = 0;

	private:
		ShapeType m_type;
	};

}
