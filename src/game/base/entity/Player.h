#pragma once
#include <SDL2/SDL_net.h>
#include "Entity.h"
#include "../../../input/InputHandler.h"
#include <glm/glm.hpp>

class Player : public Entity
{
	friend class GameName;
	friend class GameClient;
public:

	Player();
	virtual ~Player();

	void Update(float deltaTime);

	void HandlePMove(float deltaTime);

	void Draw() {}

	void SendEntity() {}
	
	void Hurt(Entity *attacker, float dmg);

	void Die();

	void Touch(Entity *other);

	IPaddress * myAddress;
private:

	void HandlePlayerInput(float deltaTime);

	void ProjectView();

	glm::vec2 cam_rot;

	InputHandler *input;

	// For movement
	glm::vec3 up;
	glm::vec3 right;
	glm::vec3 dir;


	glm::vec3 idir; // Intended direction vector from player input
	long lastJumpTime; 

};