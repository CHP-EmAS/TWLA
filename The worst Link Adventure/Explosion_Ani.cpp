#include "Explosion_Ani.h"

#include "Particle_Manager.h"


Explosion_Ani::Explosion_Ani(sf::Sprite &aniSprite,unsigned int maxSpeed,Particle_Manager::ParticleType shape, int ms_delayTime)
: ms_delayTime(ms_delayTime)
{
	this->maxSpeed = maxSpeed;
	this->aniSprite = &aniSprite;

	delayClock.restart();

	this->shape = shape;

	isComplete = false;
}

void Explosion_Ani::animate()
{
	if(isComplete) return;

	sf::Image img;
	img = aniSprite->getTexture()->copyToImage();

	//Init
	for(int x = 0 ; x < aniSprite->getTextureRect().width; x++)
	{
		for(int y = 0 ; y < aniSprite->getTextureRect().height; y++)
		{
			sf::Color PixelColor = img.getPixel(aniSprite->getTextureRect().left+x,aniSprite->getTextureRect().top+y);

			if(PixelColor.a < 5) continue;

			int posX = aniSprite->getPosition().x + x*aniSprite->getScale().x;
			int posY = aniSprite->getPosition().y + y*aniSprite->getScale().y;

			int randX = rand() % maxSpeed + 1;
			int randY = rand() % maxSpeed + 1;

			int xSpeed;
			int ySpeed;

			if(rand()%2 == 1) xSpeed = -randX;
			else xSpeed = randX;
			
			if(rand()%2 == 1) ySpeed = -randY;
			else ySpeed = randY;
	
			Particle_Manager::getMe().addParticle(shape,aniSprite->getScale().x,PixelColor,3000,20,posX,posY,xSpeed,ySpeed,true);
		}
	}

	isComplete = true;
}

Explosion_Ani::~Explosion_Ani(void)
{
}

