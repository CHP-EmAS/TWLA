#pragma once
#include "SFML/Graphics.hpp"
#include <list>

#include "Defines.h"

#include "Particle.h"

class Particle_Manager : public Singleton<Particle_Manager>
{

friend class Singleton<Particle_Manager>;

public:
	
	struct itemParticle
	{
		int mapPosX;
		int mapPosY;

		int itemNumber;
		int scale;

		int despawnTime_Sec;

		sf::Sprite itemParticleSprite;
		sf::Clock despawnClock;

		sf::Clock aniClock;
		short aniTick;
		bool aniMode;
	};

	enum ParticleType
	{
		Circle,
		Rectangle,
		Text
	};
	
	Particle *addParticle(ParticleType type,int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX = 0, int speedY = 0, bool relativToMap = true, bool rotate = false);
	Particle *addParticle(sf::Sprite particleSprite,unsigned int ms_showTime,unsigned int ms_delayTime,int speedX = 0, int speedY = 0, bool relativToMap = true, bool rotate = false);

	bool spawnItemParticle(int itemNumber,int scale,int mapPosX,int mapPosY,int despawnTime_Sec = 20);
	int isCollidingWithItemParticle(collisionbox* box);

	void drawParticle(sf::RenderWindow &mainWindow);
	void updateParticle();

	void setMaxParticleAmount(unsigned int amount);

	void clearParticleList();
	int getActivParticleAmount();

	~Particle_Manager(void);
private:
	Particle_Manager(void);
	
	std::list<Particle*> particleList;
	std::list<itemParticle*> itemParticleList;

	unsigned int maxParticleAmount;
};

