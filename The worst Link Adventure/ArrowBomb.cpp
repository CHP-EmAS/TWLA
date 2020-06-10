#include "Weapons.h"

#include "InteractivObject.h"

#include "Weapon_Manager.h"
#include "Particle_Manager.h"
#include "Animation_Manager.h"
#include "Player_Manager.h"
#include "NPC.h"
#include "Enemy.h"

using namespace Weapons;

ArrowBomb::ArrowBomb(unsigned int speed, short dir, float damage,int mapPosX,int mapPosY) : Weapon(dir, damage,mapPosX,mapPosY)
{
	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	weaponSprite.setTextureRect(sf::IntRect(130,97,13,15));
	weaponSprite.setOrigin(6,7);

	switch(dir)
	{
	case LEFT:
		speedX = -int(speed);
		speedY = 0;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-1000,weaponSprite.getPosition().y-1,1000,3);
		break;
	case RIGHT:
		speedX = speed;
		speedY = 0;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x,weaponSprite.getPosition().y-1,1000,3);
		break;
	case DOWN:
		speedX = 0;
		speedY = speed;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y,3,1000);
		break;
	case UP:
		speedX = 0;
		speedY = -int(speed);
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y-1000,3,1000);
		break;
	}

	weaponSprite.setScale(SCALE-0.5,SCALE-0.5);

	coveredTrack = 0;
	this->speed = speed;
}

void ArrowBomb::update()
{
	if(flyClock.getElapsedTime().asMilliseconds() >= 10)
	{
		
		mapPosX += speedX;
		mapPosY += speedY;
		
		weaponSprite.rotate(-5);

		coveredTrack += speed;

		if(coveredTrack >= 40 * ZOOMSIZE)
		{
			isClosed = true;
		}

		collisionbox *box = Collision_System::getMe().isCollide(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y-1,3,3);

		if(coveredTrack >= ZOOMSIZE && box->type != collisionboxType::None_Collisionbox)
		{
			explode();
		}

		flyClock.restart();
	}

	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
}

void ArrowBomb::explode()
{
	Weapon_Manager::getMe().setCurrentInteractWeapon(this);
	Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(weaponSprite.getPosition().x-32,weaponSprite.getPosition().y-32),sf::Vector2i(64,64),damage,interactType::Explosion_Interact);
	Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);

	Animation_Manager::getMe().doExplosion(weaponSprite.getPosition().x,weaponSprite.getPosition().y,650,true);
	Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Bomb_Explosion);
	isClosed = true;
}

ArrowBomb::~ArrowBomb(void)
{
}