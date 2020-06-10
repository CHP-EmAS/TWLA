#include "Rectangle_Particle.h"

#include "Map_Manager.h"


Rectangle_Particle::Rectangle_Particle(int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX, int speedY,bool relativToMap,bool rotate)
: Particle(speedX,speedY,rotate,relativToMap,ms_showTime,ms_delayTime)
{
	if(relativToMap)
	{
		this->mapPosX = posX - Map_Manager::getMe().getMapPosX();
		this->mapPosY = posY - Map_Manager::getMe().getMapPosY();
	}

	shape.setSize(sf::Vector2f(size,size));
	shape.setFillColor(color);
	shape.setOrigin(size/2,size/2);
	shape.setPosition(posX,posY);

	isComplete = false;
}

void Rectangle_Particle::draw(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(shape);
}

void Rectangle_Particle::update()
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
			shape.setPosition(shape.getPosition().x+xSpeed,shape.getPosition().y+ySpeed);
		}

		if(rotate)
		{
			if(shape.getRotation() + 1 > 360)
				shape.setRotation(0);
			else
				shape.setRotation(shape.getRotation() + 1);
		}

		delayClock.restart();
	}


	if(showClock.getElapsedTime().asMilliseconds() > ms_showTime)
	{
		if(shape.getFillColor().a-1 <= 0)
			isComplete = true;
		else
			shape.setFillColor(sf::Color(shape.getFillColor().r,shape.getFillColor().g,shape.getFillColor().b,shape.getFillColor().a-1));
	}

	if(relativToMap)
	{
		shape.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	}
}

Rectangle_Particle::~Rectangle_Particle(void)
{

}
