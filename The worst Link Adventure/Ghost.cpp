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

#define GHOST_UP sf::IntRect(14*TILESIZE,8*TILESIZE,TILESIZE,TILESIZE)
#define GHOST_UP_2 sf::IntRect(15*TILESIZE,8*TILESIZE,TILESIZE,TILESIZE)
#define GHOST_LEFT sf::IntRect(9*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)
#define GHOST_LEFT_2 sf::IntRect(10*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)
#define GHOST_RIGHT sf::IntRect(10*TILESIZE,12*TILESIZE,-TILESIZE,TILESIZE)
#define GHOST_RIGHT_2 sf::IntRect(11*TILESIZE,12*TILESIZE,-TILESIZE,TILESIZE)

Ghost::Ghost(int mapPosX,int mapPosY) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setPosition(0,0);
	enemySprite.setTextureRect(GHOST_RIGHT);
	enemySprite.setScale(2,2);
	
	m_collisionbox.type = collisionboxType::Interact_Only;
	showLiveBar = false;
	init = false;
	isAttacking = true;

	Enemy::type = EnemyType::Ghost;

	name = "Geist";

	moveable::collisionbox_Update_Range = 120;
	m_collisionbox.height = 32;
	m_collisionbox.width = 32;

	damage = 0.5;
	live = 10;
	
	fullLive = live;

	speed = 0.5;
	speedX = 0;
	speedY = 0;

	flyToPlayer = false;
}

void Ghost::update()
{
	Enemy::update();

	if(updateClock.getElapsedTime().asMilliseconds() >= 10)
	{
		updateClock.restart();

		if(flyToPlayer && enemySprite.getColor().a < 255)
		{
			if(enemySprite.getColor().a + 5 < 255)
			{
				enemySprite.setColor(sf::Color(255,255,255,enemySprite.getColor().a+5));
			}
			else
			{
				enemySprite.setColor(sf::Color(255,255,255,255));
			}
		}
		else if(!flyToPlayer && enemySprite.getColor().a > 0)
		{
			if(enemySprite.getColor().a - 5 > 0)
			{
				enemySprite.setColor(sf::Color(255,255,255,enemySprite.getColor().a-5));
			}
			else
			{
				enemySprite.setColor(sf::Color(255,255,255,0));
			}
		}

		if(flyToPlayer && enemySprite.getColor().a > 200)
		{
			calculateMoveSpeeds();

			mapPosX += speedX;
			mapPosY += speedY;

			m_collisionbox.mapPosX = mapPosX;
			m_collisionbox.mapPosY = mapPosY;
		}
	}
	
	if(checkClock.getElapsedTime().asMilliseconds() >= 100)
	{
		checkClock.restart();

		int myPosX = getPosX() + 16;
		int myPosY = getPosY() + 16;

		short playerDir = Player_Manager::getMe().getLookDir();
		int playerPosX = Player_Manager::getMe().getPosX() + 16;
		int playerPosY = Player_Manager::getMe().getPosY() + 16;

		int distanceToPlayerX = playerPosX - myPosX;
		int distanceToPlayerY = playerPosY - myPosY;

		switch(playerDir)
		{
		case LEFT:
			if(distanceToPlayerX > 0 && myPosY < (playerPosY + distanceToPlayerX) && myPosY > (playerPosY - distanceToPlayerX)) 
				flyToPlayer = false;
			else
				flyToPlayer = true;
			break;
		case RIGHT:
			if(distanceToPlayerX < 0 && myPosY < (playerPosY - distanceToPlayerX) && myPosY > (playerPosY + distanceToPlayerX)) 
				flyToPlayer = false;
			else
				flyToPlayer = true;
			break;
		case UP:
			if(distanceToPlayerY > 0 && myPosX < (playerPosX + distanceToPlayerY) && myPosX > (playerPosX - distanceToPlayerY)) 
				flyToPlayer = false;
			else
				flyToPlayer = true;
			break;
		case DOWN:
			if(distanceToPlayerY < 0 && myPosX < (playerPosX - distanceToPlayerY) && myPosX > (playerPosX + distanceToPlayerY)) 
				flyToPlayer = false;
			else
				flyToPlayer = true;
			break;
			break;
		}
	}

	if(Collision_System::getMe().boxCollision(Player_Manager::getMe().getCollisionbox(),&m_collisionbox))
	{
		flyToPlayer = true;
	}

	if(moneyClock.getElapsedTime().asSeconds() > 1)
	{
		moneyClock.restart();

		if(Collision_System::getMe().boxCollision(Player_Manager::getMe().getCollisionbox(),&m_collisionbox))
		{
			switch(File_Manager::getMe().getGameDifficulty())
			{
			case IMMORTAL:
			case EASY:
			case NORMAL:
				Player_Manager::getMe().getGameHUD().removeEdelsteine(1);
				break;
			case DIFFICULT:
			case HARDCORE:
				Player_Manager::getMe().getGameHUD().removeEdelsteine(2);
				break;
			default:
				Player_Manager::getMe().getGameHUD().removeEdelsteine(1);
				break;
			}

			Animation_Manager::getMe().doExplosion(Player_Manager::getMe().getPosX()+16,Player_Manager::getMe().getPosY()+16,650,true,sf::Color::Green,true);
		}
	}
}

void Ghost::interact(interactType interact_Type)
{
	
}

void Ghost::hurt(float damage)
{
	
}

void Ghost::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	Player_Manager::getMe().getGameStats().addMonsterKill();
}

std::string Ghost::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active)  + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live)    + ",";
	fullData += std::to_string(this->isAttacking) + ",";
	fullData += std::to_string(this->flyToPlayer);

	return fullData;
}

bool Ghost::loadMySaveData(std::string data)
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
	std::string str_flyToPlayer = data.substr(0,pos);

	data = data.substr(pos+1);
	std::string str_currentDir = data;

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->live = atof(str_live.c_str());
	this->isAttacking = atoi(str_isAttacking.c_str());
	this->flyToPlayer = atoi(str_flyToPlayer.c_str());

	return true;
}

void Ghost::calculateMoveSpeeds()
{
	collisionbox *targetObjCBox = Player_Manager::getMe().getCollisionbox();
	sf::Vector2i boxPos = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(targetObjCBox->mapPosX-16,targetObjCBox->mapPosY-16));

	float flyPosX = boxPos.x + (targetObjCBox->width/2);
	float flyPosY = boxPos.y + (targetObjCBox->height/2);
	
	float distanceX = (mapPosX + Map_Manager::getMe().getMapPosX()) - flyPosX;
	float distanceY = (mapPosY + Map_Manager::getMe().getMapPosY()) - flyPosY;

	if(distanceX < 0) distanceX *= -1;
	if(distanceY < 0) distanceY *= -1;

	float percentX = (distanceX * 100.f) / (distanceX + distanceY);
	float percentY = (distanceY * 100.f) / (distanceX + distanceY);

	if(mapPosX + Map_Manager::getMe().getMapPosX() > flyPosX) speedX = -((speed / 100.f) * percentX);
	else if(mapPosX + Map_Manager::getMe().getMapPosX() < flyPosX) speedX = ((speed / 100.f) * percentX);
	else speedX = 0;

	if(mapPosY + Map_Manager::getMe().getMapPosY() > flyPosY) speedY = -((speed / 100.f) * percentY);
	else if(mapPosY + Map_Manager::getMe().getMapPosY() < flyPosY) speedY = ((speed / 100.f) * percentY);
	else speedY = 0;

	if(!Collision_System::getMe().boxCollision(Player_Manager::getMe().getCollisionbox(),&m_collisionbox))
	{
		if(speedY < 0)
		{
			if(speedX < 0)
			{
				if(speedY < speedX) 
					enemySprite.setTextureRect(GHOST_UP);
				else
					enemySprite.setTextureRect(GHOST_LEFT);
			}
			else 
			{
				if((speedY*-1.f) > speedX)
					enemySprite.setTextureRect(GHOST_UP);
				else
					enemySprite.setTextureRect(GHOST_RIGHT);
			}
		}
		else
		{
			if(speedX < 0)
			{
				enemySprite.setTextureRect(GHOST_LEFT);
			}
			else 
			{
				enemySprite.setTextureRect(GHOST_RIGHT);
			}
		}
	}
}

Ghost::~Ghost(void)
{

}