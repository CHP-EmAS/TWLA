#ifndef MAP_DEFINES
#define MAP_DEFINES

//** Hitbox Colors **//
#define HITBOX_COLOR_ALPHA 255

#define HB_PLAYER_COLOR sf::Color(255,140,0,HITBOX_COLOR_ALPHA)
#define HB_OBJEKT_COLOR sf::Color(75,75,255,HITBOX_COLOR_ALPHA)
#define HB_COLLISION_TILE_COLOR sf::Color(255,75,75,HITBOX_COLOR_ALPHA)
#define HB_FREE_TILE_COLOR sf::Color(255,255,255,HITBOX_COLOR_ALPHA)

//***Tile Settings***//
#define NONE 0
#define INTERACTIV 1
#define WATER 2
#define ABYSS 3
#define LAVA 4

class InteractivObject;

enum interactType
{
	Standart_Interact = -1,
	Collision_Interact,
	Key_Interact,
	Explosion_Interact,
	Hit_Interact,
	Arrow_Interact,
	Boomerang_Interact,
	Laser_Interact,
	Hammer_Interact
};

struct spawnPoint
{
	int x;
	int y;
};

struct tile
{
	int x;
	int y;

	int tileNumber;
	bool drawTileSprite;

	int settings;

	InteractivObject *boundObject;
};

struct fullCommand
{
	std::string command;
	std::string content;
};

#endif