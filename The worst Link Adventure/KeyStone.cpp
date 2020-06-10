#include "Objects.h"

#include "Message_Manager.h"
#include "Quest_Manager.h"
#include "Player_Manager.h"

KeyStone::KeyStone(int x, int y,short ID,short type,int keyNumber) : InteractivObject(x,y,ID)
{
	objType = ObjectType::Key_Stone;

	tileset = Graphic_Manager::Overworld;

	switch(type)
	{
	case 0:
		setObjectSprite(419,tileset);
		break;
	default:
		setObjectSprite(419,tileset);
		break;
	}

	this->keyNumber = keyNumber;

	collisionMode = 1;
	interactable = true;

	this->type = type;

	cBox.height = ZOOMSIZE - 10;

	open = false;
}

void KeyStone::interact(interactType interact_Type)
{
	if(interactable && interact_Type == Key_Interact && activ)
	{
		if(Quest_Manager::getMe().isQuestCommandDone("found-" + std::to_string(keyNumber)))
		{
			std::string command = "open-";
			command += std::to_string(mapID) + "-";
			command += std::to_string(ID);

			activ = false;
			interactable = false;
			collisionMode = false;

			Player_Manager::getMe().updatePlayerHitBoxes();
		}
		else
		{
			Message_Manager::getMe().showMsg("Ich hab keinen passenden Schlüssel dafür.",sf::Color(0,255,255));
		}
	}
	else if(interactable && interact_Type == Collision_Interact)
	{
		Message_Manager::getMe().showShortHintMsg("Q - Öffnen");
	}
}

std::string KeyStone::getMySaveData()
{
	std::string content = std::to_string(open) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool KeyStone::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->open = bool(atoi(str_broken.c_str()));

	if(open)
	{
		activ = false;
		interactable = false;
		collisionMode = false;
	}

	return false;
}

KeyStone::~KeyStone(void)
{

}