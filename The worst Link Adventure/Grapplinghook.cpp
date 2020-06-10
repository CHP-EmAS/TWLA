#include "Weapons.h"

#include "NPC.h"

using namespace Weapons;

Grapplinghook::Grapplinghook(float speed,int maxLength,short dir,float damage,int spawnPosX,int spawnPosY)
:Weapon(dir,damage,spawnPosX,spawnPosY)
{
	boundObjPositionX = nullptr;
	boundObjPositionY = nullptr;
	isArrived  = nullptr;

	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	weaponSprite.setTextureRect(sf::IntRect(373,50,8,12));
	weaponSprite.setOrigin(4,6);
	weaponSprite.setScale(SCALE-0.5,SCALE-0.5);
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());

	beginnSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	beginnSprite.setTextureRect(sf::IntRect(341,54,8,4));
	beginnSprite.setOrigin(4,2);
	beginnSprite.setScale(SCALE-0.5,SCALE-0.5);
	beginnSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());

	chainRect.setFillColor(sf::Color::Black);
	chainRect.setOutlineColor(sf::Color::Black);
	chainRect.setOutlineThickness(1);

	startPosition.x = mapPosX;
	startPosition.y = mapPosY;

	switch(dir)
	{
	case LEFT:
		weaponSprite.setRotation(180);
		beginnSprite.setRotation(180);
		speedX = -speed;
		speedY = 0;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-maxLength-speed,weaponSprite.getPosition().y-1,maxLength+speed,3);
		break;
	case RIGHT:
		weaponSprite.setRotation(0);
		beginnSprite.setRotation(0);
		speedX = speed;
		speedY = 0;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x,weaponSprite.getPosition().y-1,maxLength+speed,3);
		break;
	case DOWN:
		weaponSprite.setRotation(90);
		beginnSprite.setRotation(90);
		speedX = 0;
		speedY = speed;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y,3,maxLength+speed);
		break;
	case UP:
		weaponSprite.setRotation(270);
		beginnSprite.setRotation(270);
		speedX = 0;
		speedY = -speed;
		Collision_System::getMe().get_colliding_Collisionboxes(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y-maxLength-speed,3,maxLength+speed);
		break;
	}

	comeBack = false;
	targetHit = false;
	this->speed = speed;
	coveredTrack = 0;
	this->maxLength = maxLength;

	currentFlyLoopSound = Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Hook_Fly,true);
}

void Grapplinghook::update()
{
	if(flyClock.getElapsedTime().asMilliseconds() >= 8)
	{
		if(targetHit)
		{
			float hookPosX = mapPosX + Map_Manager::getMe().getMapPosX();
			float hookPosY = mapPosY + Map_Manager::getMe().getMapPosY();

			float distanceX = (startPosition.x + Map_Manager::getMe().getMapPosX()) - hookPosX;
			float distanceY = (startPosition.y + Map_Manager::getMe().getMapPosY()) - hookPosY;

			if(distanceX < 0) distanceX *= -1;
			if(distanceY < 0) distanceY *= -1;

			float percentX = (distanceX * 100.f) / (distanceX + distanceY);
			float percentY = (distanceY * 100.f) / (distanceX + distanceY);

			if(startPosition.x + Map_Manager::getMe().getMapPosX() > hookPosX) speedX = -((speed / 100.f) * percentX);
			else if(startPosition.x + Map_Manager::getMe().getMapPosX() < hookPosX) speedX = ((speed / 100.f) * percentX);
			else speedX = 0;

			if(startPosition.y + Map_Manager::getMe().getMapPosY() > hookPosY) speedY = -((speed / 100.f) * percentY);
			else if(startPosition.y + Map_Manager::getMe().getMapPosY() < hookPosY) speedY = ((speed / 100.f) * percentY);
			else speedY = 0;

			if(speedX == 0 && speedY == 0)
			{
				close();
			}

			if(boundObjPositionX != nullptr && boundObjPositionY != nullptr)
			{
				*boundObjPositionX += speedX;
				*boundObjPositionY += speedY;
			}

			startPosition.x += speedX;
			startPosition.y += speedY;
		}
		else if(comeBack)
		{
			float startPosX = startPosition.x + Map_Manager::getMe().getMapPosX();
			float startPosY = startPosition.y + Map_Manager::getMe().getMapPosY();

			float distanceX = (mapPosX + Map_Manager::getMe().getMapPosX()) - startPosX;
			float distanceY = (mapPosY + Map_Manager::getMe().getMapPosY()) - startPosY;

			if(distanceX < 0) distanceX *= -1;
			if(distanceY < 0) distanceY *= -1;

			float percentX = (distanceX * 100.f) / (distanceX + distanceY);
			float percentY = (distanceY * 100.f) / (distanceX + distanceY);

			if(mapPosX + Map_Manager::getMe().getMapPosX() > startPosX) speedX = -((speed / 100.f) * percentX);
			else if(mapPosX + Map_Manager::getMe().getMapPosX() < startPosX) speedX = ((speed / 100.f) * percentX);
			else speedX = 0;

			if(mapPosY + Map_Manager::getMe().getMapPosY() > startPosY) speedY = -((speed / 100.f) * percentY);
			else if(mapPosY + Map_Manager::getMe().getMapPosY() < startPosY) speedY = ((speed / 100.f) * percentY);
			else speedY = 0;

			if(speedX == 0 && speedY == 0)
			{
				close();
			}

			mapPosX += speedX;
			mapPosY += speedY;
		}
		else
		{
			collisionbox* collision = Collision_System::getMe().isCollide(collisionlist,weaponSprite.getPosition().x-1,weaponSprite.getPosition().y-1,3,3);

			if(collision->type  == Standart_Collisionbox  || collision->type == Interactiv_Collisionbox || collision->type == NPC_Collisionbox)
			{
				comeBack = true;
				Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Hook_No_Taget_Hit);
			}
			else if(collision->type == Enemy_Collisionbox)
			{
				if(collision->boundNPC != nullptr)
					collision->boundNPC->interact(Hit_Interact);

				targetHit = true;
				Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Hook_Target_Hit);
			}
			else if(collision->type == Enemy_Collisionbox)
			{
				if(collision->boundNPC != nullptr)
					collision->boundNPC->interact(Hit_Interact);

				if(collision->boundObj != nullptr)
					collision->boundObj->interact(Hit_Interact);

				if(collision->boundNPC != nullptr)
					collision->boundNPC->interact(Hit_Interact);
			}
			else if(collision->type == Grabable_Collisionbox)
			{
				targetHit = true;
				Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Hook_Target_Hit);
			}
			else
			{
				mapPosX += speedX;
				mapPosY += speedY;
				coveredTrack += speed;

				if(coveredTrack >= maxLength)
				{
					comeBack = true;
				}
			}
		}

		flyClock.restart();
	}

	chainRect.setSize(sf::Vector2f(-(mapPosX-startPosition.x),-(mapPosY - startPosition.y)));
	chainRect.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());

	beginnSprite.setPosition(startPosition.x + Map_Manager::getMe().getMapPosX(),startPosition.y + Map_Manager::getMe().getMapPosY());
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
}

void Grapplinghook::draw(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(chainRect);
	mainWindow.draw(beginnSprite);
	mainWindow.draw(weaponSprite);
}

void Grapplinghook::setBoundObjPositions(float *posX,float *posY)
{
	this->boundObjPositionX = posX;
	this->boundObjPositionY = posY;
}

void Grapplinghook::setIsArrivedVariable(bool *boolVar)
{
	this->isArrived = boolVar;
}

void Grapplinghook::close()
{
	if(isArrived != nullptr) *isArrived = true;

	isClosed = true;

	if(currentFlyLoopSound != nullptr)
	{
		currentFlyLoopSound->mainSound.stop();
		currentFlyLoopSound = nullptr;
	}
}

Grapplinghook::~Grapplinghook()
{

}