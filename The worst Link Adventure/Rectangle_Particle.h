#pragma once
#include "particle.h"
class Rectangle_Particle : public Particle
{
public:
	Rectangle_Particle(int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX, int speedY,bool relativToMap,bool rotate);

	void draw(sf::RenderWindow &mainWindow);
	void update();

	~Rectangle_Particle(void);
private:
	sf::RectangleShape shape;
};

