#pragma once

#include <list>
#include "Weapon.h"

#include "Defines.h"

class Weapon_Manager : public Singleton<Weapon_Manager>
{

friend class Singleton<Weapon_Manager>;

public:

	enum weapon
	{
		Woodsword,
		Ironsword,
		Goldsword,
		Herosword,

		Bomb,
		Waterbomb,

		Wood_Arrow,
		Feather_Arrow,
		Iron_Arrow,
		
		Boomerang,
		Grapplinghock,
		
		Bombbow,
		Laser,
		Magic,

		Godsword = 998,
		God_Arrow,
	};

	Weapon *useWeapon(weapon weaponID,int mapPosX,int mapPosY,int dir);

	void doDamageOnPos(sf::Vector2i pos, sf::Vector2i size,float dmg,interactType interactType = Standart_Interact);

	void drawActivWeapons(sf::RenderWindow &mainWindow);
	void updateActivWeapons();

	void setCurrentInteractWeapon(Weapon* weapon);
	void clearCurrentInteractWeapon();

	void deleteAllWeapons();

	Weapon &getCurrentInteractWeapon();
	
	void toggleDamageDebug();

	~Weapon_Manager(void);
private:
	Weapon_Manager(void);

	std::list<Weapon*> activWeaponsList;

	Weapon *currentInteractWeapon;
	Weapon *emptyWeapon;

	bool damageDebug;
};

