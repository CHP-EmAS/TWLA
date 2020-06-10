#include "Objects.h"

#include "Map_Manager.h"
#include "Message_Manager.h"
#include "Graphic_Manager.h"
#include "Quest_Manager.h"
#include "Sound_Manager.h"
#include "Screen_Manager.h"

Door::Door(int x, int y,short ID, int keyNumber, std::string portWorld, int portSpawnPointNumber):InteractivObject(x,y,ID)
{
	objType = ObjectType::Door;

	this->portWorld = portWorld;
	this->portSpawnPointNumber = portSpawnPointNumber;
	this->keyNumber = keyNumber;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	cBox.height = ZOOMSIZE - 14;

	setObjectSprite(411,tileset);
}

void Door::interact(interactType interact_Type)
{
	if(activ)
	{
		if(interactable && interact_Type == Key_Interact)
		{
			std::string command = "open-";
			command += std::to_string(mapID) + "-";
			command += std::to_string(ID);

			Quest_Manager::getMe().doQuestCommand(command);

			open(keyNumber);
		}
		else if(interactable && interact_Type == Collision_Interact)
			Message_Manager::getMe().showShortHintMsg("Q - Öffnen");
	}
}

void Door::open(int key)
{
	if(Quest_Manager::getMe().isQuestCommandDone("found-" + std::to_string(keyNumber)) || key == -1)
	{
		//Tür öffnen
		setObjectSprite(71,tileset);

		//Ausblenden
		Screen_Manager::getMe().fadeOut();

		//Tür schließen
		setObjectSprite(411,tileset);

		//Neue map setzen
		Map_Manager::getMe().changeCurrentMap(portWorld,portSpawnPointNumber);

		//Einblenden
		Screen_Manager::getMe().fadeIn();
	}
	else
	{
		Message_Manager::getMe().showMsg("Ich hab keinen passenden Schlüssel dafür.",sf::Color(0,255,255));
	}
}

Door::~Door(void)
{
}
