#pragma once

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

private:
	ShapeType m_type;
};

