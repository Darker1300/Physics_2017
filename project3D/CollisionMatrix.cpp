#include "Collision.h"

using namespace Physics;
using namespace Collision;

// Create an static array of all collision functions
// This is ordered based on Shape::ShapeType
const CollisionFunction Collision::CollisionMatrix[] = {
	/*Point		&& Point		*/	nullptr,
	/*Point		&& Plane		*/	nullptr,
	/*Point		&& Sphere		*/	nullptr,
	/*Point		&& AABB			*/	nullptr,
	/*Point		&& Box			*/	nullptr,
	/*Point		&& Capsule		*/	nullptr,
	/*Point		&& Ray			*/	nullptr,
	/*Point		&& Mesh			*/	nullptr,
	/*Point		&& MeshConvex	*/	nullptr,

	/*Plane		&& Point		*/	nullptr,
	/*Plane		&& Plane		*/	nullptr,
	/*Plane		&& Sphere		*/	TestPlaneSphere,
	/*Plane		&& AABB			*/	nullptr,
	/*Plane		&& Box			*/	nullptr,
	/*Plane		&& Capsule		*/	nullptr,
	/*Plane		&& Ray			*/	nullptr,
	/*Plane		&& Mesh			*/	nullptr,
	/*Plane		&& MeshConvex	*/	nullptr,

	/*Sphere	&& Point		*/	nullptr,
	/*Sphere	&& Plane		*/	TestSpherePlane,
	/*Sphere	&& Sphere		*/	nullptr,
	/*Sphere	&& AABB			*/	nullptr,
	/*Sphere	&& Box			*/	nullptr,
	/*Sphere	&& Capsule		*/	nullptr,
	/*Sphere	&& Ray			*/	nullptr,
	/*Sphere	&& Mesh			*/	nullptr,
	/*Sphere	&& MeshConvex	*/	nullptr,

	/*AABB		&& Point		*/	nullptr,
	/*AABB		&& Plane		*/	nullptr,
	/*AABB		&& Sphere		*/	nullptr,
	/*AABB		&& AABB			*/	nullptr,
	/*AABB		&& Box			*/	nullptr,
	/*AABB		&& Capsule		*/	nullptr,
	/*AABB		&& Ray			*/	nullptr,
	/*AABB		&& Mesh			*/	nullptr,
	/*AABB		&& MeshConvex	*/	nullptr,

	/*Box		&& Point		*/	nullptr,
	/*Box		&& Plane		*/	nullptr,
	/*Box		&& Sphere		*/	nullptr,
	/*Box		&& AABB			*/	nullptr,
	/*Box		&& Box			*/	nullptr,
	/*Box		&& Capsule		*/	nullptr,
	/*Box		&& Ray			*/	nullptr,
	/*Box		&& Mesh			*/	nullptr,
	/*Box		&& MeshConvex	*/	nullptr,

	/*Capsule	&& Point		*/	nullptr,
	/*Capsule	&& Plane		*/	nullptr,
	/*Capsule	&& Sphere		*/	nullptr,
	/*Capsule	&& AABB			*/	nullptr,
	/*Capsule	&& Box			*/	nullptr,
	/*Capsule	&& Capsule		*/	nullptr,
	/*Capsule	&& Ray			*/	nullptr,
	/*Capsule	&& Mesh			*/	nullptr,
	/*Capsule	&& MeshConvex	*/	nullptr,

	/*Ray		&& Point		*/	nullptr,
	/*Ray		&& Plane		*/	nullptr,
	/*Ray		&& Sphere		*/	nullptr,
	/*Ray		&& AABB			*/	nullptr,
	/*Ray		&& Box			*/	nullptr,
	/*Ray		&& Capsule		*/	nullptr,
	/*Ray		&& Ray			*/	nullptr,
	/*Ray		&& Mesh			*/	nullptr,
	/*Ray		&& MeshConvex	*/	nullptr,

	/*Mesh		&& Point		*/	nullptr,
	/*Mesh		&& Plane		*/	nullptr,
	/*Mesh		&& Sphere		*/	nullptr,
	/*Mesh		&& AABB			*/	nullptr,
	/*Mesh		&& Box			*/	nullptr,
	/*Mesh		&& Capsule		*/	nullptr,
	/*Mesh		&& Ray			*/	nullptr,
	/*Mesh		&& Mesh			*/	nullptr,
	/*Mesh		&& MeshConvex	*/	nullptr,

	/*MeshConvex&& Point		*/	nullptr,
	/*MeshConvex&& Plane		*/	nullptr,
	/*MeshConvex&& Sphere		*/	nullptr,
	/*MeshConvex&& AABB			*/	nullptr,
	/*MeshConvex&& Box			*/	nullptr,
	/*MeshConvex&& Capsule		*/	nullptr,
	/*MeshConvex&& Ray			*/	nullptr,
	/*MeshConvex&& Mesh			*/	nullptr,
	/*MeshConvex&& MeshConvex	*/	nullptr,
};
