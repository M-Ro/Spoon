#include "../Projectile.h"

class Spoon : public Projectile
{
public:
	void Update(float deltaTime);
	Spoon(Entity *owner);
	~Spoon();
	float nextupdate = 0.0f;
};