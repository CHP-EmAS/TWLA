#include "Weapon.h"

Weapon::Weapon(short dir, float damage,int mapPosX,int mapPosY)
{
	this->damage = damage;
	this->mapPosX = mapPosX;
	this->mapPosY = mapPosY;
	this->dir = dir;

	isClosed = false;
}

void Weapon::draw(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(weaponSprite);
}

const sf::Sprite &Weapon::getWeaponSprite()
{
	return weaponSprite;
}

float Weapon::getDamage()
{
	return damage;
}

bool Weapon::isWeaponClosed()
{
	return isClosed;
}

void Weapon::close()
{
	isClosed = true;
}

Weapon::~Weapon(void)
{
	
}
