#include "Application3D.h"

#include "DEBUG_NEW_LEAK_DETECT.h"

#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Scene.h"

#include "Body.h"
#include "Sphere.h";
#include "AABB.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

Application3D::Application3D() {

}

Application3D::~Application3D() {

}

bool Application3D::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(1000000, 1000000, 1000000, 1000000);

	//// create simple camera transforms
	m_cameraTransform = Transform(glm::vec3(0, 5, 7));
	m_cameraTransform.LookAt(glm::vec3(0));

	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	m_scene = new Physics::Scene((Application3D*)this);
	m_spawnCooldown = 0.0f;
	return true;
}

void Application3D::shutdown() {

	delete m_scene;

	Gizmos::destroy();
}

void Application3D::fixedUpdate(float deltaTime)
{
	m_scene->Update(deltaTime);
}
#include <iostream>
#include "Vector3.h"
void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();
	// wipe the gizmos clean for this frame
	Gizmos::clear();
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	const static float turnSpeed = 2.0f;
	const static float moveSpeed = 12.0f;

	// Camera Rotation
	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		m_cameraTransform.RotateAround(glm::vec3(0), glm::vec3(0, 1, 0), deltaTime * -turnSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		m_cameraTransform.RotateAround(glm::vec3(0), glm::vec3(0, 1, 0), deltaTime * turnSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_cameraTransform.RotateAround(glm::vec3(0), m_cameraTransform.Right(), deltaTime * -turnSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_cameraTransform.RotateAround(glm::vec3(0), m_cameraTransform.Right(), deltaTime * turnSpeed);

	// Camera Distance Translation
	if (input->isKeyDown(aie::INPUT_KEY_KP_ADD))
		m_cameraTransform.MoveTowards(glm::vec3(0), deltaTime * moveSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_KP_SUBTRACT))
		m_cameraTransform.MoveTowards(glm::vec3(0), deltaTime * -moveSpeed);

	// Camera LookAt World Origin
	m_cameraTransform.LookAt(glm::vec3(0));


	// Spawning controls
	if (m_spawnCooldown != 0.0f && m_spawnCooldown > 0.0f) {
		if (m_spawnCooldown - deltaTime <= 0.0f)
			m_spawnCooldown = 0.0f;
		else m_spawnCooldown -= deltaTime;
	}
	if (m_spawnCooldown == 0.0f) {
		bool fired = false;
		bool isBox = false;

		if (input->isKeyDown(aie::INPUT_KEY_LEFT_CONTROL) || input->isKeyDown(aie::INPUT_KEY_RIGHT_CONTROL)) {
			fired = true;
			isBox = false;
		}
		if (input->isKeyDown(aie::INPUT_KEY_LEFT_ALT) || input->isKeyDown(aie::INPUT_KEY_RIGHT_ALT)) {
			fired = true;
			isBox = true;
		}
		if (fired)
		{
			m_spawnCooldown = 0.5f;
			Physics::Body* body = new Physics::Body(15.0f);
			body->SetPosition(m_cameraTransform.Position() + -m_cameraTransform.Forward());
			body->AddVelocity(-m_cameraTransform.Forward() * 20);
			Physics::Shape* shape;
			if (isBox)
				shape = new Physics::AABB(0.66f);
			else
				shape = new Physics::Sphere(0.33f);
			body->SetShape(shape);
			m_scene->AddBody(body);
		}
	}
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	// Draw Physics Gizmos
	m_scene->DrawGizmos();

	Gizmos::draw(m_projectionMatrix * m_cameraTransform.InverseMatrix());
}