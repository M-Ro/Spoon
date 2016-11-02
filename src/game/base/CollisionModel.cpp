#include <cmath>

#include "CollisionModel.h"

CollisionModel::CollisionModel(ModelType type)
{
	this->type = type;
}

CollisionModel::CollisionModel(ModelType type, float radius) : CollisionModel(type)
{
	this->radius = radius;
}

CollisionModel::CollisionModel(ModelType type, glm::vec3 size) : CollisionModel(type)
{
	this->size = size;
}

CollisionModel::~CollisionModel()
{

}

bool CollisionModel::Intersects(const glm::vec3 &position, const glm::vec3 &position2, CollisionModel &other)
{
	if(type == ModelType::Box)
	{
		if(other.type == ModelType::Box)
			return AABB2(position, position2, other.size);
		else if(other.type == ModelType::Sphere)
			return AABBSphere(position, position2, other.radius);
	}
	else if(type == ModelType::Sphere)
	{
		if(other.type == ModelType::Box)
			return other.AABBSphere(position2, position, radius);
		else if(other.type == ModelType::Sphere)
			return Sphere2(position, position2, other.radius);
	}

	return false;
}

bool CollisionModel::PointIntersects(const glm::vec3 &position, const glm::vec3 &point)
{
	if(type == ModelType::Box)
	{
		glm::vec3 mins = glm::vec3(position.x-size.x/2, position.y-size.y/2, position.z-size.z/2);
		glm::vec3 maxs = glm::vec3(position.x+size.x/2, position.y+size.y/2, position.z+size.z/2);

		return  (point.x >= mins.x && point.x <= maxs.x) &&
				(point.y >= mins.y && point.y <= maxs.y) &&
				(point.z >= mins.z && point.z <= maxs.z);
	}
	else if(type == ModelType::Sphere)
	{
		float dst = std::sqrt((point.x - position.x) * (point.x - position.x) +
                           	(point.y - position.y) * (point.y - position.y) +
                           	(point.z - position.z) * (point.z - position.z));

		return (dst < radius);
	}

	return false;
}

bool CollisionModel::AABB2(const glm::vec3 &position, const glm::vec3 &origin, const glm::vec3 &size)
{
	glm::vec3 a_mins = glm::vec3(position.x-this->size.x/2, position.y-this->size.y/2, position.z-this->size.z/2);
	glm::vec3 a_maxs = glm::vec3(position.x+this->size.x/2, position.y+this->size.y/2, position.z+this->size.z/2);
	glm::vec3 b_mins = glm::vec3(origin.x-size.x/2, origin.y-size.y/2, origin.z-size.z/2);
	glm::vec3 b_maxs = glm::vec3(origin.x+size.x/2, origin.y+size.y/2, origin.z+size.z/2);

	return  (a_mins.x <= b_maxs.x && a_maxs.x >= b_mins.x) &&
			(a_mins.y <= b_maxs.y && a_maxs.y >= b_mins.y) &&
			(a_mins.z <= b_maxs.z && a_maxs.z >= b_mins.z);
}

bool CollisionModel::AABBSphere(const glm::vec3 &position, const glm::vec3 &origin, const float &r)
{
	glm::vec3 mins = glm::vec3(position.x-this->size.x/2, position.y-this->size.y/2, position.z-this->size.z/2);
	glm::vec3 maxs = glm::vec3(position.x+this->size.x/2, position.y+this->size.y/2, position.z+this->size.z/2);

	float x = glm::max(mins.x, glm::min(origin.x, maxs.x));
	float y = glm::max(mins.y, glm::min(origin.y, maxs.y));
	float z = glm::max(mins.z, glm::min(origin.z, maxs.z));

	// this is the same as isPointInsideSphere
	float dst = std::sqrt((x - origin.x) * (x - origin.x) +
						  (y - origin.y) * (y - origin.y) +
						  (z - origin.z) * (z - origin.z));

	return (dst < r);
}

bool CollisionModel::Sphere2(const glm::vec3 &position, const glm::vec3 &origin, const float &r)
{
	float dst = std::sqrt((position.x - origin.x) * (position.x - origin.x) +
						 (position.y - origin.y) * (position.y - origin.y) +
						 (position.z - origin.z) * (position.z - origin.z));

	return (dst < (radius + r));
}

bool CollisionModel::OnFloor(const glm::vec3 &position, CollisionModel &floor)
{
	if(floor.type == ModelType::Box)
	{
		if(type == ModelType::Box)
		{
			/* Check bottom corners */
			glm::vec3 p[4];
			p[0] = glm::vec3(position.x+size.x/2, position.y-size.y/2, position.z-size.z/2);
			p[1] = glm::vec3(position.x+size.x/2, position.y-size.y/2, position.z+size.z/2);
			p[2] = glm::vec3(position.x-size.x/2, position.y-size.y/2, position.z-size.z/2);
			p[3] = glm::vec3(position.x-size.x/2, position.y-size.y/2, position.z+size.z/2);

			for(int i=0; i<4; i++)
				if(floor.PointIntersects(glm::vec3(0, 0, 0), p[i]))
					return true;
		}
		else if(type == ModelType::Sphere)
		{
			return floor.AABBSphere(glm::vec3(0, 0, 0), position, radius);
		}
	}
	else if(floor.type == ModelType::Complex) // TODO
	{
		return false;
	}

	return false;
}