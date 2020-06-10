#pragma once

#include <SFML/Graphics.hpp>

#include "animation.h"


class Blink_Ani : public Animation
{
public:
	Blink_Ani(sf::Sprite &aniSprite, sf::Color blinkColor, sf::Color delColor, unsigned short blinkRepeat, int ms_waitTime);

	void animate();

	~Blink_Ani(void);
private:
	sf::Clock blinkClock;
	const int ms_BlinkTime;

	int blinkRepeat;
	int repeat;
	bool mode;

	sf::Color blinkColor;
	sf::Color lastColor;

	sf::Sprite *aniSprite;
};

