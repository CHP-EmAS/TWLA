#include "Weapons.h"

#include "NPC.h"
#include "Enemy.h"
#include "Player_Manager.h"
#include "Map_Manager.h"
#include "Particle_Manager.h"

using namespace Weapons;

Laser::Laser(int mapPosX_beginn,int mapPosY_beginn,short dir,float damage,int duration_ms)
:Weapon(0,damage,mapPosX_beginn,mapPosY_beginn)
{
	std::list<collisionbox*> collisionlist;
	int maxRange = 400;

	this->dir = dir;

	this->mapPosX_beginn = mapPosX_beginn;
	this->mapPosY_beginn = mapPosY_beginn;

	this->damage = damage;
	this->duration_ms = duration_ms;

	switch(dir)
	{
	case LEFT:
	{
		this->mapPosX_end = mapPosX_beginn - maxRange;
		this->mapPosY_end = mapPosY_beginn;

		sf::Vector2i pos_end = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX_end,mapPosY_end));

		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,pos_end.x,pos_end.y,maxRange,0);
		break;
	}
	case RIGHT:
	{
		this->mapPosX_end = mapPosX_beginn + maxRange;
		this->mapPosY_end = mapPosY_beginn;

		sf::Vector2i pos_beginn = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX_beginn,mapPosY_beginn));

		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,pos_beginn.x,pos_beginn.y,maxRange,0);
		break;
	}
	case UP:
	{
		this->mapPosX_end = mapPosX_beginn;
		this->mapPosY_end = mapPosY_beginn - maxRange;

		sf::Vector2i pos_end = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX_end,mapPosY_end));

		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,pos_end.x,pos_end.y,0,maxRange);
		break;
	}
	case DOWN:
	{
		this->mapPosX_end = mapPosX_beginn;
		this->mapPosY_end = mapPosY_beginn + maxRange;

		sf::Vector2i pos_beginn = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX_beginn,mapPosY_beginn));

		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,pos_beginn.x,pos_beginn.y,0,maxRange);
		break;
	}
	}

	for(auto boxCursor : collisionlist)
	{
		switch(dir)
		{
		case LEFT:
			if(mapPosX_end < boxCursor->mapPosX + boxCursor->width) 
				mapPosX_end = boxCursor->mapPosX + boxCursor->width;
			break;
		case RIGHT:
			if(mapPosX_end > boxCursor->mapPosX) 
				mapPosX_end = boxCursor->mapPosX;
			break;
		case UP:
			if(mapPosY_end < boxCursor->mapPosY + boxCursor->height) 
				mapPosY_end = boxCursor->mapPosY + boxCursor->height;
			break;
		case DOWN:
			if(mapPosY_end > boxCursor->mapPosY) 
				mapPosY_end = boxCursor->mapPosY;
			break;
		}
	}

	laserLine = sf::VertexArray(sf::PrimitiveType::Lines,10); 

	int currentMapPosX = Map_Manager::getMe().getMapPosX();
	int currentMapPosY = Map_Manager::getMe().getMapPosY();

	laserLine[0].position = sf::Vector2f(mapPosX_beginn + currentMapPosX,mapPosY_beginn + currentMapPosY);
	laserLine[1].position = sf::Vector2f(mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY);
	laserLine[2].position = sf::Vector2f(mapPosX_beginn + currentMapPosX+1,mapPosY_beginn + currentMapPosY);
	laserLine[3].position = sf::Vector2f(mapPosX_end + currentMapPosX+1,mapPosY_end + currentMapPosY);
	laserLine[4].position = sf::Vector2f(mapPosX_beginn + currentMapPosX-1,mapPosY_beginn + currentMapPosY);
	laserLine[5].position = sf::Vector2f(mapPosX_end + currentMapPosX-1,mapPosY_end + currentMapPosY);
	laserLine[6].position = sf::Vector2f(mapPosX_beginn + currentMapPosX,mapPosY_beginn + currentMapPosY+1);
	laserLine[7].position = sf::Vector2f(mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY+1);
	laserLine[8].position = sf::Vector2f(mapPosX_beginn + currentMapPosX,mapPosY_beginn + currentMapPosY-1);
	laserLine[9].position = sf::Vector2f(mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY-1);

	laserLine[0].color = sf::Color::Red;
	laserLine[1].color = sf::Color::Red;
	laserLine[2].color = sf::Color::Red;
	laserLine[3].color = sf::Color::Red;
	laserLine[4].color = sf::Color::Red;
	laserLine[5].color = sf::Color::Red;
	laserLine[6].color = sf::Color::Red;
	laserLine[7].color = sf::Color::Red;
	laserLine[8].color = sf::Color::Red;
	laserLine[9].color = sf::Color::Red;

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Laser);
}

void Laser::update()
{
	int currentMapPosX = Map_Manager::getMe().getMapPosX();
	int currentMapPosY = Map_Manager::getMe().getMapPosY();

	laserLine[0].position = sf::Vector2f(mapPosX_beginn + currentMapPosX,mapPosY_beginn + currentMapPosY);
	laserLine[1].position = sf::Vector2f(mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY);
	laserLine[2].position = sf::Vector2f(mapPosX_beginn + currentMapPosX+1,mapPosY_beginn + currentMapPosY);
	laserLine[3].position = sf::Vector2f(mapPosX_end + currentMapPosX+1,mapPosY_end + currentMapPosY);
	laserLine[4].position = sf::Vector2f(mapPosX_beginn + currentMapPosX-1,mapPosY_beginn + currentMapPosY);
	laserLine[5].position = sf::Vector2f(mapPosX_end + currentMapPosX-1,mapPosY_end + currentMapPosY);
	laserLine[6].position = sf::Vector2f(mapPosX_beginn + currentMapPosX,mapPosY_beginn + currentMapPosY+1);
	laserLine[7].position = sf::Vector2f(mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY+1);
	laserLine[8].position = sf::Vector2f(mapPosX_beginn + currentMapPosX,mapPosY_beginn + currentMapPosY-1);
	laserLine[9].position = sf::Vector2f(mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY-1);

	if(durationClock.getElapsedTime().asMilliseconds() > duration_ms)
	{ 
		close();
	}

	if(durationClock.getElapsedTime().asMilliseconds() % 10 == 0)
	{
		Particle_Manager::getMe().addParticle(Particle_Manager::Rectangle,SCALE,sf::Color::Yellow,100,30,mapPosX_end + currentMapPosX,mapPosY_end + currentMapPosY,rand()%5-2,rand()%5-2,true);
	}

	if(damageClock.getElapsedTime().asMilliseconds() > 100)
	{
		std::list<collisionbox*> collisionlist;

		sf::Vector2i pos_end = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX_end,mapPosY_end));
		sf::Vector2i pos_beginn = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX_beginn,mapPosY_beginn));

		Weapon_Manager::getMe().setCurrentInteractWeapon(this);

		switch(dir)
		{
		case LEFT:
			Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(pos_end.x,pos_end.y),sf::Vector2i(pos_beginn.x - pos_end.x,1),damage,interactType::Laser_Interact);
			break;
		case RIGHT:
			Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(pos_beginn.x,pos_beginn.y),sf::Vector2i(pos_end.x - pos_beginn.x,1),damage,interactType::Laser_Interact);
			break;
		case UP:
			Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(pos_end.x,pos_end.y),sf::Vector2i(1,pos_beginn.y - pos_end.y),damage,interactType::Laser_Interact);
			break;
		case DOWN:
			Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(pos_beginn.x,pos_beginn.y),sf::Vector2i(1,pos_end.y - pos_beginn.y),damage,interactType::Laser_Interact);
			break;
		}
		
		Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);

		damageClock.restart();
	}
}

void Laser::draw(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(laserLine);
}

void Laser::setShotDuration(int duration_ms)
{
	this->duration_ms = duration_ms;
}

void Laser::close()
{
	isClosed = true;
}

Laser::~Laser()
{

}