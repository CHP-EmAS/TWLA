#include "Img_Explosion_Ani.h"

#include "Map_Manager.h"
#include "Graphic_Manager.h"
#include "Particle_Manager.h"
#include "Sound_Manager.h"

Img_Explosion_Ani::Img_Explosion_Ani(int posX,int posY,unsigned int ms_explosionTime,bool relativToMap,sf::Color explosionColor, bool smallExplosion)
	:ms_explosionTime(ms_explosionTime)
{
	this->relativToMap = relativToMap;
	this->smallExplosion = smallExplosion;

	if(relativToMap)
	{
		this->mapPosX = posX - Map_Manager::getMe().getMapPosX();
		this->mapPosY = posY - Map_Manager::getMe().getMapPosY();
	}

	if(!smallExplosion)
	{
		explosionSprite.setTexture(Graphic_Manager::getMe().getTexture(Graphic_Manager::Explosion));
		explosionSprite.setTextureRect(sf::IntRect(3,36,14,14));
		explosionSprite.setOrigin(7,7);
	}
	else
	{
		explosionSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
		explosionSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,800));
		explosionSprite.setOrigin(8,8);
	}

	explosionSprite.setScale(SCALE,SCALE);
	explosionSprite.setPosition(posX,posY);
	explosionSprite.setColor(explosionColor);

	aniParticle = Particle_Manager::getMe().addParticle(explosionSprite,ms_explosionTime/8,ms_explosionTime,0,0,relativToMap);

	mode = 0;
	isComplete = false;

	explodeClock.restart();
}

void Img_Explosion_Ani::animate()
{
	if(explodeClock.getElapsedTime().asMilliseconds() >= ms_explosionTime/8)
	{
		if(smallExplosion)
		{
			switch(mode)
			{
			case 0:
				explosionSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,800));
				break;
			case 1:
				explosionSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,801));
				break;
			case 2:
				explosionSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,802));
				break;
			case 3:
				isComplete = true;
				break;
			}

			aniParticle = Particle_Manager::getMe().addParticle(explosionSprite,ms_explosionTime/8,ms_explosionTime,0,0,relativToMap);
		}
		else
		{
			switch(mode)
			{
			case 0:
				explosionSprite.setTextureRect(sf::IntRect(20,25,30,30));
				explosionSprite.setOrigin(15,15);
				break;
			case 1:
				explosionSprite.setTextureRect(sf::IntRect(53,24,30,32));
				explosionSprite.setOrigin(15,16);
				break;
			case 2:
				explosionSprite.setTextureRect(sf::IntRect(86,16,44,46));
				explosionSprite.setOrigin(22,23);
				break;
			case 3:
				explosionSprite.setTextureRect(sf::IntRect(133,26,28,30));
				explosionSprite.setOrigin(14,15);
				break;
			case 4:
				explosionSprite.setTextureRect(sf::IntRect(164,20,39,39));
				explosionSprite.setOrigin(19,19);
				break;
			case 8:
				explosionSprite.move(0,-8);
				explosionSprite.setTextureRect(sf::IntRect(206,18,42,41));
				explosionSprite.setOrigin(21,20);
				break;
			case 12:
				explosionSprite.move(0,-8);
				explosionSprite.setTextureRect(sf::IntRect(251,34,43,38));
				explosionSprite.setOrigin(21,19);
				break;
			case 13:
				isComplete = true;
				break;
			}

			if(mode < 4)
			{
				aniParticle = Particle_Manager::getMe().addParticle(explosionSprite,ms_explosionTime/8,ms_explosionTime,0,0,relativToMap);
			}
			else if(mode == 4 || mode == 8)
			{
				aniParticle = Particle_Manager::getMe().addParticle(explosionSprite,ms_explosionTime/2,ms_explosionTime/10,0,-1,relativToMap);
			}
			else if(mode == 12)
			{
				aniParticle = Particle_Manager::getMe().addParticle(explosionSprite,ms_explosionTime/3,ms_explosionTime/13,0,-1,relativToMap);
			}
		}

		mode++;
		explodeClock.restart();
	}

	if(relativToMap)
	{
		explosionSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	}
}

Img_Explosion_Ani::~Img_Explosion_Ani(void)
{
}
