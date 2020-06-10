#ifndef COLLISION_DEFINES
#define COLLISION_DEFINES

class InteractivObject;
class NPC;
class Enemy;

enum collisionboxType
{
	None_Collisionbox,
	Standart_Collisionbox,
	Interactiv_Collisionbox,
	Grabable_Collisionbox,
	NPC_Collisionbox,
	Enemy_Collisionbox,
	Player_Collisionbox,
	Interact_Only
};

struct collisionbox
{
	int height;
	int	width;

	int mapPosX;
	int mapPosY;

	collisionboxType type;

	NPC* boundNPC;
	Enemy* boundEnemy;
	InteractivObject* boundObj;
};

#endif