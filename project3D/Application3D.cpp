#include "Application3D.h"

#include "DEBUG_NEW_LEAK_DETECT.h"

#include "Gizmos.h"
#include "Input.h"
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>

#include "Scene.h"

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
	Gizmos::create(10000, 10000, 10000, 10000);

	//// create simple camera transforms
	//m_cameraPos = glm::vec3(10);
	//m_viewMatrix = glm::lookAt(m_cameraPos, glm::vec3(0), glm::vec3(0, 1, 0));
	m_cameraTransform = Transform(glm::vec3(0, 5, 10));
	m_cameraTransform.LookAt(glm::vec3(0));

	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	m_scene = new Physics::Scene((Application3D*)this);

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

void Application3D::update(float deltaTime) {

	// query time since application started
	float time = getTime();

	// rotate camera
	//m_cameraMatrix = glm::lookAt(vec3(glm::sin(time) * 30, 10, glm::cos(time) * 30),
	//	vec3(0), vec3(0, 1, 0));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	//// demonstrate a few shapes
	//Gizmos::addAABBFilled(vec3(0), vec3(1), vec4(0, 0.5f, 1, 0.25f));
	//Gizmos::addSphere(vec3(5, 0, 5), 1, 8, 8, vec4(1, 0, 0, 0.5f));
	//Gizmos::addRing(vec3(5, 0, -5), 1, 1.5f, 8, vec4(0, 1, 0, 1));
	//Gizmos::addDisk(vec3(-5, 0, 5), 1, 16, vec4(1, 1, 0, 1));
	//Gizmos::addArc(vec3(-5, 0, -5), 0, 2, 1, 8, vec4(1, 0, 1, 1));

	//mat4 t = glm::rotate(time, glm::normalize(vec3(1, 1, 1)));
	//t[3] = vec4(-2, 0, 0, 1);
	//Gizmos::addCylinderFilled(vec3(0), 0.5f, 1, 5, vec4(0, 1, 1, 1), &t);


	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	const static float turnSpeed = 2.0f;

	if (input->isKeyDown(aie::INPUT_KEY_LEFT))
		m_cameraTransform.RotateAround(glm::vec3(0), glm::vec3(0, 1, 0), deltaTime * -turnSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
		m_cameraTransform.RotateAround(glm::vec3(0), glm::vec3(0, 1, 0), deltaTime * turnSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_UP))
		m_cameraTransform.RotateAround(glm::vec3(0), m_cameraTransform.Right(), deltaTime * -turnSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_DOWN))
		m_cameraTransform.RotateAround(glm::vec3(0), m_cameraTransform.Right(), deltaTime * turnSpeed);

	const static float moveSpeed = 12.0f;

	if (input->isKeyDown(aie::INPUT_KEY_KP_ADD))
		m_cameraTransform.MoveTowards(glm::vec3(0), deltaTime * moveSpeed);
	if (input->isKeyDown(aie::INPUT_KEY_KP_SUBTRACT))
		m_cameraTransform.MoveTowards(glm::vec3(0), deltaTime * -moveSpeed);

	m_cameraTransform.LookAt(glm::vec3(0));
}

void Application3D::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// update perspective in case window resized
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f,
		getWindowWidth() / (float)getWindowHeight(),
		0.1f, 1000.f);

	m_scene->DrawGizmos();

	Gizmos::draw(m_projectionMatrix * m_cameraTransform.InverseMatrix());
	//Gizmos::draw(m_projectionMatrix * m_viewMatrix);
}