#pragma once

#include <vector>
#include <glm\vec3.hpp>

class PhysicsObject;

class Scene
{
public:
	Scene();
	~Scene();

	void Update(float _deltaTime);

	glm::vec3 m_gravity;
	std::vector<PhysicsObject*> m_objects;
};
