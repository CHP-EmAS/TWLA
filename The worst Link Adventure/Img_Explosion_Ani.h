#pragma once

#include <SFML/Graphics.hpp>

#include "animation.h"
#include "Particle.h"

class Img_Explosion_Ani : public Animation
{
public:
	Img_Explosion_Ani(int posX,int posY,unsigned int ms_explosionTime = 650,bool relativToMap = false,sf::Color explosionColor = sf::Color::White,bool smallExplosion = false);

	void animate();

	~Img_Explosion_Ani(void);

private:
	sf::Clock explodeClock;
	const int ms_explosionTime;

	int mapPosX;
	int mapPosY;

	bool relativToMap;
	bool smallExplosion;

	short mode;

	sf::Sprite explosionSprite;
	Particle *aniParticle;
};

