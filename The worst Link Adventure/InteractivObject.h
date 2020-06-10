#pragma once

#include <list>
#include <string>
#include "SFML/Graphics.hpp"

#include "Defines.h"

#include "Graphic_Manager.h"
#include "Collision_System.h"

class InteractivObject
{
public:
	InteractivObject();
	InteractivObject(int posX,int posY, short ID);

	enum ObjectType
	{
		Break_Abyss,
		Bush,
		Button,
		Cactus,
		Chest,
		Command_Door,
		Door,
		Fragment_Inserter,
		Global_Wall,
		Grabable_Tile,
		Jar,
		Key_Stone,
		Portal,
		Secret_Bush,
		Sign,
		Stone,
		Teleporter,
		Trigger,
		Unknown = -1
	};

	//getter
	short getID();

	ObjectType getObjectType();

	int getPosX();
	int getPosY();
	
	bool isActive();
	bool isInteractable();
	bool getCollisionMode();

	sf::Sprite &getObjectSprite();

	int getTileset();
	int getTileNumber();

	InteractivObject *getMyPointer();

	collisionbox* getCollisionBox();

	//setter
	void setPosition(int posX,int posY);

	void setInteract(bool interact);
	void setCollisionMode(bool collision);
	void setActiv(bool activ);

	void setObjectSprite(int tile,Graphic_Manager::tileset tileset);

	void setMapID(short ID);
	
	//Draw
	virtual void draw(sf::RenderWindow &mainWindow,float x,float y);

	//Interact virtual to all interactiv Objects
	virtual void interact(interactType interact_Type);

	virtual void executeObjectCommand(std::string command);

	virtual std::string getMySaveData();
	virtual bool loadMySaveData(std::string data);

	virtual ~InteractivObject(void);
protected:

	//Objekt ID
	short ID;
	short mapID;
	ObjectType objType;

	//Position on Map
	int posX;
	int posY;

	bool collisionMode; // 0 = keine Kollision, 1 = Kollision
	bool interactable; // is interactable?
	bool activ;

	sf::Sprite spr_object; //Sprite

	Graphic_Manager::tileset tileset; //wich Tileset is used
	int tileNumber;

	collisionbox cBox;
	int cBoxPosX;
	int cBoxPosY;
};

