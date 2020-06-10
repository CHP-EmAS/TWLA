#include "Weapons.h"

#include "Weapon_Manager.h"
#include "Particle_Manager.h"
#include "NPC_Manager.h"
#include "Player_Manager.h"

using namespace Weapons;

Bow_Arrow::Bow_Arrow(Weapon_Manager::weapon arrow,unsigned int speed, short dir, float damage,int mapPosX,int mapPosY) : Weapon(dir, damage,mapPosX,mapPosY)
{
	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());

	switch(arrow)
	{
	case Weapon_Manager::Wood_Arrow:
		weaponSprite.setTextureRect(sf::IntRect(198,112,5,16));
		weaponSprite.setOrigin(2,0);
		break;
	case Weapon_Manager::Feather_Arrow:
		weaponSprite.setTextureRect(sf::IntRect(5,128,7,16));
		weaponSprite.setOrigin(3,0);
		break;
	case Weapon_Manager::Iron_Arrow:
		weaponSprite.setTextureRect(sf::IntRect(21,128,7,16));
		weaponSprite.setOrigin(3,0);
		break;
	}

	switch(dir)
	{
	case LEFT:
		weaponSprite.setRotation(270);
		speedX = -int(speed);
		speedY = 0;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-1000,weaponSprite.getPosition().y-1,1000,3);
		break;
	case RIGHT:
		weaponSprite.setRotation(90);
		speedX = speed;
		speedY = 0;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x,weaponSprite.getPosition().y-1,1000,3);
		break;
	case DOWN:
		weaponSprite.setRotation(180);
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
	hasTargetHit = false;
	this->speed = speed;

	Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Arrow_Shoot);
}

void Bow_Arrow::update()
{
	if(flyClock.getElapsedTime().asMilliseconds() >= 10)
	{
		if(!hasTargetHit)
		{
			mapPosX += speedX;
			mapPosY += speedY;
		}

		coveredTrack += speed;

		if(coveredTrack >= 40 * ZOOMSIZE)
		{
			if(hasTargetHit)
			{
				if(weaponSprite.getColor().a > 2)
					weaponSprite.setColor(sf::Color(255,255,255,weaponSprite.getColor().a-2));
				else
					isClosed = true;
			}
			else
			{
				isClosed = true;
			}
		}

		if(!hasTargetHit)
		{
			collisionbox* collision = Collision_System::getMe().isCollide(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y-1,3,3);

			if(collision->type != None_Collisionbox)
			{
				hasTargetHit = true;

				if(collision->type == collisionboxType::Standart_Collisionbox)
					Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Arrow_Hit_No_Target);
				else
					Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Arrow_Hit);

				Weapon_Manager::getMe().setCurrentInteractWeapon(this);

				switch(collision->type)
				{
				case Interactiv_Collisionbox:
					if(collision->boundObj != nullptr)
						collision->boundObj->interact(Arrow_Interact);
					break;
				case Player_Collisionbox:
					Player_Manager::getMe().hurt(damage);
					isClosed = true;
					break;
				case NPC_Collisionbox:
					if(collision->boundNPC != nullptr)
						collision->boundNPC->interact(interactType::Arrow_Interact);
					isClosed = true;
					break;
				case Enemy_Collisionbox:
					if(collision->boundEnemy != nullptr)
						collision->boundEnemy->interact(interactType::Arrow_Interact);
					isClosed = true;
					break;
				case Interact_Only:
					if(collision->boundNPC != nullptr)
						collision->boundNPC->interact(interactType::Arrow_Interact);

					if(collision->boundObj != nullptr)
						collision->boundObj->interact(interactType::Arrow_Interact);

					if(collision->boundEnemy != nullptr)
						collision->boundEnemy->interact(interactType::Arrow_Interact);
					break;
				}

				Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);
			}
		}

		flyClock.restart();
	}

	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
}

Bow_Arrow::~Bow_Arrow(void)
{
}
