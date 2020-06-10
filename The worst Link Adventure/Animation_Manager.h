#pragma once

#include <SFML/Graphics.hpp>
#include <list>

#include "Defines.h"

#include "Animation.h"
#include "Particle_Manager.h"

class Animation_Manager : public Singleton<Animation_Manager>
{

friend class Singleton<Animation_Manager>;

public:
	//Animation by Sprite
	void doBlinkAnimation(sf::Sprite &obj,sf::Color blinkColor, sf::Color delColor = sf::Color::White,unsigned short blinkRepeat = 1,int ms_waitTime = 200);
	void doZoomAnimation(sf::Sprite &obj, unsigned int zoomPercent, int ms_soomWaitTime = 5);
	void doSpriteExplosion(sf::Sprite &obj,unsigned int maxSpeed,Particle_Manager::ParticleType type = Particle_Manager::Circle, int ms_delayTime = 20);

	//Animation by Position
	void doExplosion(int posX,int posY,unsigned int ms_speed = 650,bool relativToMap = false,sf::Color explosionColor = sf::Color::White,bool smallExplosion = false);

	//Update all running Animations
	void updateAnimations(); 

	void cancelAllAnimation();
	int getRunningAnimationAmount();

	//inTime Animation
	sf::Color colorChange(sf::Color input, int colorIndex = 1);

	~Animation_Manager(void);
private:
	Animation_Manager(void);

	std::list<Animation*> animation_list;
};

