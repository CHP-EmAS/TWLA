#include "Objects.h"

#include "Map_Manager.h"
#include "Player_Manager.h"


Teleporter::Teleporter(int x, int y,short ID, int portSpawnPointNumber):InteractivObject(x,y,ID)
{
	objType = ObjectType::Teleporter;

	this->portSpawnPointNumber = portSpawnPointNumber;

	collisionMode = 0;
	interactable = true;

	setObjectSprite(0,Graphic_Manager::Overworld);
	spr_object.setColor(sf::Color::Transparent);
}

void Teleporter::draw(sf::RenderWindow &mainWindow)
{
	;
}

void Teleporter::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Collision_Interact)
		{
			teleport();
		}
	}
}

void Teleporter::teleport()
{
	//Ausblenden
	Screen_Manager::getMe().fadeOut();

	//neue map setzen
	int tpX = Map_Manager::getMe().getCurrentMap().getSpawnPointX(portSpawnPointNumber);
	int tpY = Map_Manager::getMe().getCurrentMap().getSpawnPointY(portSpawnPointNumber);

	Player_Manager::getMe().tp(tpX,tpY);

	//Einblenden
	Screen_Manager::getMe().fadeIn();
}

Teleporter::~Teleporter(void)
{
}