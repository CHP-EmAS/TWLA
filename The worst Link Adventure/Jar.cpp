#include "Objects.h"

#include "Graphic_Manager.h"
#include "Message_Manager.h"
#include "Animation_Manager.h"
#include "Player_Manager.h"
#include "Map_Manager.h"
#include "Sound_Manager.h"

Jar::Jar(int x, int y,short ID,int item):InteractivObject(x,y,ID)
{
	objType = ObjectType::Jar;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	setObjectSprite(431,tileset);
	
	cBox.height = ZOOMSIZE - 10;

	broken = false;
	this->item = item;
}

void Jar::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Key_Interact)
			Message_Manager::getMe().showMsg("Was sich wohl darin befindet?",sf::Color(0,255,255));
		else if(interact_Type == Collision_Interact)
			Message_Manager::getMe().showShortHintMsg("Q - Untersuchen");
		else if(interact_Type == Hit_Interact || 
			interact_Type == Boomerang_Interact || 
			interact_Type == Arrow_Interact || 
			interact_Type == Explosion_Interact || 
			interact_Type == Hammer_Interact)
		{
			broken = true;
			interactable = false;
			collisionMode = false;

			Player_Manager::getMe().updatePlayerHitBoxes();

			if(item == -1)
			{
				int rand = std::rand()%100;

				if(rand < 30)
				{
					Particle_Manager::getMe().spawnItemParticle(0,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
				}
				else if(rand >= 30 && rand < 60)
				{
					Particle_Manager::getMe().spawnItemParticle(1,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
				}
				else if(rand >= 60 && rand < 80)
				{
					Particle_Manager::getMe().spawnItemParticle(45,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
				}
				else if(rand >= 80 && rand < 95)
				{
					if(Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::Bow)) Particle_Manager::getMe().spawnItemParticle(34,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
					else Particle_Manager::getMe().spawnItemParticle(0,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
				}
				else if(rand >= 95 && rand < 99)
				{
					Particle_Manager::getMe().spawnItemParticle(2,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
				}
				else if(rand == 99)
				{
					Particle_Manager::getMe().spawnItemParticle(3,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
				}
			}
			else
			{
				Particle_Manager::getMe().spawnItemParticle(item,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,20);
			}

			setObjectSprite(443,tileset);
		}
	}
}

std::string Jar::getMySaveData()
{
	std::string content = std::to_string(broken) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Jar::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->broken = bool(atoi(str_broken.c_str()));

	if(broken)
	{
		setObjectSprite(443,tileset);
	}

	return false;
}

Jar::~Jar(void)
{

}