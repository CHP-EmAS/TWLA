#include "Objects.h"

#include "Graphic_Manager.h"
#include "Message_Manager.h"
#include "Animation_Manager.h"
#include "Player_Manager.h"

Stone::Stone(int x, int y,short ID,short type):InteractivObject(x,y,ID)
{
	objType = ObjectType::Stone;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	this->type = type;

	if(type == 0)
	{
		setObjectSprite(143,tileset);
	}
	else if(type == 1)
	{
		setObjectSprite(442,tileset);
	}
	
	cBox.height = ZOOMSIZE - 5;

	broken = false;
}

void Stone::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Key_Interact)
		{
			if(type == 0)
			{
				Message_Manager::getMe().showMsg("Ich brauche was Schweres um diesen Brocken zu zerstören",sf::Color(0,255,255));
			}
			else if(type == 1)
			{
				Message_Manager::getMe().showMsg("Diesen Fels ist zu schwerzum Schieben, ich muss ihn sprengen!",sf::Color(0,255,255));
			}
		}
		else if(interact_Type == Collision_Interact)
		{
			Message_Manager::getMe().showShortHintMsg("Q - Untersuchen");
		}
		else if(interact_Type == Explosion_Interact)
		{
			broken = true;
			interactable = false;
			collisionMode = false;
			setObjectSprite(399,tileset);
			Player_Manager::getMe().updatePlayerHitBoxes();
			Particle_Manager::getMe().spawnItemParticle(rand()%4,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
		}
		else if(interact_Type == Hammer_Interact)
		{
			if(type == 0)
			{
				broken = true;
				interactable = false;
				collisionMode = false;
				setObjectSprite(399,tileset);
				Player_Manager::getMe().updatePlayerHitBoxes();
				Particle_Manager::getMe().spawnItemParticle(rand()%3,SCALE,posX*ZOOMSIZE,posY*ZOOMSIZE,10);
			}
		}
	}
}

std::string Stone::getMySaveData()
{
	std::string content = std::to_string(broken) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Stone::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->broken = bool(atoi(str_broken.c_str()));

	if(broken)
		setObjectSprite(399,tileset);

	return false;
}

Stone::~Stone(void)
{

}