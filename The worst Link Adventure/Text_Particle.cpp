#include "Text_Particle.h"

#include "Map_Manager.h"


Text_Particle::Text_Particle(int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX, int speedY,bool relativToMap,bool rotate)
: Particle(speedX,speedY,rotate,relativToMap,ms_showTime,ms_delayTime)
{
	if(relativToMap)
	{
		this->mapPosX = posX - Map_Manager::getMe().getMapPosX();
		this->mapPosY = posY - Map_Manager::getMe().getMapPosY();
	}

	particleText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::font::Arial),size);
	particleText.setPosition(posX,posY);
	particleText.setFillColor(color);
	particleText.setOrigin(particleText.getGlobalBounds().width/2,particleText.getGlobalBounds().height/2);
	
	originalText = "";

	isComplete = false;

	mode = Standart;
	ms_modeSpeed = ms_showTime+1000;
}

void Text_Particle::draw(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(particleText);
}

void Text_Particle::update()
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
			particleText.setPosition(particleText.getPosition().x+xSpeed,particleText.getPosition().y+ySpeed);
		}

		if(rotate)
		{
			if(particleText.getRotation() + 1 > 360)
				particleText.setRotation(0);
			else
				particleText.setRotation(particleText.getRotation() + 1);
		}

		delayClock.restart();

		if(showClock.getElapsedTime().asMilliseconds() > ms_showTime)
		{
			if(particleText.getFillColor().a - 5 <= 0)
				isComplete = true;
			else
				particleText.setFillColor(sf::Color(particleText.getFillColor().r,particleText.getFillColor().g,particleText.getFillColor().b,particleText.getFillColor().a - 5));
		}
	}

	if(modeClock.getElapsedTime().asMilliseconds() >= ms_modeSpeed)
	{
		switch(mode)
		{
		case Angry:
		{
			std::string newText = "";

			for(int i = 0;i < originalText.size();i++)
			{
				newText += originalText[rand()%originalText.size()];
			}

			newText += '!';

			particleText.setString(newText);
			particleText.setOrigin(particleText.getGlobalBounds().width/2,particleText.getGlobalBounds().height/2);
			break;
		}
		case Color_Change:
		{
			int r = particleText.getFillColor().r;
			int g = particleText.getFillColor().g;
			int b = particleText.getFillColor().b;
			int a = particleText.getFillColor().a;

			if(r == 255 && g < 255 && b == 0)
			{
				g++;
			}
			else if(r>0 && g == 255 && b == 0)
			{
				r--;
			}
			else if(r == 0 && g == 255 && b < 255)
			{
				b++;
			}
			else if(r == 0 && g > 0 && b == 255)
			{
				g--;
			}
			else if(r < 255 && g == 0 && b == 255)
			{
				r++;
			}
			else if(r == 255 && g == 0 && b > 0)
			{
				b--;
			}
			else
			{
				r = 255;
				g = 0;
				b = 0;
			}

			particleText.setFillColor(sf::Color(r,g,b,a));
			
			break;
		}
		case Smaller:
			if(particleText.getScale().x > 0 && particleText.getScale().y > 0)
				particleText.setScale(particleText.getScale() - sf::Vector2f(0.02,0.02));
			break;
		case Bigger:
			particleText.setScale(particleText.getScale() + sf::Vector2f(0.02,0.02));
			break;
		}

		modeClock.restart();
	}

	if(relativToMap)
	{
		particleText.setPosition(mapPosX + Map_Manager::getMe().getMapPosX(),mapPosY + Map_Manager::getMe().getMapPosY());
	}
}

void Text_Particle::setString(std::string text)
{
	originalText = text;
 	particleText.setString(text);
	particleText.setOrigin(particleText.getGlobalBounds().width/2,particleText.getGlobalBounds().height/2);
}

void Text_Particle::setFont(Graphic_Manager::font font)
{
	particleText.setFont(Graphic_Manager::getMe().getFont(font));
	particleText.setOrigin(particleText.getGlobalBounds().width/2,particleText.getGlobalBounds().height/2);
}

void Text_Particle::changeMode(Text_Particle::TextMode mode)
{
	this->mode = mode;

	switch(mode)
	{
	case Standart:
		ms_modeSpeed = ms_showTime + 1000;
		break;
	case Angry:
		ms_modeSpeed = 200;
		break;
	case Color_Change:
		ms_modeSpeed = 5;
		break;
	case Bigger:
	case Smaller:
		ms_modeSpeed = 10;
		break;
	}
}

Text_Particle::~Text_Particle(void)
{
}
