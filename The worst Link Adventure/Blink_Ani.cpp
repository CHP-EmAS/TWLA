#include "Blink_Ani.h"

Blink_Ani::Blink_Ani(sf::Sprite &aniSprite, sf::Color blinkColor, sf::Color delColor, unsigned short blinkRepeat, int ms_waitTime)
: ms_BlinkTime(ms_waitTime)
{
	this->aniSprite = &aniSprite;
	this->blinkColor = blinkColor;
	this-> blinkRepeat = blinkRepeat;

	lastColor = delColor;
	aniSprite.setColor(blinkColor);

	repeat = 0;

	isComplete = false;
	mode = 1;
}

void Blink_Ani::animate()
{
	if(isComplete) return;

	if(blinkClock.getElapsedTime().asMilliseconds() > ms_BlinkTime)
	{
		if(mode == 0)
		{
			if(aniSprite->getColor() != blinkColor)
			{
				if(aniSprite->getColor() != lastColor) lastColor = aniSprite->getColor();

				aniSprite->setColor(blinkColor);
			}

			mode = 1;
		}
		else
		{
			if(aniSprite->getColor() != lastColor)
			{
				aniSprite->setColor(lastColor);
			}

			repeat++;

			mode = 0;
		}

		blinkClock.restart();
	}

	if(repeat >= blinkRepeat)
		isComplete = true;
}

Blink_Ani::~Blink_Ani(void)
{

}
