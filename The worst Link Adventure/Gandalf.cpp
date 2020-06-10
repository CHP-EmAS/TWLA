#include "Enemys.h"

#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Collision_System.h"
#include "Weapon_Manager.h"
#include "Weapons.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "Quest_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"
#include "Command_Manager.h"

#define GANDALF_LEFT sf::IntRect(27*TILESIZE,15*TILESIZE,TILESIZE*2,TILESIZE*2)
#define GANDALF_LEFT_2 sf::IntRect(33*TILESIZE,15*TILESIZE,TILESIZE*2,TILESIZE*2)

#define GANDALF_RIGHT sf::IntRect(29*TILESIZE,15*TILESIZE,-(TILESIZE*2),TILESIZE*2)
#define GANDALF_RIGHT_2 sf::IntRect(35*TILESIZE,15*TILESIZE,-(TILESIZE*2),TILESIZE*2)

#define GANDALF_UP sf::IntRect(29*TILESIZE,15*TILESIZE,TILESIZE*2,TILESIZE*2)
#define GANDALF_UP_2 sf::IntRect(35*TILESIZE,15*TILESIZE,TILESIZE*2,TILESIZE*2)

#define GANDALF_DOWN sf::IntRect(25*TILESIZE,15*TILESIZE,TILESIZE*2,TILESIZE*2)
#define GANDALF_DOWN_2 sf::IntRect(31*TILESIZE,15*TILESIZE,TILESIZE*2,TILESIZE*2)

Gandalf::Gandalf(int mapPosX,int mapPosY) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setTextureRect(GANDALF_DOWN_2);
	enemySprite.setScale(2,2);
	enemySprite.setColor(sf::Color(255,255,255,255));

	starSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	starSprite.setTextureRect(sf::IntRect(22*TILESIZE,16*TILESIZE,13,13));
	starSprite.setOrigin(6,6);

	canMove = false;
	canCollide = false;
	init = false;
	isAttacking = true;
	updateAlways = true;
	showLiveBar = false;

	Enemy::type = EnemyType::Gandalf;

	name = "Gandalf";

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
		damage = 0.75;
		live = 10;
		break;
	case HARDCORE:
		damage = 0.75;
		live = 15;
		break;
	default:
		damage = 0.5;
		live = 10;
		break;
	}

	fullLive = live;

	m_collisionbox.width = 40;
	cBoxPosX = 12;

	m_collisionbox.height = 45;
	cBoxPosY = 19;

	bossMode = 2;
	explosionCounter = 0;
	lookDir = DOWN;

	magicStatus = 0;
}

void Gandalf::update()
{
	bool secondTP = false;

	if(!init)
	{
		secondTP = true;
		Sound_Manager::getMe().changeBackgroundMusic("Music/Boss Theme.ogg");
	}

	Enemy::update();

	if(secondTP) tp(320,64);

	if(updateClock.getElapsedTime().asMilliseconds() >= 10)
	{
		updateClock.restart();
		switch(bossMode)
		{
		case 0: //fadeIn
			if(enemySprite.getColor().a + 4 < 255)
			{
				enemySprite.setColor(sf::Color(255,255,255,enemySprite.getColor().a + 4));
			}
			else
			{
				enemySprite.setColor(sf::Color(255,255,255,255));
				bossMode = 1;
				waitClock.restart();
			}
			break;
		case 1:
		{
			if(waitClock.getElapsedTime().asMilliseconds() > 200)
			{
				switch(lookDir)
				{
				case LEFT:
					enemySprite.setTextureRect(GANDALF_LEFT_2);
					break;
				case RIGHT:
					enemySprite.setTextureRect(GANDALF_RIGHT_2);
					break;
				case UP:
					enemySprite.setTextureRect(GANDALF_UP_2);
					break;
				case DOWN:
					enemySprite.setTextureRect(GANDALF_DOWN_2);
					break;
				}

				Weapon *magic = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Magic,getMapPosX() + 32,getMapPosY() + 32,lookDir);
				((Weapons::Magic *)magic)->setTargetObj(Player_Manager::getMe().getCollisionbox());
				((Weapons::Magic *)magic)->setBoundObj(&m_collisionbox);
				((Weapons::Magic *)magic)->setStatusVariable(&magicStatus);
				

				bossMode = 8;
				waitClock.restart();
			}
			break;
		}
		case 2:
			if(enemySprite.getColor().a - 7 > 0)
			{
				enemySprite.setColor(sf::Color(255,255,255,enemySprite.getColor().a - 7));
			}
			else
			{
				enemySprite.setColor(sf::Color(255,255,255,0));
				tp(-64,-64);
				waitClock.restart();
				bossMode = 3;
			}
			break;
		case 3:
			if(waitClock.getElapsedTime().asMilliseconds() > 2000)
			{
				calculateNewPosition();
			}
			break;
		case 4:
			if(waitClock.getElapsedTime().asMilliseconds() > 2000)
			{
				bossMode = 5;

				Command_Manager::getMe().doCommand("doDialog 997");

				Screen_Manager::getMe().fadeOut();

 				Player_Manager::getMe().tp(336,288);
				Player_Manager::getMe().move(UP);
				Player_Manager::getMe().setPlayerCanMove(false);
				
				enemySprite.setColor(sf::Color(255,255,255,255));
				tpToFixPoint(UP);

				Screen_Manager::getMe().fadeIn();

				waitClock.restart();
			}
			break;
		case 5:
			if(waitClock.getElapsedTime().asMilliseconds() > 2000)
			{
				Weapon *magic = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Magic,160+16,288+16,lookDir);
				((Weapons::Magic *)magic)->setTargetObj(&m_collisionbox);

				magic = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Magic,512+16,288+16,lookDir);
				((Weapons::Magic *)magic)->setTargetObj(&m_collisionbox);

				bossMode = 6;
				waitClock.restart();
			}
			break;
		case 7:
			if(waitClock.getElapsedTime().asMilliseconds() >= 300 && explosionCounter <= 10)
			{
				Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x + rand() % 64,enemySprite.getPosition().y + rand() % 64,650,true,sf::Color::Blue);
				Sound_Manager::getMe().playEffectSound(Sound_Manager::Bomb_Explosion);
				waitClock.restart();

				explosionCounter++;
			}

			if(explosionCounter > 10)
			{
				if(Animation_Manager::getMe().getRunningAnimationAmount() == 0)
				{
					std::vector<int> temp;
					kill(temp,0);
				}
			}
			break;
		case 8:
			if(magicStatus == 1)
			{
				bossMode = 2;
				waitClock.restart();
				magicStatus = 0;
			}
			else if(magicStatus == 2)
			{
				bossMode = 9;
				waitClock.restart();
				magicStatus = 0;
			}
			break;
		case 9:
			if(waitClock.getElapsedTime().asMilliseconds() > 2000)
			{
				bossMode = 2;
				waitClock.restart();
			}
			break;
		}
	}

	if(laserClock.getElapsedTime().asMilliseconds() > 5000)
	{
		Weapon *laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,96+16,96+25,DOWN);
		((Weapons::Laser *)laser)->setShotDuration(500);

		laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,576+16,96+25,DOWN);
		((Weapons::Laser *)laser)->setShotDuration(500);

		laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,128,288+16,RIGHT);
		((Weapons::Laser *)laser)->setShotDuration(500);

		laser = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,576,288+16,LEFT);
		((Weapons::Laser *)laser)->setShotDuration(500);

		laserClock.restart();
	}
}

void Gandalf::interact(interactType interact_Type)
{
	if(bossMode < 4 || bossMode >= 8)
	{
		switch(interact_Type)
		{
		case interactType::Hit_Interact:
		case interactType::Laser_Interact:
		case interactType::Explosion_Interact:
		case interactType::Arrow_Interact:
		case interactType::Boomerang_Interact:
		case interactType::Hammer_Interact:
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
	else if(bossMode == 6)
	{
		bossMode = 7;
	}
}

void Gandalf::draw(sf::RenderWindow &mainWindow)
{
	if(explosionCounter < 6) Enemy::draw(mainWindow);
}

void Gandalf::hurt(float damage)
{
	DOUT("Enemy damage: " << damage)

	live -= damage;

	Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x+32,enemySprite.getPosition().y+32,650,true,sf::Color::Blue);
	
	enemySprite.setColor(sf::Color(255,255,255,0));
	tp(-64,-64);
	bossMode = 3;

	if(live <= 0)
	{
		Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Stopped);
		waitClock.restart();
		bossMode = 4;
	}

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Hit);
}

void Gandalf::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	std::string command = "kill-boss-1";
	Quest_Manager::getMe().doQuestCommand(command);

	if(Map_Manager::getMe().getCurrentMap().getID() == 997)
	{
		Command_Manager::getMe().doCommand("doObjectCmd cmd:open 2");
		Command_Manager::getMe().doCommand("doObjectCmd cmd:open 1");
		Player_Manager::getMe().setPlayerCanMove(true);
		Player_Manager::getMe().give(50,1);
		Player_Manager::getMe().give(100,1);
	}

	Player_Manager::getMe().getGameStats().addMonsterKill();
 	setActiv(false);
 }

std::string Gandalf::getMySaveData()
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

bool Gandalf::loadMySaveData(std::string data)
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

void Gandalf::calculateNewPosition()
{
	enemySprite.setColor(sf::Color(255,255,255,0));

	int rand = std::rand()%4;

	if(Player_Manager::getMe().getMapPosX() < 224)
	{
		if(rand <= 1) tpToFixPoint(RIGHT);
		else if(rand == 2) tpToFixPoint(UP);
		else tpToFixPoint(DOWN);
	}
	else if(Player_Manager::getMe().getMapPosX() > 480)
	{
		if(rand <= 1) tpToFixPoint(LEFT);
		else if(rand == 2) tpToFixPoint(UP);
		else tpToFixPoint(DOWN);
	}
	else if(Player_Manager::getMe().getMapPosY() > 208)
	{
		if(rand <= 1) tpToFixPoint(UP);
		else if(rand == 2) tpToFixPoint(LEFT);
		else tpToFixPoint(RIGHT);
	}
	else
	{
		if(rand <= 1) tpToFixPoint(DOWN);
		else if(rand == 2) tpToFixPoint(LEFT);
		else tpToFixPoint(RIGHT);
	}

	bossMode = 0;
}

void Gandalf::tpToFixPoint(short dir)
{
	switch(dir)
	{
	case LEFT:
		tp(128,161);
		enemySprite.setTextureRect(GANDALF_RIGHT);
		lookDir = RIGHT;
		break;
	case RIGHT:
		tp(512,161);
		enemySprite.setTextureRect(GANDALF_LEFT);
		lookDir = LEFT;
		break;
	case UP:
		tp(320,64);
		enemySprite.setTextureRect(GANDALF_DOWN);
		lookDir = DOWN;
		break;
	case DOWN:
		tp(320,288);
		enemySprite.setTextureRect(GANDALF_UP);
		lookDir = UP;
		break;
	}
}

Gandalf::~Gandalf(void)
{
}