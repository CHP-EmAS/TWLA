#include "Enemy.h"

#include "Map_Manager.h"
#include "NPC_Manager.h"
#include "Animation_Manager.h"
#include "Quest_Manager.h"

Enemy::Enemy(int mapPosX,int mapPosY)
{
	NEWOUT("Neuer Enemy wurde erstellt! (" << this << ")")

	this->spawnPosX = mapPosX;
	this->spawnPosY = mapPosY;

	this->mapPosX = spawnPosX;
	this->mapPosY = spawnPosY;

	m_collisionbox.mapPosX = spawnPosX;
	m_collisionbox.mapPosY = spawnPosY;
	m_collisionbox.boundNPC = nullptr;
	m_collisionbox.boundObj = nullptr;
	m_collisionbox.boundEnemy = this;
	m_collisionbox.height = ZOOMSIZE;
	m_collisionbox.width = ZOOMSIZE;
	m_collisionbox.type = Enemy_Collisionbox;

	enemySprite.setPosition(0,0);
	enemySprite.setScale(SCALE,SCALE);

	active = false;

	canCollide = true;

	damage = 0;

	live = 10;
	fullLive = live;

	moveRadius = 0;
	attackRadius = 100;
	isAttacking = false;

	type = Snake;

	name = "None";

	showLiveBar = true;
	updateAlways = false;

	liveBar.setSize(sf::Vector2f(32,5));
	liveBar.setFillColor(sf::Color::Green);
	
	liveBarFrame.setSize(liveBar.getSize());
	liveBarFrame.setFillColor(sf::Color(100,100,100,100));
};

void Enemy::draw(sf::RenderWindow &mainWindow)
{
	if(active)
		mainWindow.draw(enemySprite);

	if(Map_Manager::getMe().isDrawingFrames())
	{
		sf::RectangleShape box;

		box.setFillColor(sf::Color(255,200,0,150));
		box.setPosition(Map_Manager::getMe().getMapPosX() + m_collisionbox.mapPosX,Map_Manager::getMe().getMapPosY() + m_collisionbox.mapPosY);
		box.setSize(sf::Vector2f(m_collisionbox.width,m_collisionbox.height));

		mainWindow.draw(box);
	}

	if(showLiveBar)
	{
		mainWindow.draw(liveBarFrame);
		mainWindow.draw(liveBar);
	}
}

void Enemy::update()
{
	if(!init)
	{
		mapPosX = spawnPosX;
		mapPosY = spawnPosY;

		m_collisionbox.mapPosX = spawnPosX + cBoxPosX;
		m_collisionbox.mapPosY = spawnPosY + cBoxPosY;

		update_Collisionbox_List();
		init = true;
	}

	enemySprite.setPosition(Map_Manager::getMe().getMapPosX() + mapPosX,Map_Manager::getMe().getMapPosY() + mapPosY);

	if(showLiveBar)
	{
		liveBarFrame.setPosition(enemySprite.getPosition().x + enemySprite.getGlobalBounds().width/2- liveBarFrame.getSize().x/2,enemySprite.getPosition().y - liveBarFrame.getSize().y - 5);

		//Colorfade von Grün über Gelb zu Rot
		float livePercent = live / fullLive;

		int red;
		int green;

		if(livePercent*100.f > 50.f)
		{
			green = 255;
			red = 255 - (255 * ((live - fullLive/2) / (fullLive/2)));
		}
		else
		{
			red = 255;
			green = 255 * ((live) / (fullLive/2));
		}

		liveBar.setFillColor(sf::Color(red,green,0));

		//Positionen der Lebensanzeige setzen
		liveBar.setSize(sf::Vector2f(liveBarFrame.getSize().x *livePercent,liveBar.getSize().y));
		liveBar.setPosition(liveBarFrame.getPosition());
	}
}

void Enemy::setActiv(bool activ)
{
	if(activ)
	{
		NPC_Manager::getMe().addActivEnemy(this);
		this->active = true;
	}
	else
	{
		NPC_Manager::getMe().removeActivEnemy(this);
		this->active = false;
	}
}

collisionbox *Enemy::getCollisionbox()
{
	return &m_collisionbox;
}

sf::Vector2i Enemy::getEnemySize()
{
	sf::Vector2i size;

	size.x = enemySprite.getGlobalBounds().width;
	size.y = enemySprite.getGlobalBounds().height;

	return size;
}

bool Enemy::alwaysUpdate()
{
	return updateAlways;
}

Enemy::~Enemy(void)
{
	DELOUT("Ein Enemy wurde geloescht!\t(" << this << ")")
}
