#pragma once

#include "Item_List.h"

class Weapon
{
public:
	Weapon(short dir,float damage,int mapPosX,int mapPosY);

	virtual void update() = 0;
	virtual void draw(sf::RenderWindow &mainWindow);

	virtual void close();

	float getDamage();
	bool isWeaponClosed();

	const sf::Sprite &getWeaponSprite();

	virtual ~Weapon(void);
protected:
	float damage;

	short dir;

	float mapPosX;
	float mapPosY;

	sf::Sprite weaponSprite;

	bool isClosed;
};

