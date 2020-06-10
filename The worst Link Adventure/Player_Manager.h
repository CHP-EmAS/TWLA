#pragma once

#include "SFML/Graphics.hpp"

#include <string>
#include <list>

#include "Defines.h"
#include "HUD.h"
#include "InGame_Stats.h"
#include "Inventory_Manager.h"

class Player_Manager : public Singleton<Player_Manager>
{

friend class Singleton<Player_Manager>;

public:

	struct playerStats
	{
		float maxLive;
		float live;

		int maxEnergy;
		int energy;

		int armor;

		float damage;

		int moveSpeed;
		int swimmSpeed;
	};

	bool loadPlayerSaveData(std::string data);
	std::string getPlayerSaveData();

	void update();
	void updatePlayerTile();

	void updatePlayerStats();

	void updatePlayerHitBoxes();
	void clearPlayerHitboxes();

	void updateAnimations();

	void drawPlayer(sf::RenderWindow &mainWindow);
	
	void doItemFoundAni(int itemNumber, bool oneHand = false);
	void doWeapon(Inventory_Manager::slotID weaponSlot);

	void doDieAni(sf::RenderWindow &mainWindow);
	void doKickAni(int ms_duration);
	void doSwordAni();
	void doBowAni();
	void doHammerAni();
	void doHookAni();
	void doFallAni(int tileX,int tileY);
	void doMagicwandAni();
	void doSleepAni();
	void doBarrierAni();

	void doEndAnimationMovie();

	void hurt(float damage,bool resetNoDmg = true);
	void heal(float live);
	void give(int itemNumber,unsigned int amount);

	void move(int dir);
	sf::Vector2f getLastPos();
	sf::Vector2f getLastMapPos();

	void tp(int x,int y);
	void focusPlayer();
	void toggleGodmode();
	void resetItemVariables();

	void checkMoveKeys();
	void ckeckFightKeys();
	void checkDebugKeys();
	void checkPlayKeys();

	void checkEnvironment();
	void checkPlayerMoveBox();
	void checkItemPaticleCollision();

	void tryToTeleportOutOfHitBox();

	float getPosX();
	float getPosY();

	float getMapPosX();
	float getMapPosY();

	short getLookDir();

	playerStats getPlayerStats();
	bool isBarrierOn();

	HUD &getGameHUD();
	InGame_Stats &getGameStats();

	collisionbox* getCollisionbox();

	void setPos(int x, int y);
	void setPlayerColor(sf::Color color);
	void setPlayerCanMove(bool canMove);

	sf::Sprite &getPlayerSprite();

	~Player_Manager(void);
private:
	Player_Manager(void);
	
	//Stats
	playerStats stats;
	
	//HUD / Stats
	HUD *game_HUD;
	InGame_Stats *gameStats;

	//Moving 
	bool canMove; //kann er sich bewegen

	unsigned short dir; //In welche Richtung schaut Link
	unsigned short moving_Mode; //wie bewegt er sich Laufend,Schwimmend...
	unsigned short dirAnimation; //Welchen Schritt tut Link

	//Last Moving-State for update or no-update
	short lastDir;
	short lastMovingMode;
	short lastDirAnimation;
	short lastShield;

	sf::Vector2f lastPlayerMove;
	sf::Vector2f lastTpPos;

	float posX;
	float posY;

	//HitBox range
	float lastUpdatePositionX;
	float lastUpdatePositionY;

	collisionbox playerCollisionbox;
	std::list<collisionbox*> nearPlayerCollisionBoxes;

	//move
	float movespeed;
	bool godmode;

	int moveAniTime;
	sf::Clock moveAniClock;

	int moveSpeedTime;
	sf::Clock moveSpeedClock;

	sf::Clock interactKeyClock;

	//swimm
	const int swimmAniTime;
	sf::Clock  swimmAniClock;

	//Animations
	short currentAnimation;
	sf::Clock aniClock;
	int current_msAniTime;
	sf::IntRect lastLinkAniRect;
	sf::Sprite aniSprite;
	bool drawAniSprite;
	bool drawAniBeforPlayer;
	int aniMode;
	sf::Vector2f lastAniPos;

	sf::Clock weaponHitWaiter;

	//fight
	bool swordKeyPressed;
	bool weaponKeyPressed;
	sf::Clock hitWaiterClock;

	//found
	bool foundItemAni;
	sf::Sprite itemSprite;

	//damage
	sf::Color dmgColor;
	const int noDmgTime;
	sf::Clock noDmgClock;

	bool hasBoomerang;
	bool hasGrapplibghook;
	bool isSwordAniFinished;

	sf::CircleShape barrier;
	bool showBarrier;
	sf::Clock barrierClock;

	//Sleep Timer/Varibels
	int sleepTimeSec;
	sf::Clock sleepClock;
	sf::Text sleepText;
	short sleepSize;

	//Sprite/Texture
	sf::Sprite spr_Player;
	sf::Texture tex_Player;

	//energy
	sf::Clock energyFillClock;
	int ms_energyFillTime;
	sf::Clock enegryEmptyClock;
	int ms_energyEmptyTime;

	//ending
	sf::Sprite spr_heart;
	short heartCounter;
};

