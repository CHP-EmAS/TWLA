#include "Objects.h"

#include "Map_Manager.h"
#include "Message_Manager.h"
#include "Screen_Manager.h"

Portal::Portal(int x, int y,short ID, std::string portWorld, int portSpawnPointNumber):InteractivObject(x,y,ID)
{
	objType = ObjectType::Portal;

	this->portWorld = portWorld;
	this->portSpawnPointNumber = portSpawnPointNumber;

	collisionMode = 0;
	interactable = true;

	setObjectSprite(0,Graphic_Manager::Overworld);
	spr_object.setColor(sf::Color::Transparent);
}

void draw(sf::RenderWindow &mainWindow)
{
	;
}

void Portal::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Collision_Interact)
		{
			teleport();
		}
	}
}

void Portal::teleport()
{
	//Ausblenden
	Screen_Manager::getMe().fadeOut();

	//neue map setzen
	Map_Manager::getMe().changeCurrentMap(portWorld,portSpawnPointNumber);

	//Einblenden
	Screen_Manager::getMe().fadeIn();
}

Portal::~Portal(void)
{
}

