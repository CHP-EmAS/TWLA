#include "Particle.h"

Particle::Particle(float xSpeed, float ySpeed, bool rotate,bool relativToMap, unsigned int ms_showTime, unsigned int ms_delayTime)
:xSpeed(xSpeed),
ySpeed(ySpeed),
ms_delayTime(ms_delayTime),
ms_showTime(ms_showTime)
{
	this->rotate = rotate;
	this->relativToMap = relativToMap;
	showClock.restart();
}

bool Particle::isParticleComplete()
{
	return isComplete;
}

void Particle::deleteParticle()
{
	isComplete = true;
}

Particle::~Particle(void)
{

}
