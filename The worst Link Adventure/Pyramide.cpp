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

#define HEAD sf::IntRect(13*TILESIZE,12*TILESIZE,TILESIZE*2,TILESIZE*2)
#define BODY sf::IntRect(15*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)

#define FIRE sf::IntRect(18*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)
#define FIRE_2 sf::IntRect(21*TILESIZE,12*TILESIZE,TILESIZE,TILESIZE)

#define CROSSHAIR sf::IntRect(42*TILESIZE,0*TILESIZE,TILESIZE,TILESIZE)

Pyramide::Pyramide(int mapPosX,int mapPosY) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setTextureRect(HEAD);
	enemySprite.setScale(2,2);
	enemySprite.setOrigin(16,16);

	spr_body.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_body.setTextureRect(BODY);
	spr_body.setScale(2,2);
	spr_body.setOrigin(8,8);

	spr_fireball.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_fireball.setTextureRect(FIRE);
	spr_fireball.setScale(2,2);
	spr_fireball.setOrigin(8,8);

	spr_crosshair.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_crosshair.setTextureRect(CROSSHAIR);
	spr_crosshair.setScale(2,2);
	spr_crosshair.setOrigin(8,8);

	canMove = true;
	canCollide = true;
	init = false;
	isAttacking = true;
	updateAlways = true;
	showLiveBar = false;

	Enemy::type = EnemyType::Pyramide;

	name = "Pyramide";

	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		damage = 0.25;
		bodyLength = 5;
		break;
	case NORMAL:
		damage = 0.5;
		bodyLength = 10;
		break;
	case DIFFICULT:
		damage = 1;
		bodyLength = 12;
		break;
	case HARDCORE:
		damage = 2;
		bodyLength = 15;
		break;
	default:
		damage = 0.5;
		bodyLength = 10;
		break;
	}

	fullLive = live;

	bossMode = 0;

	for(int i = 0; i < bodyLength; i++)
	{
		bodyTile newBody;

		newBody.mapPos.x = mapPosX;
		newBody.mapPos.y= mapPosY;

		bodyList.push_back(newBody);
	}

	moveable::m_collisionbox.width = 60;
	moveable::m_collisionbox.height = 60;
	moveable::cBoxPosX = -30;
	moveable::cBoxPosY = -30;

	currentDir = RIGHT;
	enemySprite.rotate(270);
	fireballAmount = 0;
	explosionCounter = 0;
	fireState = true;
}

void Pyramide::update()
{
	bool secondTP = false;

	if(!init)
	{
		secondTP = true;
		Sound_Manager::getMe().changeBackgroundMusic("Music/Boss Theme.ogg");
	}

	Enemy::update();

	if(secondTP) tp(spawnPosX,spawnPosY);

	if(updateClock.getElapsedTime().asMilliseconds() >= (bodyList.size() / 2) + 2)
	{
		updateClock.restart();
		
		switch(bossMode)
		{
		case 0:
			if(waitClock.getElapsedTime().asMilliseconds() > 300)
			{
				waitClock.restart();

				if(explosionCounter < 5)
				{
					Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x - 32 + rand() % 64,enemySprite.getPosition().y - 32 + rand() % 64,650,true,sf::Color::Yellow);
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Bomb_Explosion);
					waitClock.restart();

					explosionCounter++;
				}
				else
				{
					bossMode = 1;
					explosionCounter = 0;
				}
				
			}
			break;
		case 1:
			moveHead();

			if(waitClock.getElapsedTime().asSeconds() > 13 && ((mapPosX > 280 && mapPosX < 360) || (mapPosY > 280 && mapPosY < 360)))
			{
				bossMode = 2;
				waitClock.restart();
			}

			break;
		case 2:
			if(waitClock.getElapsedTime().asMilliseconds() > 500)
			{
				if(fireballAmount >= bodyLength - 3 - bodyList.size()/2)
				{
					bossMode = 1;
					fireballAmount = 0;
				}

				fireball newfire;
				newfire.flyMapPos.x = Player_Manager::getMe().getMapPosX() + 16;
				newfire.flyMapPos.y = Player_Manager::getMe().getMapPosY() + 16;
				newfire.spawnPos.x = mapPosX;
				newfire.spawnPos.y = mapPosY;
				newfire.mapPos = newfire.spawnPos;

				fireballList.push_back(newfire);

				fireballAmount++;

				waitClock.restart();
			}
			break;
		case 3:
			Weapon_Manager::getMe().deleteAllWeapons();
			bossMode = 4;
			break;
		case 4:
			if(waitClock.getElapsedTime().asMilliseconds() > 300)
			{
				waitClock.restart();

				if(explosionCounter <= 9)
				{
					Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x - 32 + rand() % 64,enemySprite.getPosition().y - 32 + rand() % 64,650,true,sf::Color::Yellow);
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Bomb_Explosion);
					waitClock.restart();

					explosionCounter++;
				}
				else
				{
					if(Animation_Manager::getMe().getRunningAnimationAmount() == 0 && Particle_Manager::getMe().getActivParticleAmount() == 0)
					{
						std::vector<int> temp;
						kill(temp,0);
					}
				}
				
			}
			break;
		}
	}

	if(fireballClock.getElapsedTime().asMilliseconds() > 5)
	{
		fireballClock.restart();
		calculateFireball();

		if(fireState)
		{
			spr_fireball.setTextureRect(FIRE_2);
		}
		else
		{
			spr_fireball.setTextureRect(FIRE);
		}

		fireState = !fireState;
	}
}

void Pyramide::interact(interactType interact_Type)
{
	if(bossMode > 2) return;

	switch(interact_Type)
	{
	case interactType::Explosion_Interact:
	{
		Weapon *interactWeapon = &Weapon_Manager::getMe().getCurrentInteractWeapon();

		if(interactWeapon != nullptr)
		{
			if(bodyList.size() > 0)
			{
				sf::Vector2f popPos = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f((*(--bodyList.end())).mapPos));
				Animation_Manager::getMe().doExplosion(popPos.x,popPos.y,650,true,sf::Color::Yellow);
				bodyList.pop_back();
				Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Hit);
			}
			else
			{
				bossMode = 3;
			}
		}
		else
		{
			FOUT("Die aktuelle interaktions Waffe ist NULL!")
		}

		break;
	}
	}
}

void Pyramide::draw(sf::RenderWindow &mainWindow)
{
	for(std::list<bodyTile>::iterator cursor = bodyList.begin() ; cursor != bodyList.end(); ++cursor)
	{
		spr_body.setPosition(sf::Vector2f(Map_Manager::getMe().convert_MapPos_to_Pos(cursor->mapPos)));
		mainWindow.draw(spr_body);
	}

	if((bossMode > 0 && explosionCounter<6) || (bossMode == 0 && explosionCounter> 3))
	{
		Enemy::draw(mainWindow);
	}

	for(auto cursor:fireballList)
	{
		float distanceX = (cursor.flyMapPos.x - cursor.spawnPos.x);
		float distanceY = (cursor.flyMapPos.y - cursor.spawnPos.y);

		spr_fireball.setRotation((atan2(distanceY,distanceX) / 3.1415926 * 180.f) - 90.f);

		spr_fireball.setPosition(sf::Vector2f(Map_Manager::getMe().convert_MapPos_to_Pos(cursor.mapPos)));
		mainWindow.draw(spr_fireball);

		spr_crosshair.setPosition(sf::Vector2f(Map_Manager::getMe().convert_MapPos_to_Pos(cursor.flyMapPos)));
		mainWindow.draw(spr_crosshair);
	}
}

void Pyramide::hurt(float damage)
{

}

void Pyramide::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	std::string command = "kill-boss-2";
	Quest_Manager::getMe().doQuestCommand(command);

	Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Stopped);

	if(Map_Manager::getMe().getCurrentMap().getID() == 995)
	{
		Command_Manager::getMe().doCommand("doObjectCmd cmd:open 4");
		Player_Manager::getMe().give(50,1);
		Player_Manager::getMe().give(102,1);
	}

	Player_Manager::getMe().getGameStats().addMonsterKill();
 	setActiv(false);
 }

std::string Pyramide::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live) + ",";
	fullData += std::to_string(this->isAttacking) + ",";
	fullData += std::to_string(this->bossMode) + ",";
	fullData += std::to_string(this->currentDir) + ",";
	fullData += std::to_string(this->bodyList.size()) + ",";

	return fullData;
}

bool Pyramide::loadMySaveData(std::string data)
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

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_currentDir = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_size = data.substr(0,pos);

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->live = atof(str_live.c_str());
	this->isAttacking = atoi(str_isAttacking.c_str());
	this->bossMode = atoi(str_bossMode.c_str());

	for (std::list<bodyTile>::iterator cursor = bodyList.begin() ; cursor != bodyList.end(); ++cursor)
	{
		cursor->mapPos.x = atoi(str_mapPosX.c_str());
		cursor->mapPos.y = atoi(str_mapPosY.c_str());
	}

	currentDir = atoi(str_currentDir.c_str());

	switch(currentDir)
	{
	case LEFT:
		enemySprite.setRotation(90);
		break;
	case RIGHT:
		enemySprite.setRotation(270);
		break;
	case UP:
		enemySprite.setRotation(180);
		break;
	case DOWN:
		enemySprite.setRotation(0);
		break;
	}

	bodyList.clear();

	for(int i = 0; i < atoi(str_size.c_str()); i++)
	{
		bodyTile newBody;

		newBody.mapPos.x = mapPosX;
		newBody.mapPos.y= mapPosY;

		bodyList.push_back(newBody);
	}

	return true;
}

void Pyramide::moveHead()
{
	sf::Vector2i lastMovement;

	//moveHead
	collisionbox *collision = move(currentDir);

	if(collision != nullptr)
	{
		if(collision->type != None_Collisionbox)
		{
			if(currentDir == LEFT)
			{
				currentDir = UP;
				enemySprite.setRotation(180);
			}
			else if(currentDir == RIGHT)
			{
				currentDir = DOWN;
				enemySprite.setRotation(0);
			}
			else if(currentDir == UP)
			{
				currentDir = RIGHT;
				enemySprite.setRotation(270);
			}
			else if(currentDir == DOWN)
			{
				currentDir = LEFT;
				enemySprite.setRotation(90);
			}
		}
		else
		{
			switch(currentDir)
			{
			case LEFT:
				lastMovement.x = -1;
				lastMovement.y = 0;
				break;
			case RIGHT:
				lastMovement.x = 1;
				lastMovement.y = 0;
				break;
			case UP:
				lastMovement.x = 0;
				lastMovement.y = -1;
				break;
			case DOWN:
				lastMovement.x = 0;
				lastMovement.y = 1;
				break;
			}
		}
	}

	int bodyPos = 0; 

	for (std::list<bodyTile>::iterator cursor = bodyList.begin() ; cursor != bodyList.end(); ++cursor)
	{
		int distance = 31;

		if(bodyPos == 0)
		{
			cursor->lastMovements.push_front(lastMovement);
			distance = 43;
		}

		if(cursor->lastMovements.size() > distance)
		{
			sf::Vector2i movment = *(--cursor->lastMovements.end());
			
			cursor->mapPos += movment;

			if(cursor != (--bodyList.end()))
			{
				std::list<bodyTile>::iterator tempCursor = cursor;

				tempCursor++;

				tempCursor->lastMovements.push_front(movment);
			}

			cursor->lastMovements.pop_back();
		}

		bodyPos++;
	}
}

void Pyramide::calculateFireball()
{
	float speed = 1.5;
	float speedX;
	float speedY;

	for(std::list<fireball>::iterator cursor = fireballList.begin() ; cursor != fireballList.end(); NULL)
	{
		float distanceX = (cursor->mapPos.x  - cursor->flyMapPos.x);
		float distanceY = (cursor->mapPos.y  - cursor->flyMapPos.y);

		if(distanceX < 0) distanceX *= -1;
		if(distanceY < 0) distanceY *= -1;

		float percentX = (distanceX * 100.f) / (distanceX + distanceY);
		float percentY = (distanceY * 100.f) / (distanceX + distanceY);

		if(cursor->mapPos.x  > cursor->flyMapPos.x) speedX = -((speed / 100.f) * percentX);
		else if(cursor->mapPos.x  < cursor->flyMapPos.x) speedX = ((speed / 100.f) * percentX);
		else speedX = 0;

		if(cursor->mapPos.y > cursor->flyMapPos.y) speedY = -((speed / 100.f) * percentY);
		else if(cursor->mapPos.y < cursor->flyMapPos.y) speedY = ((speed / 100.f) * percentY);
		else speedY = 0;

		cursor->mapPos.x += speedX;
		cursor->mapPos.y += speedY;

		if(cursor->mapPos.x > cursor->flyMapPos.x - 1 && cursor->mapPos.x < cursor->flyMapPos.x + 1 && cursor->mapPos.y > cursor->flyMapPos.y - 1 && cursor->mapPos.y < cursor->flyMapPos.y + 1)
		{
			sf::Vector2f pos = Map_Manager::getMe().convert_MapPos_to_Pos(cursor->flyMapPos);
			Animation_Manager::getMe().doExplosion(pos.x,pos.y,650,true,sf::Color::Yellow,false);
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Bomb_Explosion);

			Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(pos - sf::Vector2f(30,30)),sf::Vector2i(60,60),damage,interactType::Explosion_Interact);

			cursor = fireballList.erase(cursor);
		}
		else
		{
			cursor++;
		}
	}
}

Pyramide::~Pyramide(void)
{
}