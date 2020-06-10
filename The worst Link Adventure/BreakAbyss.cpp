#include "Objects.h"

#include "Graphic_Manager.h"
#include "Message_Manager.h"
#include "Animation_Manager.h"
#include "Player_Manager.h"
#include "Map_Manager.h"
#include "Sound_Manager.h"

BreakAbyss::BreakAbyss(int x, int y,short ID):InteractivObject(x,y,ID)
{
	objType = ObjectType::Break_Abyss;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	setObjectSprite(454,tileset);
	
	setCollisionMode(false);

	broken = false;
}

void BreakAbyss::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Collision_Interact)
		{
			if(!broken)
			{
				broken = true;
				setObjectSprite(455,tileset);
			}
			else
			{
				Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Fall);
				Player_Manager::getMe().doFallAni(posX,posY);
				Player_Manager::getMe().hurt(0.5);
			}
		}
	}
}

std::string BreakAbyss::getMySaveData()
{
	std::string content = std::to_string(broken) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool BreakAbyss::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->broken = bool(atoi(str_broken.c_str()));

	if(broken)
	{
		setObjectSprite(455,tileset);
	}

	return false;
}

BreakAbyss::~BreakAbyss(void)
{

}