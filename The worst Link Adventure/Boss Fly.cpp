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
#include "Command_Manager.h"

#define BOSS_FLY_1 sf::IntRect(15*TILESIZE,13*TILESIZE,TILESIZE*2,TILESIZE)
#define BOSS_FLY_2 sf::IntRect(17*TILESIZE,13*TILESIZE,TILESIZE*2,TILESIZE)

#define SMALL_FLY_1 sf::IntRect(16*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)
#define SMALL_FLY_2 sf::IntRect(17*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)

Boss_Fly::Boss_Fly(int mapPosX,int mapPosY) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setPosition(-100,-100);
	enemySprite.setTextureRect(BOSS_FLY_1);
	enemySprite.setScale(2,2);

	spr_miniFly.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_miniFly.setTextureRect(SMALL_FLY_1);
	spr_miniFly.setScale(2,2);
	
	canCollide = false;
	init = false;
	isAttacking = true;
	updateAlways = true;

	Enemy::type = EnemyType::Boss_Fly;

	name = "Fliegen Königin";

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		damage = 0.25;
		live = 5;
		break;
	case NORMAL:
		damage = 0.25;
		live = 10;
		break;
	case DIFFICULT:
		damage = 0.5;
		live = 10;
		break;
	case HARDCORE:
		damage = 0.5;
		live = 15;
		break;
	default:
		damage = 0.25;
		live = 10;
		break;
	}

	fullLive = live;

	m_collisionbox.width = 64;

	tp(mapPosX,mapPosY - 320);

	bossMode = 0;
	flyMode = 0;
}

void Boss_Fly::update()
{
	bool secondTP = false;

	if(!init)
	{
		secondTP = true;
		Sound_Manager::getMe().changeBackgroundMusic("Music/Boss Theme.ogg");
	}

	Enemy::update();

	if(secondTP) tp(mapPosX,mapPosY - 128);

	if(updateClock.getElapsedTime().asMilliseconds() >= 7)
	{
		if(bossMode == 0)//Hinuter fliegen
		{
			if(mapPosY < spawnPosY)
			{
				move(DOWN);
			}
			else 
			{
				bossMode = 1;
			}
		}
		else if(bossMode == 1) //Nach links und rechts fliegen
		{
			if(flyMode == 0)
			{
				if(mapPosX > spawnPosX - 7*32)
				{
					move(LEFT);
					move(LEFT);
				}
				else
				{
					flyMode = 1;
				}
			}
			else
			{
				if(mapPosX < spawnPosX + 7*32)
				{
					move(RIGHT);
					move(RIGHT);
				}
				else
				{
					flyMode = 0;
				}
			}
		}
		else if(bossMode == 2) //hoch fliegen
		{
			if(mapPosY > spawnPosY - 5*32)
			{
				move(UP);
			}
			else 
			{
				bossMode = 3;
				arrowDropAmount = 0;
			}
		}
		else if(bossMode == 3) //kleine Fliegen spawnen
		{
			if(miniFlySpawnClock.getElapsedTime().asMilliseconds() > 200)
			{
				spawnNewMiniFly(rand()%18 + 1,-5);
				miniFlySpawnClock.restart();
			}

			updateMiniFlys();
		}
		else if(bossMode == 4)//warten bis alle Fiegen weg sind
		{
			if(miniFlys.size() == 0)
			{
				bossMode = 0;
			}

			updateMiniFlys();
		}
		else if(bossMode == 5) //sterben
		{
			if(bossClock.getElapsedTime().asMilliseconds() >= 300 && wingsClock.getElapsedTime().asMilliseconds() < 3000)
			{
				Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x + rand() % 64,enemySprite.getPosition().y + rand() % 32,650,true,sf::Color::Green);
				Sound_Manager::getMe().playEffectSound(Sound_Manager::Bomb_Explosion);
				bossClock.restart();
			}

			if(wingsClock.getElapsedTime().asMilliseconds() >= 3000)
			{
				bossMode = 6;
			}
		}
		else if(bossMode == 6) //löschen
		{
			if(Animation_Manager::getMe().getRunningAnimationAmount() == 0)
			{
				std::vector<int> temp;
				kill(temp,0);
			}
		}

		updateClock.restart();
	}

	if(bossMode < 5)
	{
		if(wingsClock.getElapsedTime().asMilliseconds() >= 100)
		{
			enemySprite.setTextureRect(BOSS_FLY_1);
			spr_miniFly.setTextureRect(SMALL_FLY_1);
			wingsClock.restart();
		}
		else if(wingsClock.getElapsedTime().asMilliseconds() >= 50)
		{
			enemySprite.setTextureRect(BOSS_FLY_2);
			spr_miniFly.setTextureRect(SMALL_FLY_2);
		}

		if(bossClock.getElapsedTime().asMilliseconds() >= 13000)
		{
			if(bossMode == 1) bossMode = 2;
			else bossMode = 4;

			bossClock.restart();
		}
	}
}

void Boss_Fly::interact(interactType interact_Type)
{
	switch(interact_Type)
	{
	case interactType::Arrow_Interact:
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

void Boss_Fly::draw(sf::RenderWindow &mainWindow)
{
	if(bossMode == 3 || bossMode == 4)
	{
		for(auto fly : miniFlys)
		{
			spr_miniFly.setPosition(Map_Manager::getMe().getMapPosX() + fly.posX,Map_Manager::getMe().getMapPosY() + fly.posY);
			mainWindow.draw(spr_miniFly);
		}
	}
	else if(bossMode != 6)
	{
		Enemy::draw(mainWindow);
	}
}

void Boss_Fly::hurt(float damage)
{
	if(bossMode < 5)
	{
		DOUT("Enemy damage: " << damage)

		live -= damage;

		Animation_Manager::getMe().doBlinkAnimation(enemySprite,sf::Color::Red);

		if(live <= 0)
		{
			Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Stopped);
			wingsClock.restart();
			bossClock.restart();
			bossMode = 5;
		}

		Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x,enemySprite.getPosition().y,650,true,sf::Color::Green);

		Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Hit);
	}
}

void Boss_Fly::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	std::string command = "kill-boss-0";
	Quest_Manager::getMe().doQuestCommand(command);

	if(Map_Manager::getMe().getCurrentMap().getID() == 998)
	{
		Command_Manager::getMe().doCommand("setObjectActivity 0 1");
		Command_Manager::getMe().doCommand("setObjectActivity 1 1");
		Command_Manager::getMe().doCommand("doDialog 998");
	}

	Player_Manager::getMe().getGameStats().addMonsterKill();
 	setActiv(false);
 }

std::string Boss_Fly::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live) + ",";
	fullData += std::to_string(this->isAttacking) + ",";
	fullData += std::to_string(this->bossMode) + ",";

	return fullData;
}

bool Boss_Fly::loadMySaveData(std::string data)
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
	std::string str_bossMode = data.substr(0,pos);

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->live = atof(str_live.c_str());
	this->isAttacking = atoi(str_isAttacking.c_str());
	this->bossMode = atoi(str_bossMode.c_str());

	return true;
}

void Boss_Fly::spawnNewMiniFly(int tile_posX, int tile_posY)
{
	miniFly newFly;
	newFly.posX = tile_posX*32;
	newFly.posY = tile_posY*32;
	newFly.arrowDrop = false;
	miniFlys.push_back(newFly);
}

void Boss_Fly::updateMiniFlys()
{
	std::list<miniFly>::iterator fly;
	for(fly = miniFlys.begin(); fly != miniFlys.end(); fly++)
	{
		collisionbox flyBox;
		flyBox.height = 32;
		flyBox.width = 32;
		flyBox.mapPosX = fly->posX;
		flyBox.mapPosY = fly->posY;

		if(fly->posY < 20*32)
		{
			fly->posY += 3;

			if(fly->posY >= 5*32 && fly->posY <= 5*32+5 && !fly->arrowDrop && arrowDropAmount < 2)
			{
				if(rand() % 6 == 4)
				{
					Particle_Manager::getMe().spawnItemParticle(34,SCALE,fly->posX + rand() % 33,fly->posY + rand() % 33);
					fly->arrowDrop = true;
					arrowDropAmount++;
				}
				else if(rand() % 50 == 10)
				{
					Particle_Manager::getMe().spawnItemParticle(46,SCALE,fly->posX + rand() % 33,fly->posY + rand() % 33);
					fly->arrowDrop = true;
					arrowDropAmount++;
				}
			}
		}
		else
		{
			fly = miniFlys.erase(fly);
			if(fly == miniFlys.end()) break;
		}

		if(Collision_System::getMe().boxCollision(Player_Manager::getMe().getCollisionbox(),&flyBox))
		{
			Player_Manager::getMe().hurt(damage);
		}
	}
}

Boss_Fly::~Boss_Fly(void)
{
}