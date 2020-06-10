#pragma once
#include "particle.h"
class Sprite_Particle : public Particle
{
public:
	Sprite_Particle(sf::Sprite &particleSprite,unsigned int ms_showTime,unsigned int ms_delayTime,int speedX = 0, int speedY = 0, bool relativToMap = false, bool rotate = false);

	void draw(sf::RenderWindow &mainWindow);
	void update();

	~Sprite_Particle(void);
private:
	sf::Sprite particleSprite;
};

