#include "Weapons.h"

#include "NPC.h"
#include "Enemy.h"
#include "Player_Manager.h"

using namespace Weapons;

Boomerang::Boomerang(float speed,short dir,float damage,int spawnPosX,int spawnPosY)
:Weapon(dir,damage,spawnPosX,spawnPosY)
{
	this->boundObjCBox = nullptr;
	isArrived = nullptr;

	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	weaponSprite.setTextureRect(sf::IntRect(180,130,8,12));
	weaponSprite.setOrigin(4,6);
	weaponSprite.setScale(SCALE,SCALE);

	switch(dir)
	{
	case LEFT:
		speedX = -speed;
		speedY = 0;
		break;
	case RIGHT:
		
		speedX = speed;
		speedY = 0;
		break;
	case DOWN:
		speedX = 0;
		speedY = speed;
		break;
	case UP:
		speedX = 0;
		speedY = -speed;
		break;
	}

	comeBack = false;
	this->speed = speed;
	coveredTrack = 0;

	Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Boomerang_Throw);
	currentFlyLoopSound = Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Boomerang_Fly,true);

	flyClock.restart();
}

void Boomerang::setBoundObj(collisionbox *boundObjCBox)
{
	this->boundObjCBox = boundObjCBox;
}

void Boomerang::setIsArrivedVariable(bool *boolVar)
{
	this->isArrived = boolVar;
}

void Boomerang::clearIsArrivedVariable()
{
	this->isArrived = nullptr;
}

void Boomerang::update()
{
	collisionbox cBox;
	cBox.mapPosX = mapPosX - (weaponSprite.getOrigin().x * SCALE) + 7;
	cBox.mapPosY = mapPosY - (weaponSprite.getOrigin().y * SCALE) + 7;
	cBox.width = 10;
	cBox.height = 10;

	if(flyClock.getElapsedTime().asMilliseconds() >= 8)
	{
		weaponSprite.rotate(speed*2);

		if(comeBack)
		{
			if(boundObjCBox != nullptr)
			{
				sf::Vector2i boxPos = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(boundObjCBox->mapPosX,boundObjCBox->mapPosY));

				float flyPosX = boxPos.x + (boundObjCBox->width/2);
				float flyPosY = boxPos.y + (boundObjCBox->height/2);
				
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

				if(Collision_System::getMe().boxCollision(boundObjCBox,&cBox))
				{
					close();

					Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Boomerang_Catch);
				}
			}
			else
			{
				if(coveredTrack >= 50 * ZOOMSIZE)
				{
					close();
				}
			}
		}

		if(damageClock.getElapsedTime().asMilliseconds() >= 50)
		{
			sf::Vector2i myBoxPos = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(cBox.mapPosX,cBox.mapPosY));

			std::list<collisionbox*> collisionlist;
			Collision_System::getMe().get_colliding_Interactiv_Collisionboxes(collisionlist,myBoxPos.x,myBoxPos.y,cBox.width,cBox.height);

			Weapon_Manager::getMe().setCurrentInteractWeapon(this);

			for(auto cursor:collisionlist)
			{
				if(boundObjCBox != nullptr)
				{
					if(cursor == boundObjCBox) continue;
				}

				if(cursor->type == collisionboxType::Interactiv_Collisionbox)
				{
					if(cursor->boundObj != nullptr)
					{
						cursor->boundObj->interact(Boomerang_Interact);
						comeBack = true;
					}
				}
				else if(cursor->type == Player_Collisionbox)
				{
					Player_Manager::getMe().hurt(damage);
					comeBack = true;
				}
				else if(cursor->type == NPC_Collisionbox)
				{
					if(cursor->boundNPC != nullptr)
					{
						cursor->boundNPC->interact(Boomerang_Interact);
						comeBack = true;
					}
				}
				else if(cursor->type == Enemy_Collisionbox)
				{
					if(cursor->boundEnemy != nullptr)
					{
						cursor->boundEnemy->interact(Boomerang_Interact);
						comeBack = true;
					}
				}
				else if(cursor->type == Interact_Only)
				{
					if(cursor->boundNPC != nullptr)
						cursor->boundNPC->interact(Boomerang_Interact);

					if(cursor->boundObj != nullptr)
						cursor->boundObj->interact(Boomerang_Interact);

					if(cursor->boundEnemy != nullptr)
						cursor->boundEnemy->interact(Boomerang_Interact);
				}
			}

			Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);

			damageClock.restart();
		}

		mapPosX += speedX;
		mapPosY += speedY;

		coveredTrack += speed;

		if(coveredTrack >= (10 * ZOOMSIZE))
		{
			comeBack = true;
		}

		flyClock.restart();
	}

	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
}

void Boomerang::close()
{
	if(isArrived != nullptr) *isArrived = true;

	isClosed = true;

	if(currentFlyLoopSound  != nullptr)
	{
		currentFlyLoopSound->mainSound.stop();
		currentFlyLoopSound = nullptr;
	}
}

Boomerang::~Boomerang()
{
	close();
}