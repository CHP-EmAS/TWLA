#include "Objects.h"

#include "Message_Manager.h"
#include "Map_Manager.h"
#include "Player_Manager.h"
#include "Dialog_Manager.h"

GlobalWall::GlobalWall(int x, int y,short ID,bool open) : InteractivObject(x,y,ID)
{
	objType = ObjectType::Global_Wall;

	setObjectSprite(129,Graphic_Manager::Turm_Dungeon);

	setOpen(open);
}

void GlobalWall::interact(interactType interact_Type)
{
	if(interactable && interact_Type == Key_Interact)
	{
		Message_Manager::getMe().showMsg("Es sieht so aus als wäre diese Wand mit einerart Machanismus verbunden!",sf::Color(0,255,255));
	}
	else if(interactable && interact_Type == Collision_Interact)
	{
		Message_Manager::getMe().showShortHintMsg("Q - Untersuchen");
	}
}

void GlobalWall::executeObjectCommand(std::string command)
{
	if(command == "toggle")
	{
		setOpen(!open);
		Player_Manager::getMe().updatePlayerHitBoxes();
	}
}

std::string GlobalWall::getMySaveData()
{
	std::string content = std::to_string(open) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool GlobalWall::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->open = bool(atoi(str_broken.c_str()));

	setOpen(open);

	return true;
}

void GlobalWall::setOpen(bool open)
{
	this->open = open;

	if(open)
	{
		setObjectSprite(129,Graphic_Manager::Turm_Dungeon);
		setInteract(false);
		setCollisionMode(false);
	}
	else
	{
		setObjectSprite(130,Graphic_Manager::Turm_Dungeon);
		setInteract(true);
		setCollisionMode(true);
	}
}

GlobalWall::~GlobalWall(void)
{

}