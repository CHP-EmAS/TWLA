#pragma once

#include "moveable.h"
#include "Graphic_Manager.h"

#include <list>

class Enemy : public moveable
{
public:
	Enemy(int mapPosX,int mapPosY);

	enum EnemyType
	{
		Snake,
		Laser_Tower,
		Jumper,
		Slime,
		Boss_Fly,
		Spike,
		Gandalf,
		Ghost,
		Pyramide,
		Valac
	};

	virtual void update() = 0;
	virtual void interact(interactType interact_Type) = 0;
	virtual void draw(sf::RenderWindow& mainWindow);

	void setActiv(bool activ);

	virtual void hurt(float damage) = 0;
	virtual void kill(std::vector<int> dropItems,unsigned short itemDropAmount) = 0;

	collisionbox *getCollisionbox();
	sf::Vector2i getEnemySize();
	bool alwaysUpdate();

	virtual std::string getMySaveData() = 0;
	virtual bool loadMySaveData(std::string data) = 0;

	virtual ~Enemy(void);
protected:
	float damage;

	float live;
	float fullLive;

	int moveRadius;
	int attackRadius;
	bool isAttacking;

	EnemyType type;

	std::string name;

	int spawnPosX;
	int spawnPosY;

	sf::Sprite enemySprite;

	bool active;
	bool init;
	bool updateAlways;

	bool showLiveBar;
	sf::RectangleShape liveBar;
	sf::RectangleShape liveBarFrame;
};

