#pragma once

#include <glm/glm.hpp>

class CollisionModel
{
public:
	enum ModelType
	{
		Box,
		Sphere,
		Complex // Polygonal - Not implemented yet
	};

	CollisionModel() : CollisionModel(ModelType::Box) {};
	CollisionModel(ModelType type);
	CollisionModel(ModelType type, float radius);
	CollisionModel(ModelType type, glm::vec3 size);

	~CollisionModel();

	bool Intersects(const glm::vec3 &position, const glm::vec3 &position2, CollisionModel &other);

	//void ReadGeometry(); // Todo: Read complex geometry as collision model

	/* Collision checking functions. Position is the current position of the collision model */
	bool PointIntersects(const glm::vec3 &position, const glm::vec3 &point);

	bool AABB2(const glm::vec3 &position, const glm::vec3 &origin, const glm::vec3 &size);

	bool AABBSphere(const glm::vec3 &position, const glm::vec3 &origin, const float &r);

	bool Sphere2(const glm::vec3 &position, const glm::vec3 &origin, const float &r);

	// Returns true if bottom of collision intersects floor
	bool OnFloor(const glm::vec3 &position, CollisionModel &floor);

	ModelType type;

	float radius; // Radius of Sphere
	glm::vec3 size; // Size of aabb bbox
};