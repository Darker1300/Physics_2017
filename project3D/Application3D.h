#pragma once

#include "Application.h"
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include "Transform.h"

namespace Physics {
	class Scene;
}

class Application3D : public aie::Application {
public:

	Application3D();
	virtual ~Application3D();

	virtual bool startup();
	virtual void shutdown();

	virtual void fixedUpdate(float deltaTime);
	virtual void update(float deltaTime);
	virtual void draw();

protected:
	Transform	m_cameraTransform;
	glm::mat4	m_projectionMatrix;
	Physics::Scene* m_scene;
	float		m_spawnCooldown;
};
