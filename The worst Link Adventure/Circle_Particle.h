#pragma once

#include "Particle.h"

class Circle_Particle : public Particle
{
public:
	Circle_Particle(int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX, int speedY,bool relativToMap,bool rotate);

	void draw(sf::RenderWindow &mainWindow);
	void update();

	~Circle_Particle(void);
private:
	sf::CircleShape shape;
};

