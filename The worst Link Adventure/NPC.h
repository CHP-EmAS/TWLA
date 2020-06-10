#pragma once

#include "SFML/Graphics.hpp"

#include "Defines.h"
#include "moveable.h"

#include "Graphic_Manager.h"

#include <list>

class NPC : public moveable
{
public:
	NPC(int spawnPosX,int spawnPosY);

	struct movement
	{
		unsigned short direction;
		short stepAmount;
	};

	bool loadNPC(std::string path);

	void update();
	void interact(interactType interact_Type);
	void draw(sf::RenderWindow& mainWindow);

	void setActiv(bool activ);

	collisionbox *getCollisionbox();
	sf::Vector2i getNPCSize();

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	virtual ~NPC(void);
private:
	short ID;
	short dialogID;

	std::string name;

	Graphic_Manager::tileset tileset;
	int cutSizeX;
	int cutSizeY;
	int cutPosX;
	int cutPosY;

	int spawnPosX;
	int spawnPosY;

	sf::Sprite npcSprite;

	bool interactable;
	bool active;

	std::list<movement> movements;
};

