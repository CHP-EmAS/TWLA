#include "Objects.h"

#include "Message_Manager.h"
#include "Graphic_Manager.h"
#include "Quest_Manager.h"
#include "Sound_Manager.h"
#include "Player_Manager.h"
#include "Particle_Manager.h"
#include "Animation_Manager.h"

#include "Item_List.h"

Chest::Chest(int x, int y,short ID, int keyNumber,int itemNumber):InteractivObject(x,y,ID)
{
	objType = ObjectType::Chest;

	collisionMode = 1;
	interactable = true;

	isOpen = false;
	item = itemNumber;

	this->keyNumber = keyNumber;

	tileset = Graphic_Manager::Overworld;

	setObjectSprite(155,tileset);

	cBox.height = ZOOMSIZE - 10;
}

void Chest::interact(interactType interact_Type)
{
	if(activ)
	{
		if(interactable && interact_Type == Key_Interact)
		{
			open(keyNumber);

			std::string command = "open-";
			command += std::to_string(mapID) + "-";
			command += std::to_string(ID);

			Quest_Manager::getMe().doQuestCommand(command);
		}
		else if(interactable && interact_Type == Collision_Interact)
			Message_Manager::getMe().showShortHintMsg("Q - Öffnen");
	}
}

void Chest::open(int key)
{
	if(Quest_Manager::getMe().isQuestCommandDone("found-" + std::to_string(keyNumber)) || key == -1)
	{
		setObjectSprite(167,tileset);
		interactable = false;
		isOpen = true;

		Player_Manager::getMe().give(item,1);
	}
	else
	{
		Message_Manager::getMe().showMsg("Ich hab keinen passenden Schlüssel dafür.",sf::Color(0,255,255));
	}
}

std::string Chest::getMySaveData()
{
	std::string content = std::to_string(isOpen) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Chest::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_isOpen = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->isOpen = bool(atoi(str_isOpen.c_str()));

	if(isOpen)
		setObjectSprite(167,tileset);

	return false;
}

Chest::~Chest(void)
{

}
