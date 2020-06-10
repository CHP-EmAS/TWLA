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

#define SNAKE_LEFT_1	sf::IntRect(34*TILESIZE,19*TILESIZE,TILESIZE,TILESIZE)
#define SNAKE_LEFT_2	sf::IntRect(35*TILESIZE,19*TILESIZE,TILESIZE,TILESIZE)
#define SNAKE_RIGHT_1	sf::IntRect(35*TILESIZE,19*TILESIZE,-TILESIZE,TILESIZE)
#define SNAKE_RIGHT_2	sf::IntRect(36*TILESIZE,19*TILESIZE,-TILESIZE,TILESIZE)
#define SNAKE_UP_1		sf::IntRect(38*TILESIZE,19*TILESIZE,TILESIZE,TILESIZE)
#define SNAKE_UP_2		sf::IntRect(39*TILESIZE,19*TILESIZE,-TILESIZE,TILESIZE)
#define SNAKE_DOWN_1	sf::IntRect(36*TILESIZE,19*TILESIZE,TILESIZE,TILESIZE)
#define SNAKE_DOWN_2	sf::IntRect(37*TILESIZE,19*TILESIZE,-TILESIZE,TILESIZE)

Snake::Snake(int mapPosX,int mapPosY) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setPosition(0,0);
	enemySprite.setTextureRect(sf::IntRect(34*TILESIZE,19*TILESIZE,TILESIZE,TILESIZE));
	enemySprite.setScale(2,2);
	
	dir = DOWN;
	m_collisionbox.type = collisionboxType::Enemy_Collisionbox;
	showLiveBar = true;
	init = false;
	isAttacking = true;

	type = EnemyType::Snake;
	name = "Schlange";

	moveable::collisionbox_Update_Range = 120;
	m_collisionbox.height = 32;
	m_collisionbox.width = 32;

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		damage = 0.25;
		live = 1;
		break;
	case NORMAL:
		damage = 0.25;
		live = 2;
		break;
	case DIFFICULT:
		damage = 0.5;
		live = 3;
		break;
	case HARDCORE:
		damage = 0.5;
		live = 5;
		break;
	default:
		damage = 0.25;
		live = 2;
		break;
	}

	fullLive = live;

	ms_moveTime = 50;
	attackTime = 0;
}

void Snake::update()
{
	Enemy::update();

	if(updateClock.getElapsedTime().asMilliseconds() >= ms_moveTime * 8)
	{
		if(!isAttacking)
		{
			switch(rand() % 8)
			{
			case LEFT:
				dir = LEFT;
				break;
			case RIGHT:
				dir = RIGHT;
				break;
			case UP:
				dir = UP;
				break;
			case DOWN:
				dir = DOWN;
				break;
			}
		}

		step = !step;

		updateClock.restart();
	}

	if(moveClock.getElapsedTime().asMilliseconds() >= ms_moveTime)
	{
		if(isAttacking)
		{
			bool foundPlayer = false;

			if(Player_Manager::getMe().getMapPosY() > mapPosY)
			{
				collisionbox* foundCollision = move(DOWN);
				dir = DOWN;

				if(foundCollision->type == Player_Collisionbox) 
					foundPlayer = true;
			}
			else if(Player_Manager::getMe().getMapPosY() < mapPosY)
			{
				collisionbox* foundCollision = move(UP);
				dir = UP;

				if(foundCollision->type == Player_Collisionbox) 
					foundPlayer = true;
			}

			if(Player_Manager::getMe().getMapPosX() > mapPosX)
			{
				collisionbox* foundCollision = move(RIGHT);
				dir = RIGHT;

				if(foundCollision->type == Player_Collisionbox) 
					foundPlayer = true;
			}
			else if(Player_Manager::getMe().getMapPosX() < mapPosX)
			{
				collisionbox* foundCollision = move(LEFT);
				dir = LEFT;

				if(foundCollision->type == Player_Collisionbox) 
					foundPlayer = true;
			}

			if(foundPlayer)
			{
				Player_Manager::getMe().hurt(damage);
				attackTime = time(NULL);
			}

			int nowTime = time(NULL);

			if(nowTime - attackTime > 10)
			{
				isAttacking = false;
				ms_moveTime = 50;
			}
		}
		else
		{
			move(dir);
		}

		moveClock.restart();
	}

	switch(dir)
	{
	case LEFT:
		if(step) enemySprite.setTextureRect(SNAKE_LEFT_1);
		else enemySprite.setTextureRect(SNAKE_LEFT_2);
		break;
	case RIGHT:
		if(step) enemySprite.setTextureRect(SNAKE_RIGHT_1);
		else enemySprite.setTextureRect(SNAKE_RIGHT_2);
		break;
	case UP:
		if(step) enemySprite.setTextureRect(SNAKE_UP_1);
		else enemySprite.setTextureRect(SNAKE_UP_2);
		break;
	case DOWN:
		if(step) enemySprite.setTextureRect(SNAKE_DOWN_1);
		else enemySprite.setTextureRect(SNAKE_DOWN_2);
		break;
	}
}

void Snake::interact(interactType interact_Type)
{
	switch(interact_Type)
	{
	case interactType::Arrow_Interact:
	case interactType::Boomerang_Interact:
	case interactType::Explosion_Interact:
	case interactType::Hit_Interact:
	case interactType::Laser_Interact:
	{
		Weapon *interactWeapon = &Weapon_Manager::getMe().getCurrentInteractWeapon();

		if(interactWeapon != nullptr)
		{
			attackTime = time(NULL);
			isAttacking = true;
			ms_moveTime = 10;
			hurt(interactWeapon->getDamage());
		}
		else
		{
			FOUT("Die aktuelle interaktions Waffe ist NULL!")
		}

		break;
	}
	}
}

void Snake::hurt(float damage)
{
	DOUT("Enemy damage: " << damage)

	live -= damage;

	Animation_Manager::getMe().doBlinkAnimation(enemySprite,sf::Color::Red);

	if(live <= 0)
	{
		std::vector<int> dropList;

		dropList.push_back(0);dropList.push_back(0);dropList.push_back(0);dropList.push_back(0);dropList.push_back(0);
		dropList.push_back(1);dropList.push_back(1);
		dropList.push_back(2);
		dropList.push_back(45);dropList.push_back(45);dropList.push_back(45);dropList.push_back(45);dropList.push_back(45);
		dropList.push_back(46);dropList.push_back(46);
		dropList.push_back(47);

		kill(dropList,1);
	}

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Hit);
}

void Snake::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	setActiv(false);

	std::string command = "kill-";
	command += std::to_string(type);
	Quest_Manager::getMe().doQuestCommand(command);

	for(int i = 0; i < itemDropAmount;i++)
	{
		Particle_Manager::getMe().spawnItemParticle(dropItems.at(rand() % dropItems.size()),SCALE,mapPosX + rand() % 33 - 16,mapPosY + rand() % 33);
	}

	Player_Manager::getMe().getGameStats().addMonsterKill();
}

std::string Snake::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->dir) + ",";
	fullData += std::to_string(this->live) + ",";
	fullData += std::to_string(this->isAttacking);

	return fullData;
}

bool Snake::loadMySaveData(std::string data)
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
	std::string str_dir = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_live = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_isAttacking = data.substr(0,pos);

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->dir = atoi(str_dir.c_str());
	this->live = atof(str_live.c_str());

	this->isAttacking = atoi(str_isAttacking.c_str());

	if(isAttacking)
	{
		attackTime = time(NULL);
		ms_moveTime = 10;
	}

	return true;
}

Snake::~Snake(void)
{
}
