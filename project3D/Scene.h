#pragma once

#include <vector>
#include <glm\vec3.hpp>

class Application3D;

namespace Physics {

	class Body;

	class Scene
	{
	public:
		Scene(Application3D* _app);
		~Scene();

		void Start();
		void Update(float _deltaTime);
		void DrawGizmos() const;
		void Clear();

		void AddBody(Body* _body);

		glm::vec3 m_gravity;
		std::vector<Body*> m_objects;

	private:
		Application3D* m_app;
	};

}
