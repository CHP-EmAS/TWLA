#pragma once

#include <SFML/Graphics.hpp>

#include "Animation.h"

class Scale_Ani : public Animation
{
public:
	Scale_Ani(sf::Sprite &aniSprite, unsigned int zoomPercent, int ms_soomWaitTime);

	void animate();

	~Scale_Ani(void);
private:
	sf::Clock aniClock;
	const int ms_soomWaitTime;

	unsigned int zoomPercent;

	float neededScaleX;
	float neededScaleY;

	sf::Sprite *aniSprite;
};

