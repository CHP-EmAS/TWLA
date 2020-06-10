#include "Weapon_Manager.h"

#include "Weapons.h"
#include "NPC.h"
#include "InteractivObject.h"
#include "Enemy.h"

#include "Player_Manager.h"

using namespace Weapons;

Weapon_Manager::Weapon_Manager(void)
{
	emptyWeapon = new Sword(weapon::Woodsword,0,0,-1000,-1000);

	currentInteractWeapon = nullptr;

	NEWOUT("Neuer Weapon_Manager wurde erstellet! (" << this << ")");

	damageDebug = false;
}

Weapon *Weapon_Manager::useWeapon(weapon weaponID,int mapPosX,int mapPosY,int dir)
{
	Weapon *newWeapon = nullptr;

	switch(weaponID)
	{
	case Woodsword:
	{
		newWeapon = new Weapons::Sword(Woodsword,dir,0.5,mapPosX,mapPosY);
		break;
	}
	case Ironsword:
	{
		newWeapon = new Weapons::Sword(Ironsword,dir,0.75,mapPosX,mapPosY);
		break;
	}
	case Goldsword:
	{
		newWeapon = new Weapons::Sword(Goldsword,dir,1.25,mapPosX,mapPosY);
		break;
	}
	case Herosword:
	{
		newWeapon = new Weapons::Sword(Herosword,dir,2.0,mapPosX,mapPosY);
		break;
	}
	case Wood_Arrow:
	{
		newWeapon = new Weapons::Bow_Arrow(Wood_Arrow,6,dir,0.5,mapPosX,mapPosY);
		break;	
	}
	case Feather_Arrow:
	{
		newWeapon = new Weapons::Bow_Arrow(Feather_Arrow,6,dir,0.75,mapPosX,mapPosY);
		break;	
	}
	case Iron_Arrow:
	{
		newWeapon = new Weapons::Bow_Arrow(Iron_Arrow,6,dir,1.5,mapPosX,mapPosY);
		break;	
	}
	case Boomerang:
	{
		newWeapon = new Weapons::Boomerang(5,dir,0.25,mapPosX,mapPosY);
		break;
	}
	case Bomb:
	{
		newWeapon = new Weapons::Bomb(Bomb,4,32,1.5,mapPosX,mapPosY);
		break;
	}
	case Waterbomb:
	{
		newWeapon = new Weapons::Bomb(Waterbomb,4,25,1.25,mapPosX,mapPosY);
		break;
	}
	case Grapplinghock:
	{
		newWeapon = new Weapons::Grapplinghook(6,6 * ZOOMSIZE,dir,0.25,mapPosX,mapPosY);
		break;
	}
	case Bombbow:
	{
		newWeapon = new Weapons::ArrowBomb(6,dir,1.5,mapPosX,mapPosY);
		break;
	}
	case Laser:
	{
		newWeapon = new Weapons::Laser(mapPosX,mapPosY,dir,1,500);
		break;
	}
	case Magic:
	{
		newWeapon = new Weapons::Magic(1,dir,0.25,mapPosX,mapPosY);
		break;
	}
	case Godsword:
	{
		newWeapon = new Weapons::Sword(Herosword,dir,1000,mapPosX,mapPosY);
		break;
	}
	default:
	{
		newWeapon = new Weapons::Sword(Woodsword,dir,0,mapPosX,mapPosY);
		break;
	}
	}

	activWeaponsList.push_back(newWeapon);
	return newWeapon;
}

void Weapon_Manager::doDamageOnPos(sf::Vector2i pos, sf::Vector2i size,float dmg,interactType interactType)
{
	std::list<collisionbox*> collisionlist;
	Collision_System::getMe().get_colliding_Interactiv_Collisionboxes(collisionlist,pos.x,pos.y,size.x,size.y);

	for(auto cursor:collisionlist)
	{
		if(cursor->type == collisionboxType::Interactiv_Collisionbox)
		{
			if(cursor->boundObj != nullptr)
				cursor->boundObj->interact(interactType);
		}
		else if(cursor->type == Player_Collisionbox)
		{
			Player_Manager::getMe().hurt(dmg);
		}
		else if(cursor->type == NPC_Collisionbox)
		{
			if(cursor->boundNPC != nullptr)
				cursor->boundNPC->interact(interactType);
		}
		else if(cursor->type == Enemy_Collisionbox)
		{
			if(cursor->boundEnemy != nullptr)
				cursor->boundEnemy->interact(interactType);
		}
		else if(cursor->type == Interact_Only)
		{
			if(cursor->boundNPC != nullptr)
				cursor->boundNPC->interact(interactType);

			if(cursor->boundObj != nullptr)
				cursor->boundObj->interact(interactType);

			if(cursor->boundEnemy != nullptr)
				cursor->boundEnemy->interact(interactType);
		}
	}

	if(damageDebug)
	{
		//Hitbox Test
		Screen_Manager::getMe().getGameWindow().clear(sf::Color::Green);
		
		sf::RectangleShape box = sf::RectangleShape(sf::Vector2f(size.x,size.y));
		box.setPosition(pos.x,pos.y);
		box.setFillColor(sf::Color::Red);

		Screen_Manager::getMe().getGameWindow().draw(box);
		Screen_Manager::getMe().getGameWindow().display();

		Sleep(500);
	}
}

void Weapon_Manager::drawActivWeapons(sf::RenderWindow &mainWindow)
{
	std::list<Weapon*>::iterator cursor;

	for(cursor = activWeaponsList.begin(); cursor != activWeaponsList.end(); ++cursor)
	{
		if(!(*cursor)->isWeaponClosed())
		{
			(*cursor)->draw(mainWindow);
		}
	}
}

void Weapon_Manager::updateActivWeapons()
{
	std::list<Weapon*>::iterator cursor;

	for(cursor = activWeaponsList.begin(); cursor != activWeaponsList.end(); ++cursor)
	{
		if(!(*cursor)->isWeaponClosed())
		{
			(*cursor)->update();
		}
		else
		{
			delete (*cursor);

			cursor = activWeaponsList.erase(cursor);

			if(cursor == activWeaponsList.end()) 
				break;
		}
	}
}

void Weapon_Manager::setCurrentInteractWeapon(Weapon* weapon)
{
	currentInteractWeapon = weapon;
}

void Weapon_Manager::clearCurrentInteractWeapon()
{
	currentInteractWeapon = nullptr;
}

Weapon &Weapon_Manager::getCurrentInteractWeapon()
{
	if(currentInteractWeapon != nullptr)
		return *currentInteractWeapon;

	return *emptyWeapon;
}

void Weapon_Manager::deleteAllWeapons()
{
	std::list<Weapon*>::iterator cursor;

   	for(cursor = activWeaponsList.begin(); cursor != activWeaponsList.end(); ++cursor)
	{
		(*cursor)->close();
		delete (*cursor);
	}

	activWeaponsList.clear();
}

void Weapon_Manager::toggleDamageDebug()
{
	damageDebug = !damageDebug;
}

Weapon_Manager::~Weapon_Manager(void)
{
	for(auto cursor : activWeaponsList)
	{
		delete cursor;
	}

	delete emptyWeapon;

	DELOUT("Weapon_Manager wurde geloescht!\t(" << this << ")")
}
