#include "Weapons.h"

#include "Map_Manager.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "InteractivObject.h"
#include "Sound_Manager.h"
#include "NPC.h"
#include "NPC_Manager.h"
#include "Player_Manager.h"

using namespace Weapons;

Bomb::Bomb(Weapon_Manager::weapon bomb,unsigned int sec_explosionTime,unsigned int explosionRadius, float damage,int mapPosX,int mapPosY)
:Weapon(0,damage,mapPosX,mapPosY),
sec_explosionTime(sec_explosionTime)
{
	this->explosionRadius = explosionRadius;

	weaponSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));

	switch(bomb)
	{
	case Weapon_Manager::Bomb:
	{
		weaponSprite.setTextureRect(sf::IntRect(130,97,13,15));
		weaponSprite.setOrigin(6,7);

		int currentPosX = Map_Manager::getMe().getOnMapPosX(mapPosX + Map_Manager::getMe().getMapPosX());
		int currentPosY = Map_Manager::getMe().getOnMapPosY(mapPosY + Map_Manager::getMe().getMapPosY());

		if(Map_Manager::getMe().getCurrentMap().getTile(currentPosX,currentPosY)->settings == WATER)
		{
			canExplode = false;
			this->sec_explosionTime = 0;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Bomb_Defuse);
			currentFuseLoopSound = nullptr;
		}
		else
		{
			canExplode = true;
			currentFuseLoopSound = Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Bomb_Fuse,true);
		}

		break;
	}
	case Weapon_Manager::Waterbomb:
		weaponSprite.setTextureRect(sf::IntRect(116,98,8,13));
		weaponSprite.setOrigin(4,6);
		canExplode = true;
		currentFuseLoopSound = Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Bomb_Fuse,true);
		break;
	}

	weaponSprite.setScale(SCALE,SCALE);
	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());

	explosionsTimeClock.restart();

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Bomb_Drop);
}

void Bomb::update()
{
	if(explosionsTimeClock.getElapsedTime().asSeconds() >= sec_explosionTime)
	{
		if(canExplode)
		{
			Weapon_Manager::getMe().setCurrentInteractWeapon(this);
			Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(weaponSprite.getPosition().x-explosionRadius,weaponSprite.getPosition().y-explosionRadius),sf::Vector2i(explosionRadius*2,explosionRadius*2),damage,interactType::Explosion_Interact);
			Weapon_Manager::getMe().setCurrentInteractWeapon(nullptr);

			Animation_Manager::getMe().doExplosion(weaponSprite.getPosition().x,weaponSprite.getPosition().y,650,true);
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Bomb_Explosion);

			close();
		}
		else
		{
			if(explosionsTimeClock.getElapsedTime().asMilliseconds() % 50 == 0)
			{
				weaponSprite.setColor(sf::Color(255,255,255,weaponSprite.getColor().a - 10));

				if(weaponSprite.getColor().a <= 5)
				{
					close();
				}
			}
		}
	}

	if(canExplode)
	{
		if(explosionsTimeClock.getElapsedTime().asMilliseconds() % 50 == 0)
		{
			if(weaponSprite.getTextureRect().left == 130)
			{
				Particle_Manager::getMe().addParticle(Particle_Manager::Rectangle,SCALE,sf::Color::Yellow,100,30,weaponSprite.getPosition().x-8,weaponSprite.getPosition().y-12,rand()%5-2,rand()%5-2,true);
			}
			else if(weaponSprite.getTextureRect().left == 116)
			{
				Particle_Manager::getMe().addParticle(Particle_Manager::Rectangle,SCALE,sf::Color::Yellow,100,30,weaponSprite.getPosition().x+2,weaponSprite.getPosition().y-10,rand()%5-2,rand()%5-2,true);
			}
		}
	}
	else
	{
		if(explosionsTimeClock.getElapsedTime().asSeconds() >= sec_explosionTime)
		{
			if(explosionsTimeClock.getElapsedTime().asMilliseconds() % 200 == 0)
			{
				Particle_Manager::getMe().addParticle(Particle_Manager::Circle,4,sf::Color(150,150,255,200),100,30,weaponSprite.getPosition().x + rand()%32 - 16,weaponSprite.getPosition().y + rand()%20 - 10,0,0,true);
			}
		}
	}

	weaponSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
}

void Bomb::close()
{
	isClosed = true;

	if(currentFuseLoopSound != nullptr)
	{
		currentFuseLoopSound->mainSound.stop();
				currentFuseLoopSound = nullptr;
	}
}

Bomb::~Bomb(void)
{
	
}
