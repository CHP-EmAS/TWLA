#include "Sprite_Particle.h"

#include "Map_Manager.h"

Sprite_Particle::Sprite_Particle(sf::Sprite &particleSprite,unsigned int ms_showTime,unsigned int ms_delayTime,int speedX, int speedY, bool relativToMap, bool rotate)
: Particle(speedX,speedY,rotate,relativToMap,ms_showTime,ms_delayTime),
particleSprite(particleSprite)
{
	if(relativToMap)
	{
		this->mapPosX = particleSprite.getPosition().x - Map_Manager::getMe().getMapPosX();
		this->mapPosY = particleSprite.getPosition().y - Map_Manager::getMe().getMapPosY();
	}

	isComplete = false;
}

void Sprite_Particle::draw(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(particleSprite);
}

void Sprite_Particle::update()
{
	if(delayClock.getElapsedTime().asMilliseconds() > ms_delayTime)
	{
		if(relativToMap)
		{
			mapPosX += xSpeed;
			mapPosY += ySpeed;
		}
		else
		{
			particleSprite.setPosition(particleSprite.getPosition().x+xSpeed,particleSprite.getPosition().y+ySpeed);
		}

		if(rotate)
		{
			if(particleSprite.getRotation() + 5 > 360)
				particleSprite.setRotation(0);
			else
				particleSprite.setRotation(particleSprite.getRotation() + 5);
		}

		delayClock.restart();
	}

	if(showClock.getElapsedTime().asMilliseconds() > ms_showTime)
	{
		isComplete = true;
	}

	if(relativToMap)
	{
		particleSprite.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	}
}

Sprite_Particle::~Sprite_Particle(void)
{
}
