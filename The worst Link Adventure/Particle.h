#pragma once

#include <SFML/Graphics.hpp>

#include "Defines.h"

class Particle
{
public:
	Particle(float xSpeed, float ySpeed, bool rotate,bool relativToMap, unsigned int ms_showTime, unsigned int ms_delayTime);

	virtual void draw(sf::RenderWindow &mainWindow) = 0;
	virtual void update() = 0;

	bool isParticleComplete();
	void deleteParticle();

	virtual ~Particle(void);
protected:
	int mapPosX;
	int mapPosY;

	const float xSpeed;
	const float ySpeed;

	const unsigned int ms_showTime;
	sf::Clock showClock;

	const unsigned int ms_delayTime;
	sf::Clock delayClock;

	bool rotate;
	bool relativToMap;

	bool isComplete;
};

