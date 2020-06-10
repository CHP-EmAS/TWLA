#pragma once
#include "Particle.h"
#include "Graphic_Manager.h"

class Text_Particle : public Particle
{
public:
	Text_Particle(int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX, int speedY,bool relativToMap,bool rotate);
	
	enum TextMode
	{
		Angry,
		Color_Change,
		Smaller,
		Bigger,
		Standart
	};

	void draw(sf::RenderWindow &mainWindow);
	void update();

	void setString(std::string text);
	void setFont(Graphic_Manager::font font);
	void changeMode(TextMode mode);

	~Text_Particle(void);
private:
	sf::Text particleText;

	std::string originalText;

	TextMode mode;
	sf::Clock modeClock;
	int ms_modeSpeed;
};

