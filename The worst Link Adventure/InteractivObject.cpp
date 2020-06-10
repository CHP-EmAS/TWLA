#include "InteractivObject.h"

#include "Graphic_Manager.h"
#include "Command_Manager.h"

InteractivObject::InteractivObject()
{
	NEWOUT("Neue Objekt wurde erstellt! (" << this << " - p)")

	this->posX = -1;
	this->posY = -1;

	collisionMode = false;
	interactable = false;

	//set Error Tile if no Tile is set
	tileset = Graphic_Manager::Overworld;
	tileNumber = 275;
	setObjectSprite(tileNumber,tileset);

	cBox.height = 32;
	cBox.width = 32;
	cBox.mapPosX = posX;
	cBox.mapPosY = posY;
	cBox.type = Interactiv_Collisionbox;
	cBox.boundObj = this;
	cBox.boundEnemy = nullptr;
	cBox.boundNPC = nullptr;

	cBoxPosX = 0;
	cBoxPosY = 0;

	ID = -1;
}

InteractivObject::InteractivObject(int posX,int posY,short ID)
{
	NEWOUT("Neue Objekt wurde erstellt! (" << this << ",X:" << posX << ",Y:" << posY << ")")

	this->ID = ID;
	this->objType = ObjectType::Unknown;

	this->posX = posX;
	this->posY = posY;

	collisionMode = false;
	interactable = false;
	activ = true;

	//set Error Tile if no Tile is set
	tileset = Graphic_Manager::Overworld;
	tileNumber = 275;
	setObjectSprite(tileNumber,tileset);

	cBox.height = 32;
	cBox.width = 32;
	cBox.mapPosX = posX;
	cBox.mapPosY = posY;
	cBox.type = Interactiv_Collisionbox;
	cBox.boundObj = this;
	cBox.boundNPC = nullptr;
	cBox.boundEnemy = nullptr;

	cBoxPosX = 0;
	cBoxPosY = 0;
}

// *** getter *** //
short InteractivObject::getID()
{
	return ID;
}

InteractivObject::ObjectType InteractivObject::getObjectType()
{
	return objType;
}

int InteractivObject::getPosX()
{
	return posX;
}

int InteractivObject::getPosY()
{
	return posY;
}

bool InteractivObject::isActive()
{
	return activ;
}

bool InteractivObject::isInteractable()
{
	if(!activ) return false;

	return interactable;
}

bool InteractivObject::getCollisionMode()
{
	if(!activ) return false;

	return collisionMode;
}

sf::Sprite &InteractivObject::getObjectSprite()
{
	return spr_object;
}

int InteractivObject::getTileset()
{
	return tileset;
}

int InteractivObject::getTileNumber()
{
	return tileNumber;
}

InteractivObject *InteractivObject::getMyPointer()
{
	return this;
}

collisionbox *InteractivObject::getCollisionBox()
{
	cBox.mapPosX = posX * ZOOMSIZE + cBoxPosX;
	cBox.mapPosY = posY * ZOOMSIZE + cBoxPosY;

	return &cBox;
}

// *** setter *** //
void InteractivObject::setPosition(int posX,int posY)
{
	this->posX = posX;
	this->posY = posY;
}

void InteractivObject::setInteract(bool interact)
{
	interactable = interact;
}

void InteractivObject::setCollisionMode(bool collision)
{
	collisionMode = collision;
}

void InteractivObject::setActiv(bool activ)
{
	this->activ = activ;
}

void InteractivObject::setObjectSprite(int tile,Graphic_Manager::tileset tileset)
{
	this->tileset = tileset;
	tileNumber = tile;

	spr_object.setTexture(Graphic_Manager::getMe().getTilesetTexture(tileset));
	spr_object.setScale(SCALE,SCALE);
	spr_object.setTextureRect(Graphic_Manager::getMe().getIntRect(tileset,tile));
}

void InteractivObject::setMapID(short ID)
{
	mapID = ID;
}

// *** DRAW *** //
void InteractivObject::draw(sf::RenderWindow &mainWindow,float x,float y)
{
	if(activ)
	{
		spr_object.setPosition(x,y);
		mainWindow.draw(spr_object);
	}
}

//Interact virtual to all interactiv Objects
void InteractivObject::interact(interactType interact_Type)
{
	
}

void InteractivObject::executeObjectCommand(std::string command)
{
	WOUT("Objekt unterstützt den 'execute' - Befehl nicht! (" << this->ID << "," << command << ")")
}

std::string InteractivObject::getMySaveData()
{
	std::string content = "";

	content += std::to_string(this->activ) + ",";
	content += std::to_string(this->interactable) + ",";
	content += std::to_string(this->collisionMode) + ",";

	return content;
}

bool InteractivObject::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_active = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_interactable = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_collisionMode = data.substr(0,pos);

	this->activ = bool(atoi(str_active.c_str()));
	this->interactable = bool(atoi(str_interactable.c_str()));
	this->collisionMode = bool(atoi(str_collisionMode.c_str()));
	
	return true;
}

//Destruct
InteractivObject::~InteractivObject(void)
{
	DELOUT("Ein Objekt wurde geloescht!\t(" << this << ")")
}
