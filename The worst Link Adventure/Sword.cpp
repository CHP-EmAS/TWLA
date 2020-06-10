#include "Weapons.h"

#include "Player_Manager.h"
#include "Map_Manager.h"
#include "InteractivObject.h"
#include "NPC_Manager.h"
#include "Collision_System.h"

using namespace Weapons;

Sword::Sword(Weapon_Manager::weapon sword, short dir, float damage,int mapPosX,int mapPosY) : Weapon(dir, damage,mapPosX,mapPosY)
{
	isFinished = nullptr;

	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));

	switch(sword)
	{
	case Weapon_Manager::weapon::Woodsword:
		weaponSprite.setTextureRect(sf::IntRect(68,16,8,16));
		weaponSprite.setOrigin(3,14);
		break;
	case Weapon_Manager::weapon::Ironsword:
		weaponSprite.setTextureRect(sf::IntRect(68,32,8,16));
		weaponSprite.setOrigin(3,14);
		break;
	case Weapon_Manager::weapon::Goldsword:
		weaponSprite.setTextureRect(sf::IntRect(83,16,10,16));
		weaponSprite.setOrigin(4,14);
		break;
	case Weapon_Manager::weapon::Herosword:
		weaponSprite.setTextureRect(sf::IntRect(227,96,10,16));
		weaponSprite.setOrigin(4,14);
		break;
	case Weapon_Manager::weapon::Godsword:
		weaponSprite.setTextureRect(sf::IntRect(68,16,8,16));
		weaponSprite.setOrigin(4,14);
		break;
	}

	weaponSprite.setScale(SCALE,SCALE);

	weaponSprite.setRotation(0);

	switch(dir)
	{
	case LEFT:
		weaponSprite.rotate(270);
		handX = 2;
		handY = 26;
		break;
	case RIGHT:
		weaponSprite.rotate(90);
		handX = 30;
		handY = 22;
		break;
	case DOWN:
		weaponSprite.rotate(180);
		handX = 26;
		handY = 32;
		break;
	case UP:
		weaponSprite.rotate(0);
		handX = 6;
		handY = 2;
		break;
	}

	weaponSprite.setPosition(mapPosX,mapPosY);

	hitRange = 9;
	ticker = 0;

	if(mapPosX != -1000 && mapPosY != -1000)
	{
		switch(rand() % 4)
		{
		case 0:
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Sword_Slash_1);
			break;
		case 1:
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Sword_Slash_2);
			break;
		case 2:
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Sword_Slash_3);
			break;
		case 3:
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Sword_Slash_4);
			break;
		}
	}
}

void Sword::update()
{
	if(hitAniClock.getElapsedTime().asMilliseconds() > 4)
	{
		hitAniClock.restart();

		if(hitRange > 0) hitRange--;

		if(ticker > 40)
		{
			close();
		}
		
		if(hitRange == 1)
		{
			int currentPosX = mapPosX + Map_Manager::getMe().getMapPosX() + handX;;
			int currentPosY = mapPosY + Map_Manager::getMe().getMapPosY() + handY;;

			Weapon_Manager::getMe().setCurrentInteractWeapon(this);

			switch(dir)
			{
			case LEFT:
				currentPosX += hitRange;
				currentPosX -= 28;
				Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(currentPosX,currentPosY-3),sf::Vector2i(16,3),this->damage,Hit_Interact);
				break;
			case RIGHT:
				currentPosX -= hitRange;
				currentPosX += 28;
				Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(currentPosX-16,currentPosY),sf::Vector2i(16,3),this->damage,Hit_Interact);
				break;
			case DOWN:
				currentPosY -= hitRange;
				currentPosY += 28;
				Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(currentPosX-3,currentPosY-16),sf::Vector2i(3,16),this->damage,Hit_Interact);
				break;
			case UP:
				currentPosY += hitRange;
				currentPosY -= 28;
				Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(currentPosX,currentPosY),sf::Vector2i(3,16),this->damage,Hit_Interact);
				break;
			}

			Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);
		}

		ticker++;
	}

	switch(dir)
	{
	case LEFT:
		weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX() + handX + hitRange,mapPosY + Map_Manager::getMe().getMapPosY() + handY);
		break;
	case RIGHT:
		weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX() + handX - hitRange,mapPosY + Map_Manager::getMe().getMapPosY() + handY);
		break;
	case DOWN:
		weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX() + handX,mapPosY + Map_Manager::getMe().getMapPosY() + handY - hitRange);
		break;
	case UP:
		weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX() + handX,mapPosY + Map_Manager::getMe().getMapPosY() + handY + hitRange);
		break;
	}
}

void Sword::setIsFinishedVariable(bool *boolVar)
{
	isFinished = boolVar;
}

void Sword::setDamage(float damage)
{
	this->damage = damage;
}

void Sword::close()
{
	isClosed = true;

	if(isFinished != nullptr)
		*isFinished = true;
}

Sword::~Sword(void)
{
}
