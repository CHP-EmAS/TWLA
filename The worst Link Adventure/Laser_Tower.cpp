#include "Enemys.h"

#include "Quest_Manager.h"
#include "Particle_Manager.h"
#include "Weapon_Manager.h"
#include "Animation_Manager.h"
#include "Weapons.h"
#include "Player_Manager.h"

Laser_Tower::Laser_Tower(int mapPosX,int mapPosY,std::string settings) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::Overworld));
	enemySprite.setPosition(0,0);
	enemySprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::Overworld,398));
	enemySprite.setScale(SCALE,SCALE);
	
	init = false;
	isAttacking = true;

	type = EnemyType::Laser_Tower;

	name = "Laser_Tower";

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		damage = 0.5;
		live = 3;
		break;
	case NORMAL:
		damage = 1;
		live = 3;
		break;
	case DIFFICULT:
		damage = 1.5;
		live = 5;
		break;
	case HARDCORE:
		damage = 2;
		live = 5;
		break;
	default:
		damage = 1;
		live = 3;
		break;
	}

	fullLive = live;

	int pos = settings.find(",");
	std::string str_left = settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_right = settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_up= settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_down = settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_shot_duration_ms = settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_shot_wait_ms = settings.substr(0,pos);

	this->right = bool(atoi(str_right.c_str()));
	this->left = bool(atoi(str_left.c_str()));
	this->up = bool(atoi(str_up.c_str()));
	this->down = bool(atoi(str_down.c_str()));

	this->shot_duration_ms = atoi(str_shot_duration_ms.c_str());
	this->shot_wait_ms = atoi(str_shot_wait_ms.c_str());

	shot = false;
}

void Laser_Tower::update()
{
	Enemy::update();

	if(shot)
	{
		if(shotTimer.getElapsedTime().asMilliseconds() > shot_duration_ms)
		{
			shot = false;
			enemySprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::Overworld,398));
		}
	}

	if(!shot)
	{
		if(shot_wait_ms > 600)
		{
			if(waitTimer.getElapsedTime().asMilliseconds() > shot_wait_ms - 500)
			{
				enemySprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::Overworld,397));
			}
		}
	}

	if(waitTimer.getElapsedTime().asMilliseconds() > shot_wait_ms)
	{
		shot = true;
		enemySprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::Overworld,396));
		shotTimer.restart();

		Weapon *laser = nullptr;

		if(left)
		{
			laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,mapPosX+16,mapPosY+16,LEFT);
			((Weapons::Laser*)laser)->setShotDuration(shot_duration_ms);
		}

		if(right)
		{
			laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,mapPosX+16,mapPosY+16,RIGHT);
			((Weapons::Laser*)laser)->setShotDuration(shot_duration_ms);
		}

		if(up)
		{
			laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,mapPosX+16,mapPosY+16,UP);
			((Weapons::Laser*)laser)->setShotDuration(shot_duration_ms);
		}

		if(down) 
		{
			laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,mapPosX+16,mapPosY+16,DOWN);
			((Weapons::Laser*)laser)->setShotDuration(shot_duration_ms);
		}

		waitTimer.restart();
	}
}

void Laser_Tower::interact(interactType interact_Type)
{
	switch(interact_Type)
	{
	case interactType::Explosion_Interact:
	{
		Weapon *interactWeapon = &Weapon_Manager::getMe().getCurrentInteractWeapon();

		if(interactWeapon != nullptr)
		{
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

void Laser_Tower::hurt(float damage)
{
	DOUT("Enemy damage: " << damage)

	live -= damage;

	Animation_Manager::getMe().doBlinkAnimation(enemySprite,sf::Color::Red);

	if(live <= 0)
	{
		std::vector<int> dropList;

		dropList.push_back(1);dropList.push_back(1);dropList.push_back(1);dropList.push_back(1);dropList.push_back(1);
		dropList.push_back(2);dropList.push_back(2);
		dropList.push_back(3);
		dropList.push_back(4);
		dropList.push_back(46);dropList.push_back(46);dropList.push_back(46);dropList.push_back(46);dropList.push_back(46);
		dropList.push_back(47);dropList.push_back(47);
		dropList.push_back(48);

		kill(dropList,5);
	}
}

void Laser_Tower::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
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

std::string Laser_Tower::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live) + ",";
	fullData += std::to_string(this->isAttacking);

	return fullData;
}

bool Laser_Tower::loadMySaveData(std::string data)
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

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->live = atof(str_live.c_str());
	this->isAttacking = atoi(str_isAttacking.c_str());

	return true;
}

Laser_Tower::~Laser_Tower(void)
{
}
