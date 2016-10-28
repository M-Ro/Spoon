#include "entity.h"

class Projectile : public Entity
{
public:

	glm::vec3 dir;
	float dmg;
	long lifetime; // milliseconds
	float speed;

	Projectile(Entity *owner);
	virtual ~Projectile() {}

	virtual void Touch(Entity *other);

	virtual void Update(float deltaTime);

	virtual void Hurt(Entity *attacker, float dmg) {};

	virtual void Die();

protected:
	long spawntime;
};