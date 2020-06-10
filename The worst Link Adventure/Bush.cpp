#include "Objects.h"

#include "Graphic_Manager.h"
#include "Message_Manager.h"
#include "Animation_Manager.h"
#include "Player_Manager.h"
#include "Map_Manager.h"
#include "Sound_Manager.h"

Bush::Bush(int x, int y,short ID,short type):InteractivObject(x,y,ID)
{
	objType = ObjectType::Bush;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	setObjectSprite(401,tileset);
	
	cBox.height = ZOOMSIZE - 10;

	broken = false;
	this->type = type;
}

void Bush::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Key_Interact)
			Message_Manager::getMe().showMsg("Mit etwas scharfen würde ich das hier klein hacken können",sf::Color(0,255,255));
		else if(interact_Type == Collision_Interact)
			Message_Manager::getMe().showShortHintMsg("Q - Untersuchen");
		else if(interact_Type == Hit_Interact)
		{
			broken = true;
			interactable = false;
			collisionMode = false;

			Player_Manager::getMe().updatePlayerHitBoxes();
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Grass_Cut);

			sf::Sprite leaf;
			leaf.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
			leaf.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,292));
			leaf.setOrigin(8,8);
			leaf.setScale(1.5,1.5);

			int maxLeaf = rand() % 5 + 1;

			for(int i = 0; i < maxLeaf; i++)
			{
				leaf.setPosition(sf::Vector2f(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i((posX*32)+rand()%32,(posY*32)+rand()%16))));
				leaf.setRotation(rand()%360);
				Particle_Manager::getMe().addParticle(leaf,300,50,rand()%3-1,1,true,true);
			}

			if(type == 1)
			{
				setObjectSprite(88,tileset);
				Map_Manager::getMe().getCurrentMap().changeTileSetting(this->posX,this->posY,ABYSS);
			}
			else
			{
				setObjectSprite(400,tileset);
				drop();
			}

			Player_Manager::getMe().getGameStats().addCuttetGrass();
		}
	}
}

void Bush::drop()
{
	int rand = std::rand()%100;

	if(rand >= 10 && rand <= 40)
	{
		Particle_Manager::getMe().spawnItemParticle(0,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
	}
	else if(rand > 40 && rand <= 80)
	{
		int playerLive = int(Player_Manager::getMe().getGameHUD().getLive());

		if(playerLive > 2)
		{
			int hRand = std::rand()%(playerLive);
			if(hRand == 0 || hRand == 1) Particle_Manager::getMe().spawnItemParticle(45,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
			else Particle_Manager::getMe().spawnItemParticle(0,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
		}
		else
		{
			Particle_Manager::getMe().spawnItemParticle(45,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
		}
	}
	else if(rand > 80)
	{
		if(Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::Bow)) Particle_Manager::getMe().spawnItemParticle(34,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
		else Particle_Manager::getMe().spawnItemParticle(0,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
	}
}

std::string Bush::getMySaveData()
{
	std::string content = std::to_string(broken) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Bush::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->broken = bool(atoi(str_broken.c_str()));

	if(broken)
	{
		if(type == 1)
		{
			setObjectSprite(88,tileset);
			Map_Manager::getMe().getCurrentMap().changeTileSetting(this->posX,this->posY,ABYSS);
		}
		else
		{
			setObjectSprite(400,tileset);
		}
	}

	return false;
}

Bush::~Bush(void)
{

}
