#pragma once

#include "Weapon.h"
#include "Weapon_Manager.h"
#include "Map_Manager.h"
#include "Sound_Manager.h"

namespace Weapons
{
	//Bombe
	class Bomb : public Weapon
	{
	public:
		Bomb(Weapon_Manager::weapon bomb,unsigned int sec_explosionTime,unsigned int explosionRadius, float damage,int mapPosX,int mapPosY);
	
		void update();

		void close();
	
		~Bomb(void);
	private:
		unsigned int explosionRadius;
		
		sf::Clock explosionsTimeClock;
		unsigned int sec_explosionTime;

		bool canExplode;

		Sound_Manager::sound *currentFuseLoopSound;
	};

	//Schwert
	class Sword : public Weapon
	{
	public:
		Sword(Weapon_Manager::weapon sword, short dir, float damage,int mapPosX,int mapPosY);
	
		void update();

		void close();

		void setIsFinishedVariable(bool *boolVar);
		void setDamage(float damage);
	
		~Sword(void);
	private:
		short sword;
	
		int handX;
		int handY;
	
		int hitRange;
		int ticker;
	
		bool *isFinished;

		sf::Clock hitAniClock;
	};

	//Pfeile
	class Bow_Arrow : public Weapon
	{
	public:
		Bow_Arrow(Weapon_Manager::weapon arrow,unsigned int speed, short dir, float damage,int mapPosX,int mapPosY);
	
		void update();
	
		~Bow_Arrow(void);
	private:
		short arrowType;

		int speedX;
		int speedY;

		int speed;

		sf::Clock flyClock;

		unsigned int coveredTrack;

		std::list<collisionbox*> collisionlist;

		bool hasTargetHit;
	};

	//Boomerang
	class Boomerang : public Weapon
	{
	public:
		Boomerang(float speed,short dir,float damage,int spawnPosX,int spawnPosY);

		void update();

		void close();
		
		void setBoundObj(collisionbox *boundObjCBox);
		void setIsArrivedVariable(bool *boolVar);

		void clearIsArrivedVariable();

		~Boomerang();
	private:
		float speedX;
		float speedY;

		float speed;

		bool comeBack;
		bool *isArrived;

		sf::Clock flyClock;
		sf::Clock damageClock;

		float coveredTrack;

		Sound_Manager::sound *currentFlyLoopSound;

		collisionbox *boundObjCBox;
	};

	//Enterhaken
	class Grapplinghook : public Weapon
	{
	public:
		Grapplinghook(float speed,int maxLength,short dir,float damage,int spawnPosX,int spawnPosY);

		void update();
		void draw(sf::RenderWindow &mainWindow);

		void close();

		void setBoundObjPositions(float *posX,float *posY);
		void setIsArrivedVariable(bool *boolVar);

		~Grapplinghook();
	private:
		sf::Vector2f startPosition;

		float speedX;
		float speedY;

		float speed;

		bool comeBack;
		bool targetHit;

		bool *isArrived;

		sf::Clock flyClock;
		float coveredTrack;

		float maxLength;

		Sound_Manager::sound *currentFlySound;
		std::list<collisionbox*> collisionlist;

		float *boundObjPositionX;
		float *boundObjPositionY;

		sf::Sprite beginnSprite;
		sf::RectangleShape chainRect;

		Sound_Manager::sound *currentFlyLoopSound;
	};

	//Arrow Bomb
	class ArrowBomb : public Weapon
	{
	public:
		ArrowBomb(unsigned int speed, short dir, float damage,int mapPosX,int mapPosY);

		void update();
		void explode();

		~ArrowBomb();
	private:
		int speedX;
		int speedY;

		int speed;

		sf::Clock flyClock;

		unsigned int coveredTrack;

		std::list<collisionbox*> collisionlist;
	};

	//Laser
	class Laser : public Weapon
	{
	public:
		Laser(int mapPosX_beginn,int mapPosY_beginn,short dir,float damage,int duration_ms);
	
		void update();
		void draw(sf::RenderWindow &mainWindow);

		void close();
	
		void setShotDuration(int duration_ms);

		~Laser(void);
	private:
		int mapPosX_beginn;
		int mapPosY_beginn;
		int mapPosX_end;
		int mapPosY_end;

		short dir;

		sf::Clock damageClock;

		sf::Clock durationClock;
		int duration_ms;

		sf::VertexArray laserLine;
	};

	//Magic
	class Magic : public Weapon
	{
	public:
		Magic(float speed,short dir,float damage,int spawnPosX,int spawnPosY);

		void update();
		
		void setBoundObj(collisionbox *boundObjCBox);
		void setTargetObj(collisionbox *targetObjCBox);

		void setStatusVariable(short *status);

		~Magic();
	private:
		float speedX;
		float speedY;

		float speed;
		float coveredTrack;

		short *status;

		sf::Clock flyClock;
		sf::Clock damageClock;
		sf::Clock spawnStarClock;

		sf::Vector2i lastStarPos;
		sf::Sprite starSprite;

		collisionbox *boundObjCBox;
		collisionbox *targetObjCBox;

		void checkCollision(collisionbox &cBox);
	};
}

