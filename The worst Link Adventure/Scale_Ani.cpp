#include "Scale_Ani.h"

Scale_Ani::Scale_Ani(sf::Sprite &aniSprite, unsigned int zoomPercent, int ms_soomWaitTime)
: ms_soomWaitTime(ms_soomWaitTime)
{
	isComplete = false;

	this->aniSprite = &aniSprite;
	this->zoomPercent = zoomPercent;
	
	neededScaleX = float(aniSprite.getScale().x) / float(100) * float(zoomPercent);
	neededScaleY = float(aniSprite.getScale().y) / float(100) * float(zoomPercent);

	aniClock.restart();
}

void Scale_Ani::animate()
{
	if(aniClock.getElapsedTime().asMilliseconds() >= ms_soomWaitTime)
	{
		//Scale X
		if(aniSprite->getScale().x < neededScaleX)
			aniSprite->setScale(aniSprite->getScale().x + 0.01f,aniSprite->getScale().y);
		else if(aniSprite->getScale().x > neededScaleX)
			aniSprite->setScale(aniSprite->getScale().x - 0.01f,aniSprite->getScale().y);
		//Scale Y
		if(aniSprite->getScale().y < neededScaleY)
			aniSprite->setScale(aniSprite->getScale().x,aniSprite->getScale().y + 0.01f);
		else if(aniSprite->getScale().y > neededScaleY)
			aniSprite->setScale(aniSprite->getScale().x,aniSprite->getScale().y - 0.01f);

		//Überprüfen
		if(aniSprite->getScale().x - neededScaleX > -0.05 && aniSprite->getScale().x - neededScaleX < 0.05)
		{
			if(aniSprite->getScale().y - neededScaleY > -0.05 && aniSprite->getScale().y - neededScaleY < 0.05)
			{
				aniSprite->setScale(neededScaleX,neededScaleY);
				isComplete = true;
			}
		}

		aniClock.restart();
	}
}

Scale_Ani::~Scale_Ani(void)
{

}
