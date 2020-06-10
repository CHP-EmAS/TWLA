#include "Weapons.h"

#include "NPC.h"
#include "Enemy.h"
#include "Player_Manager.h"
#include "Particle_Manager.h"
#include "Animation_Manager.h"

using namespace Weapons;

Magic::Magic(float speed,short dir,float damage,int spawnPosX,int spawnPosY)
:Weapon(dir,damage,spawnPosX,spawnPosY)
{
	this->boundObjCBox = nullptr;
	this->targetObjCBox = nullptr;
	status = nullptr;

	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	weaponSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,821));
	weaponSprite.setOrigin(8,8);
	weaponSprite.setScale(SCALE,SCALE);

	starSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	starSprite.setTextureRect(sf::IntRect(22*TILESIZE,16*TILESIZE,13,13));
	starSprite.setOrigin(6,6);

	lastStarPos = sf::Vector2i(weaponSprite.getPosition());

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

	this->speed = speed;
	coveredTrack = 0;

	flyClock.restart();

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Magic);
}

void Magic::setBoundObj(collisionbox *boundObjCBox)
{
	this->boundObjCBox = boundObjCBox;
}

void Magic::setTargetObj(collisionbox *targetObjCBox)
{
	this->targetObjCBox = targetObjCBox;
}

void Magic::setStatusVariable(short *status)
{
	this->status = status;
	*status = 0;
}

void Magic::update()
{
	collisionbox cBox;
	cBox.mapPosX = mapPosX - (weaponSprite.getOrigin().x * SCALE) + 7;
	cBox.mapPosY = mapPosY - (weaponSprite.getOrigin().y * SCALE) + 7;
	cBox.width = 10;
	cBox.height = 10;

	if(flyClock.getElapsedTime().asMilliseconds() >= 8)
	{
		weaponSprite.rotate(-speed*2);

		if(targetObjCBox != nullptr)
		{
			sf::Vector2i boxPos = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(targetObjCBox->mapPosX,targetObjCBox->mapPosY));

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

			if(Collision_System::getMe().boxCollision(targetObjCBox,&cBox))
			{
				Animation_Manager::getMe().doExplosion(weaponSprite.getPosition().x,weaponSprite.getPosition().y,500,true,sf::Color::White,true);

				if(targetObjCBox->type == Player_Collisionbox) Player_Manager::getMe().hurt(damage);
				else
				{
					if(targetObjCBox->boundNPC != nullptr) targetObjCBox->boundNPC->interact(Hit_Interact);
					if(targetObjCBox->boundObj != nullptr) targetObjCBox->boundObj->interact(Hit_Interact);
					if(targetObjCBox->boundEnemy != nullptr) targetObjCBox->boundEnemy->interact(Hit_Interact);
				}

				isClosed = true;
				if(status!=nullptr) *status = 1;
			}
		}

		mapPosX += speedX;
		mapPosY += speedY;

		coveredTrack += speed;

		if(coveredTrack > 30*ZOOMSIZE)
		{
			Animation_Manager::getMe().doExplosion(weaponSprite.getPosition().x,weaponSprite.getPosition().y,500,true,sf::Color::White,true);
			isClosed = true;
			if(status!=nullptr) *status = 2;
		}

		flyClock.restart();
	}

	if(damageClock.getElapsedTime().asMilliseconds() >= 50)
	{
		checkCollision(cBox);
		damageClock.restart();
	}

	if(spawnStarClock.getElapsedTime().asMilliseconds() > 100)
	{
		spawnStarClock.restart();

		starSprite.setPosition(lastStarPos.x+rand()%10-5,lastStarPos.y+rand()%10-5);
		Particle_Manager::getMe().addParticle(starSprite,500,20,0,0,true,true);

		lastStarPos = sf::Vector2i(weaponSprite.getPosition());
	}
	
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
}

void Magic::checkCollision(collisionbox &cBox)
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

		switch(cursor->type)
		{
		case Player_Collisionbox:
			Player_Manager::getMe().hurt(damage);
		break;
		case Interactiv_Collisionbox:
		case NPC_Collisionbox:
		case Enemy_Collisionbox:
		case Interact_Only:
			if(cursor->boundNPC != nullptr) cursor->boundNPC->interact(Hit_Interact);
			if(cursor->boundObj != nullptr) cursor->boundObj->interact(Hit_Interact);
			if(cursor->boundEnemy != nullptr) cursor->boundEnemy->interact(Hit_Interact);
		break;
		}

		Animation_Manager::getMe().doExplosion(weaponSprite.getPosition().x,weaponSprite.getPosition().y,500,true,sf::Color::White,true);

		isClosed = true;
		if(status!=nullptr) *status = 2;
	}

	Collision_System::getMe().get_colliding_Normal_Collisionboxes(collisionlist,myBoxPos.x,myBoxPos.y,cBox.width,cBox.height);

	if(collisionlist.size() > 0)
	{
		Animation_Manager::getMe().doExplosion(weaponSprite.getPosition().x,weaponSprite.getPosition().y,500,true,sf::Color::White,true);

		isClosed = true;
		if(status!=nullptr) *status = 2;
	}

	Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);
}

Magic::~Magic()
{

}