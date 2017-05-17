#pragma once

enum class ShapeType
{
	Invalid,
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
	Shape(ShapeType _type = ShapeType::Invalid);
	virtual ~Shape();

	bool isType(const Shape& _other) { return m_type == _other.m_type; }

	ShapeType m_type;
};

