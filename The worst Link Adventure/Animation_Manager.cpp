#include "Animation_Manager.h"

#include "Blink_Ani.h"
#include "Scale_Ani.h"
#include "Explosion_Ani.h"
#include "Img_Explosion_Ani.h"

Animation_Manager::Animation_Manager(void)
{
	NEWOUT("Neuer Animation_Manager wurde erstellt! (" << this << ")")
}

void Animation_Manager::doBlinkAnimation(sf::Sprite &obj,sf::Color blinkColor, sf::Color delColor,unsigned short blinkRepeat,int ms_waitTime)
{
	Blink_Ani *newAni = new Blink_Ani(obj,blinkColor,delColor,blinkRepeat,ms_waitTime);

	animation_list.push_back(newAni);
}

void Animation_Manager::doZoomAnimation(sf::Sprite &obj, unsigned int zoomPercent, int ms_soomWaitTime)
{
	Scale_Ani *newAni = new Scale_Ani(obj,zoomPercent,ms_soomWaitTime);

	animation_list.push_back(newAni);
}

void Animation_Manager::doSpriteExplosion(sf::Sprite &obj,unsigned int maxSpeed,Particle_Manager::ParticleType type, int ms_delayTime)
{
	Explosion_Ani *newAni = new Explosion_Ani(obj,maxSpeed,type,ms_delayTime);

	animation_list.push_back(newAni);
}

void Animation_Manager::doExplosion(int posX,int posY,unsigned int ms_speed,bool relativToMap,sf::Color explosionColor,bool smallExplosion)
{
	Img_Explosion_Ani *newAni = new Img_Explosion_Ani(posX,posY,ms_speed,relativToMap,explosionColor,smallExplosion);

	animation_list.push_back(newAni);
}

void Animation_Manager::updateAnimations()
{
	std::list<Animation*>::iterator cursor;

	for(cursor = animation_list.begin(); cursor != animation_list.end(); ++cursor)
	{
		if(!(*cursor)->isAnimationComplete())
		{
			(*cursor)->animate();
		}
		else
		{
			delete (*cursor);

			cursor = animation_list.erase(cursor);

			if(cursor == animation_list.end()) 
				break;
		}
	}
}

void Animation_Manager::cancelAllAnimation()
{
	std::list<Animation*>::iterator cursor;

	for(cursor = animation_list.begin(); cursor != animation_list.end(); ++cursor)
	{
		delete (*cursor);
	}

	animation_list.clear();
}

int Animation_Manager::getRunningAnimationAmount()
{
	return animation_list.size();
}

sf::Color Animation_Manager::colorChange(sf::Color input, int colorIndex)
{
	int r = input.r;
	int g = input.g;
	int b = input.b;
	int a = input.a;

	if(r == 255 && g < 255 && b == 0)
	{
		g += colorIndex;
		if(g > 255) g = 255;
	}
	else if(r>0 && g == 255 && b == 0)
	{
		r -= colorIndex;
		if(r < 0) r = 0;
	}
	else if(r == 0 && g == 255 && b < 255)
	{
		b += colorIndex;
		if(b > 255) b = 255;
	}
	else if(r == 0 && g > 0 && b == 255)
	{
		g -= colorIndex;
		if(g < 0) g = 0;
	}
	else if(r < 255 && g == 0 && b == 255)
	{
		r += colorIndex;
		if(r > 255) r = 255;
	}
	else if(r == 255 && g == 0 && b > 0)
	{
		b -= colorIndex;
		if(b < 0) b = 0;
	}
	else
	{
		r = 255;
		g = 0;
		b = 0;
	}

	input.a = a;
	input.r = r;
	input.g = g;
	input.b = b;

	return input;
}

Animation_Manager::~Animation_Manager(void)
{
	std::list<Animation*>::iterator cursor;

	for(cursor = animation_list.begin(); cursor != animation_list.end(); ++cursor)
	{
		delete (*cursor);
	}

	DELOUT("Animation_Manager wurde geloescht!\t(" << this << ")")
}
