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

#define JUMPER_STATE_1 sf::IntRect(6*TILESIZE,9*TILESIZE,TILESIZE,TILESIZE)
#define JUMPER_STATE_2 sf::IntRect(5*TILESIZE,9*TILESIZE,TILESIZE,TILESIZE)

#define SCELLETON_JUMPER_STATE_1 sf::IntRect(11*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)
#define SCELLETON_JUMPER_STATE_2 sf::IntRect(12*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)

Jumper::Jumper(int mapPosX,int mapPosY,std::string settings) : Enemy(mapPosX,mapPosY)
{
	type = atoi(settings.c_str());

	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setPosition(0,0);

	if(type == 0) enemySprite.setTextureRect(JUMPER_STATE_1);
	else enemySprite.setTextureRect(SCELLETON_JUMPER_STATE_1);

	enemySprite.setScale(2,2);
	
	m_collisionbox.type = Interact_Only;
	canCollide = false;
	init = false;
	isAttacking = true;

	Enemy::type = EnemyType::Jumper;

	name = "Springer";

	ms_JumpTime = 1750;

	if(type == 0)
	{
		attackRadius = 150;
		
		switch(File_Manager::getMe().getGameDifficulty())
		{
		case IMMORTAL:
		case EASY:
			damage = 0.25;
			live = 2;
			break;
		case NORMAL:
			damage = 0.5;
			live = 2;
			break;
		case DIFFICULT:
			damage = 0.75;
			live = 3;
			break;
		case HARDCORE:
			damage = 0.75;
			live = 5;
			break;
		default:
			damage = 0.5;
			live = 2;
			break;
		}
	}
	else
	{
		attackRadius = 200;

		switch(File_Manager::getMe().getGameDifficulty())
		{
		case IMMORTAL:
		case EASY:
			damage = 0.5;
			live = 3;
			break;
		case NORMAL:
			damage = 0.75;
			live = 3;
			break;
		case DIFFICULT:
			damage = 1;
			live = 4;
			break;
		case HARDCORE:
			damage = 1;
			live = 6;
			break;
		default:
			damage = 0.75;
			live = 3;
			break;
		}
	}
	
	fullLive = live;

	jumpHeight = 0;
	isInJump = false;

	aniMode = 0;
}

void Jumper::update()
{
	Enemy::update();

	if(updateClock.getElapsedTime().asMilliseconds() >= 7)
	{
		if(isInJump)
		{
			float speedX;
			float speedY;

			float distanceX = jumpPosX - mapPosX;
			float distanceY = jumpPosY - mapPosY;

			if(jumpHeight < 128 && distanceY < 32)
			{
				move(UP);
				move(UP);
				jumpHeight++;
			}

			if(distanceX < 0) distanceX *= -1;
			if(distanceY < 0) distanceY *= -1;

			float percentX = (distanceX * 100.f) / (distanceX + distanceY);
			float percentY = (distanceY * 100.f) / (distanceX + distanceY);

			if(mapPosX > jumpPosX) speedX = -((5 / 100.f) * percentX);
			else if(mapPosX < jumpPosX) speedX = ((5 / 100.f) * percentX);
			else speedX = 0;

			if(mapPosY > jumpPosY) speedY = -((5 / 100.f) * percentY);
			else if(mapPosY < jumpPosY) speedY = ((5 / 100.f) * percentY);
			else speedY = 0;

			tp(mapPosX + speedX,mapPosY+ speedY);

			if(mapPosX + 2 >= jumpPosX && mapPosX - 2 <= jumpPosX && mapPosY + 2 >= jumpPosY && mapPosY - 2 <= jumpPosY)
			{
				m_collisionbox.height = 32;
				m_collisionbox.width = 32;
				jumpHeight = 0;

				tp(jumpPosX,jumpPosY);

				if(Collision_System::getMe().boxCollision(Player_Manager::getMe().getCollisionbox(),&m_collisionbox))
				{
					Player_Manager::getMe().hurt(damage);

					int randJumpX = rand() % 64 + 32;
					int randJumpY = rand() % 64 + 32;

					if(rand() % 2 == 0)
					{
						randJumpX *= -1;
						randJumpY *= -1;
					}

					jump(mapPosX + randJumpX,mapPosY + randJumpY);
				}
				else
				{
					isInJump = false;
					aniMode = 0;

					if(type == 0) enemySprite.setTextureRect(JUMPER_STATE_1);
					else enemySprite.setTextureRect(SCELLETON_JUMPER_STATE_1);

					aniClock.restart();
				}
			}
		}

		updateClock.restart();
	}

	if(aniClock.getElapsedTime().asMilliseconds() > 1250)
	{
		if(!isInJump)
		{
			if(aniMode == 0)
			{
				if(type == 0) enemySprite.setTextureRect(JUMPER_STATE_2);
				else enemySprite.setTextureRect(SCELLETON_JUMPER_STATE_2);

				aniMode = 1;

				if(jumpClock.getElapsedTime().asMilliseconds() >= ms_JumpTime)
				{
					int playerPosX = Player_Manager::getMe().getMapPosX();
					int playerPosY = Player_Manager::getMe().getMapPosY();

					if(playerPosX < (mapPosX + attackRadius) && playerPosX > (mapPosX - attackRadius) && playerPosY < (mapPosY + attackRadius) && playerPosY > (mapPosY - attackRadius))
					{
						jump(playerPosX,playerPosY);

					}
				}
			}
			else
			{
				if(type == 0) enemySprite.setTextureRect(JUMPER_STATE_1);
				else enemySprite.setTextureRect(SCELLETON_JUMPER_STATE_1);

				aniMode = 0;
			}
		}

		aniClock.restart();
	}
}

void Jumper::interact(interactType interact_Type)
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
			isAttacking = true;
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

void Jumper::hurt(float damage)
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

void Jumper::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
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

std::string Jumper::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live) + ",";
	fullData += std::to_string(this->isAttacking);

	return fullData;
}

bool Jumper::loadMySaveData(std::string data)
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

void Jumper::jump(int mapJumpPosX, int mapJumpPosY)
{
	isInJump = true;

	if(type == 0) enemySprite.setTextureRect(JUMPER_STATE_2);
	else enemySprite.setTextureRect(SCELLETON_JUMPER_STATE_2);

	jumpClock.restart();
	m_collisionbox.height = 0;
	m_collisionbox.width = 0;
	jumpPosX = mapJumpPosX;
	jumpPosY = mapJumpPosY;

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Jump);
}

Jumper::~Jumper(void)
{
}