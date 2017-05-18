#pragma once
#include "Shape.h"

namespace Physics {

	class AABB :
		public Shape
	{
	public:
		AABB();
		~AABB();

		void DrawGizmo(const Body* const _body, const glm::vec4& _colour = glm::vec4(1, 0, 0, 1)) const;

	};

}