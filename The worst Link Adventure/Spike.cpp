#include "Enemys.h"

#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Collision_System.h"
#include "Weapon_Manager.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "Quest_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"

#define SPIKE_STATE_1 sf::IntRect(14*TILESIZE,11*TILESIZE,TILESIZE,TILESIZE)
#define SPIKE_STATE_2 sf::IntRect(15*TILESIZE,11*TILESIZE,TILESIZE,TILESIZE)

Spike::Spike(int mapPosX,int mapPosY,std::string settings) : Enemy(mapPosX,mapPosY)
{
	int pos = settings.find(",");
	mode = atoi(settings.substr(0,pos).c_str());
	settings = settings.substr(pos+1);

	updateTime = atoi(settings.c_str());

	if(mode == 0 || mode == 2) currentDir = RIGHT;
	else if(mode == 1) currentDir = UP;

	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setPosition(0,0);
	enemySprite.setTextureRect(SPIKE_STATE_1);
	enemySprite.setScale(2,2);
	
	m_collisionbox.type = collisionboxType::Enemy_Collisionbox;
	showLiveBar = false;
	init = false;
	isAttacking = true;

	Enemy::type = EnemyType::Spike;

	name = "Stachel";

	moveable::collisionbox_Update_Range = 120;
	m_collisionbox.height = 28;
	m_collisionbox.width = 28;
	moveable::cBoxPosX = 2;
	moveable::cBoxPosY = 2;

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		damage = 0.25;
		live = 10;
		break;
	case NORMAL:
		damage = 0.5;
		live = 10;
		break;
	case DIFFICULT:
		damage = 1;
		live = 10;
		break;
	case HARDCORE:
		damage = 2;
		live = 10;
		break;
	default:
		damage = 0.25;
		live = 10;
		break;
	}

	fullLive = live;

	aniState = true;
	backwards = false;
}

void Spike::update()
{
	Enemy::update();

	if(updateClock.getElapsedTime().asMilliseconds() >= updateTime)
	{
		updateClock.restart();

		collisionbox *collision = move(currentDir);

		if(collision != nullptr)
		{
			if(collision->type != None_Collisionbox)
			{
				if(collision->type == Player_Collisionbox)
				{
					backwards = !backwards;

					if(currentDir == LEFT) currentDir = RIGHT;
					else if(currentDir == RIGHT) currentDir = LEFT;
					else if(currentDir == UP) currentDir = DOWN;
					else if(currentDir == DOWN) currentDir = UP;

					Player_Manager::getMe().hurt(damage);
				}
				else
				{
					if(mode == 0)
					{
						if(currentDir == LEFT) currentDir = RIGHT;
						else currentDir = LEFT;
					}
					else if(mode == 1)
					{
						if(currentDir == UP) currentDir = DOWN;
						else currentDir = UP;
					}
					else if(mode == 2)
					{
						if(!backwards)
						{
							if(currentDir == LEFT) currentDir = DOWN;
							else if(currentDir == RIGHT) currentDir = UP;
							else if(currentDir == UP) currentDir = LEFT;
							else if(currentDir == DOWN) currentDir = RIGHT;
						}
						else
						{
							if(currentDir == LEFT) currentDir = UP;
							else if(currentDir == RIGHT) currentDir = DOWN;
							else if(currentDir == UP) currentDir = RIGHT;
							else if(currentDir == DOWN) currentDir = LEFT;
						}
					}
				}
			}
		}
	}

	if(aniClock.getElapsedTime().asMilliseconds() >= 1000)
	{
		aniClock.restart();

		if(aniState) enemySprite.setTextureRect(SPIKE_STATE_2);
		else enemySprite.setTextureRect(SPIKE_STATE_1);

		aniState = !aniState;
	}
}

void Spike::interact(interactType interact_Type)
{
	if(interact_Type == interactType::Collision_Interact)
	{
		Player_Manager::getMe().hurt(damage);
	}
}

void Spike::hurt(float damage)
{
	
}

void Spike::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	Player_Manager::getMe().getGameStats().addMonsterKill();
}

std::string Spike::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active)  + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live)    + ",";
	fullData += std::to_string(this->isAttacking) + ",";
	fullData += std::to_string(this->backwards) + ",";
	fullData += std::to_string(this->currentDir);

	return fullData;
}

bool Spike::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_active = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_mapPosX = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_mapPosY = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_live = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_isAttacking = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_backwards = data.substr(0,pos);

	data = data.substr(pos+1);
	std::string str_currentDir = data;

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->live = atof(str_live.c_str());
	this->isAttacking = atoi(str_isAttacking.c_str());
	this->currentDir = atoi(str_currentDir.c_str());
	this->backwards = bool(atoi(str_backwards.c_str()));

	return true;
}

Spike::~Spike(void)
{
}