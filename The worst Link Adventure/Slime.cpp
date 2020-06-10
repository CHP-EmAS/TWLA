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
#include "Text_Particle.h"
#include "Dialog_Manager.h"
#include "Command_Manager.h"

#define SLIME_BIG_STATE_1 sf::IntRect(27*TILESIZE,2*TILESIZE+11,TILESIZE*2,21)
#define SLIME_BIG_STATE_1_BLINK sf::IntRect(47*TILESIZE,3*TILESIZE+11,TILESIZE*2,21)
#define SLIME_BIG_STATE_2 sf::IntRect(29*TILESIZE+5,2*TILESIZE,21,TILESIZE*2)

#define SLIME_SMALL_STATE_1 sf::IntRect(20*TILESIZE,2*TILESIZE,TILESIZE,TILESIZE)
#define SLIME_SMALL_STATE_1_BLINK sf::IntRect(49*TILESIZE,4*TILESIZE,TILESIZE,TILESIZE)
#define SLIME_SMALL_STATE_2 sf::IntRect(19*TILESIZE,2*TILESIZE,TILESIZE,TILESIZE)

Slime::Slime(int mapPosX,int mapPosY,std::string settings) : Enemy(mapPosX,mapPosY)
{
	int pos = settings.find(",");
	std::string str_type = settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_r = settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_g= settings.substr(0,pos);

	settings = settings.substr(pos+1);
	pos = settings.find(",");
	std::string str_b = settings.substr(0,pos);

	slimeColor = sf::Color(atoi(str_r.c_str()),atoi(str_g.c_str()),atoi(str_b.c_str()));
	type = atoi(str_type.c_str());

	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setPosition(0,0);
	enemySprite.setColor(slimeColor);

	if(type == 0) enemySprite.setTextureRect(SLIME_BIG_STATE_1);
	else if(type == 1) enemySprite.setTextureRect(SLIME_SMALL_STATE_1);
	else enemySprite.setTextureRect(SLIME_BIG_STATE_1_BLINK);
	
	m_collisionbox.type = Interact_Only;
	canCollide = false;
	init = false;

	Enemy::type = EnemyType::Slime;

	name = "Schliem";

	ms_JumpTime = 2000;

	if(type == 0)
	{
		enemySprite.setScale(2,2);
		m_collisionbox.height = 42;
		m_collisionbox.width = 64;
		attackRadius = 200;
		isAttacking = true;
	}
	else if(type == 1)
	{
		enemySprite.setScale(2,2);
		m_collisionbox.height = 32;
		m_collisionbox.width = 32;
		attackRadius = 100;
		isAttacking = true;
	}
	else if(type == 2)
	{
		enemySprite.setScale(3,3);
		m_collisionbox.height = 48;
		m_collisionbox.width = 100;
		attackRadius = 400;
		isAttacking = false;
	}

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		if(type == 0)
		{
			damage = 0.5;
			live = 1;
		}
		else if(type == 1)
		{
			damage = 0.25;
			live = 0.5;
		}
		else if(type == 2)
		{
			damage = 0.5;
			live = 7;
		}
		break;
	case NORMAL:
		if(type == 0)
		{
			damage = 0.5;
			live = 2;
		}
		else if(type == 1)
		{
			damage = 0.25;
			live = 1;
		}
		else if(type == 2)
		{
			damage = 0.5;
			live = 15;
		}
		break;
	case DIFFICULT:
		if(type == 0)
		{
			damage = 1;
			live = 4;
		}
		else if(type == 1)
		{
			damage = 0.5;
			live = 3;
		}
		else if(type == 2)
		{
			damage = 1.5;
			live = 20;
		}
		break;
	case HARDCORE:
		if(type == 0)
		{
			damage = 1.25;
			live = 5;
		}
		else if(type == 1)
		{
			damage = 0.75;
			live = 4;
		}
		else if(type == 2)
		{
			damage = 2;
			live = 20;
		}
		break;
	default:
		if(type == 0)
		{
			damage = 0.5;
			live = 2;
		}
		else if(type == 1)
		{
			damage = 0.25;
			live = 1;
		}
		else if(type == 2)
		{
			damage = 0.5;
			live = 15;
		}
		break;
	}
	
	fullLive = live;

	jumpHeight = 0;
	jumpState = 1;
}

void Slime::update()
{
	Enemy::update();

	if(updateClock.getElapsedTime().asMilliseconds() >= 7)
	{
		if(jumpState == 0)
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
				jumpHeight = 0;

				tp(jumpPosX,jumpPosY);

				if(Collision_System::getMe().boxCollision(Player_Manager::getMe().getCollisionbox(),&m_collisionbox))
				{
					Player_Manager::getMe().hurt(damage);

					jump(spawnPosX,spawnPosY);
				}
				else
				{
					if(type == 0) enemySprite.setTextureRect(SLIME_BIG_STATE_1);
					else if(type == 1)enemySprite.setTextureRect(SLIME_SMALL_STATE_1);
					else enemySprite.setTextureRect(SLIME_BIG_STATE_1);

					if(jumpPosX == spawnPosX && jumpPosY == spawnPosY) jumpState = 1;
					else jumpState = 2;
				}
			}
		}

		updateClock.restart();
	}
	
	if(jumpState == 2)
	{
		if(jumpClock.getElapsedTime().asMilliseconds() >= ms_JumpTime * 1.5)
		{
			jump(spawnPosX,spawnPosY);
		}
	}
	else if(jumpState == 1)
	{
		if(isAttacking)
		{
			if(blinkClock.getElapsedTime().asMilliseconds() >= ms_blinkWaitTime + 100)
			{
				if(type == 0) enemySprite.setTextureRect(SLIME_BIG_STATE_1);
				else if (type == 1) enemySprite.setTextureRect(SLIME_SMALL_STATE_1);
				else enemySprite.setTextureRect(SLIME_BIG_STATE_1); 

				ms_blinkWaitTime = (rand() % 3000) + 2000;

				blinkClock.restart();
			}
			else if(blinkClock.getElapsedTime().asMilliseconds() >= ms_blinkWaitTime)
			{
				if(type == 0) enemySprite.setTextureRect(SLIME_BIG_STATE_1_BLINK);
				else if (type == 1) enemySprite.setTextureRect(SLIME_SMALL_STATE_1_BLINK);
				else enemySprite.setTextureRect(SLIME_BIG_STATE_1_BLINK); 
			}
		}
		else
		{
			if(blinkClock.getElapsedTime().asMilliseconds() >= 750)
			{
				Particle* textParticle = Particle_Manager::getMe().addParticle(Particle_Manager::ParticleType::Text,20,sf::Color::Blue,2000,20,enemySprite.getPosition().x+enemySprite.getGlobalBounds().width/2,enemySprite.getPosition().y,rand()%3-1,-1);
				if(textParticle != nullptr)
				{
					((Text_Particle*)textParticle)->setString("Z");
					((Text_Particle*)textParticle)->setFont(Graphic_Manager::font::Pixel);
					((Text_Particle*)textParticle)->changeMode(Text_Particle::Standart);
				}
				blinkClock.restart();
			}
		}

		if(jumpClock.getElapsedTime().asMilliseconds() >= ms_JumpTime)
		{
			int playerPosX = Player_Manager::getMe().getMapPosX();
			int playerPosY = Player_Manager::getMe().getMapPosY();

			if(playerPosX < (mapPosX + attackRadius) && playerPosX > (mapPosX - attackRadius) && playerPosY < (mapPosY + attackRadius) && playerPosY > (mapPosY - attackRadius) && isAttacking)
			{
				jump(playerPosX,playerPosY);
			}
		}
	}
}

void Slime::interact(interactType interact_Type)
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
			if(!isAttacking)
			{
				if(type == 2 && Map_Manager::getMe().getCurrentMap().getID() == 993)
				{
					jumpClock.restart();
					Command_Manager::getMe().doCommand("doObjectCmd cmd:close 5");
					Command_Manager::getMe().doCommand("doObjectCmd cmd:close 6");
				}
			}

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

void Slime::hurt(float damage)
{
	DOUT("Enemy damage: " << damage)

	live -= damage;

	Animation_Manager::getMe().doBlinkAnimation(enemySprite,sf::Color::Red,slimeColor);

	if(live <= 0)
	{
		std::vector<int> dropList;

		dropList.push_back(0);dropList.push_back(0);dropList.push_back(0);dropList.push_back(0);dropList.push_back(0);
		dropList.push_back(1);dropList.push_back(1);
		dropList.push_back(2);
		dropList.push_back(45);dropList.push_back(45);dropList.push_back(45);dropList.push_back(45);dropList.push_back(45);
		dropList.push_back(46);dropList.push_back(46);
		dropList.push_back(47);

		kill(dropList,3);
	}

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Hit);
}

void Slime::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	setActiv(false);

	std::string command = "kill-";
	command += std::to_string(type);
	Quest_Manager::getMe().doQuestCommand(command);

	if(type == 2 && Map_Manager::getMe().getCurrentMap().getID() == 993)
	{
		Player_Manager::getMe().give(28,1);
		Player_Manager::getMe().give(987,1);
		Dialog_Manager::getMe().doDialog(2);
		Command_Manager::getMe().doCommand("doObjectCmd cmd:open 5");
		Command_Manager::getMe().doCommand("doObjectCmd cmd:open 6");
	}
	else
	{
		for(int i = 0; i < itemDropAmount;i++)
		{
			Particle_Manager::getMe().spawnItemParticle(dropItems.at(rand() % dropItems.size()),SCALE,mapPosX + rand() % 70,mapPosY + rand() % 26);
		}
	}

	Player_Manager::getMe().getGameStats().addMonsterKill();
}

std::string Slime::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live) + ",";
	fullData += std::to_string(this->isAttacking);

	return fullData;
}

bool Slime::loadMySaveData(std::string data)
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

void Slime::jump(int mapJumpPosX, int mapJumpPosY)
{
	jumpState = 0;

	if(type == 0) enemySprite.setTextureRect(SLIME_BIG_STATE_2);
	else if(type == 1)enemySprite.setTextureRect(SLIME_SMALL_STATE_2);
	else enemySprite.setTextureRect(SLIME_BIG_STATE_2);

	jumpClock.restart();
	jumpPosX = mapJumpPosX;
	jumpPosY = mapJumpPosY;

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Jump);
}

Slime::~Slime(void)
{
}