#include "Objects.h"

#include "Map_Manager.h"
#include "Graphic_Manager.h"
#include "Message_Manager.h"
#include "Animation_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

Secret_Bush::Secret_Bush(int x, int y,short ID, std::string portWorld, int portSpawnPointNumber) : InteractivObject(x,y,ID)
{
	objType = ObjectType::Secret_Bush;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	setObjectSprite(401,tileset);
	
	cBox.height = ZOOMSIZE - 10;

	broken = false;

	this->portSpawnPointNumber = portSpawnPointNumber;
	this->portWorld = portWorld;
}

void Secret_Bush::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Hit_Interact && !broken)
		{
			broken = true;
			collisionMode = false;
			setObjectSprite(87,tileset);
			
			Player_Manager::getMe().updatePlayerHitBoxes();
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Grass_Cut);
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Found_Secret);

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

			Player_Manager::getMe().getGameStats().addCuttetGrass();
		}
		else if(interact_Type == Collision_Interact && broken)
		{
			//Ausblenden
			Screen_Manager::getMe().fadeOut();
			//Nachrichten löschen und neue wilkommens Nachricht erstellen
			Message_Manager::getMe().showLocationMsg(portWorld);
			//neue map setzen
			Map_Manager::getMe().changeCurrentMap(portWorld,portSpawnPointNumber);
			//Einblenden
			Screen_Manager::getMe().fadeIn();;
		}
	}
}

std::string Secret_Bush::getMySaveData()
{
	std::string content = std::to_string(broken) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Secret_Bush::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->broken = bool(atoi(str_broken.c_str()));

	if(broken)
		setObjectSprite(87,tileset);

	return false;
}

Secret_Bush::~Secret_Bush(void)
{

}
