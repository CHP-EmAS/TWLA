#pragma once

#include <SFML/Graphics.hpp>

#include "animation.h"
#include "Particle_Manager.h"

class Explosion_Ani : public Animation
{
public:
	Explosion_Ani(sf::Sprite &aniSprite,unsigned int maxSpeed,Particle_Manager::ParticleType shape,int ms_delayTime);

	void animate();

	~Explosion_Ani(void);

private:
	sf::Clock delayClock;
	const int ms_delayTime;

	unsigned int maxSpeed;

	sf::Sprite *aniSprite;

	Particle_Manager::ParticleType shape;
};

