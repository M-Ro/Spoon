#include "Projectile.h"
#include "../../../auxiliary/Time.h"

Projectile::Projectile(Entity *owner) : Entity()
{
	classname = "projectile";
	moveType = MovementType::Manual;
	spawntime = Time::GetCurrentTimeMillis();
	this->owner = owner;
}

void Projectile::Update(float deltaTime)
{
	if(Time::GetCurrentTimeMillis() > spawntime + lifetime)
		Die();

	velocity = dir * speed;

	position += velocity * deltaTime;
}

void Projectile::Touch(Entity *other)
{
	if(other == owner)
		return;
	if(other->team == team)
		return;

	other->Hurt(this, dmg);
	Die();
}

void Projectile::Die()
{
	destroy = true;
}