#include "Objects.h"

#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Animation_Manager.h"
#include "Weapon_Manager.h"
#include "Player_Manager.h"

Cactus::Cactus(int x, int y,short ID,float damage):InteractivObject(x,y,ID)
{
	objType = ObjectType::Cactus;

	collisionMode = 1;
	interactable = true;

	this->damage = damage;

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		live = 1.5;
		break;
	case NORMAL:
		live = 2.5;
		break;
	case DIFFICULT:
		live = 5;
		break;
	case HARDCORE:
		live = 10;
		break;
	default:
		live = 2.5;
		break;
	}

	tileset = Graphic_Manager::Overworld;

	setObjectSprite(95,tileset);

	cBox.height  = ZOOMSIZE - 7;
	cBox.width = ZOOMSIZE - 4;
	cBoxPosX = 2;
	cBoxPosY = 0;
}

void Cactus::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Collision_Interact)
		{
			Player_Manager::getMe().hurt(damage);
		}
		else if(interact_Type == Hit_Interact)
		{
			live -= Weapon_Manager::getMe().getCurrentInteractWeapon().getDamage();

			Animation_Manager::getMe().doBlinkAnimation(spr_object,sf::Color::Red,sf::Color::White,1,150);
		}
		else if(interact_Type == Explosion_Interact)
		{
			live = 0;
		}

		if(live <= 0)
		{
			interactable = false;
			collisionMode = false;
			Animation_Manager::getMe().doSpriteExplosion(spr_object,8);
			Player_Manager::getMe().updatePlayerHitBoxes();
		}
	}
}

void Cactus::draw(sf::RenderWindow &mainWindow,float x,float y)
{
	if(live > 0 && activ)
	{
		InteractivObject::draw(mainWindow,x,y);
	}
}

std::string Cactus::getMySaveData()
{
	std::string content = std::to_string(live) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Cactus::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_live = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->live = atof(str_live.c_str());

	return false;
}

Cactus::~Cactus(void)
{
}
