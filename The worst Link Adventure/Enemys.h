#pragma once

#include "Enemy.h"

#include "Sound_Manager.h"

class Snake : public Enemy
{
public:
	Snake(int mapPosX,int mapPosY);

	void update();
	void interact(interactType interact_Type);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Snake(void);
private:

	sf::Clock updateClock;
	sf::Clock moveClock;

	bool moveToTarget;

	bool step;
	short dir;

	int ms_moveTime;

	int attackTime;
};

class Laser_Tower : public Enemy
{
public:
	Laser_Tower(int mapPosX,int mapPosY,std::string settings);

	void update();
	void interact(interactType interact_Type);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Laser_Tower(void);
private:
	bool left;
	bool right;
	bool up;
	bool down;

	sf::Clock waitTimer;
	sf::Clock shotTimer;

	bool shot;

	int shot_duration_ms;
	int shot_wait_ms;
};

class Jumper : public Enemy
{
public:
	Jumper(int mapPosX,int mapPosY,std::string settings);

	void update();
	void interact(interactType interact_Type);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Jumper(void);
private:
	void jump(int mapPosX, int mapPosY);

	sf::Clock updateClock;
	sf::Clock jumpClock;

	sf::Clock aniClock;
	bool aniMode;

	bool isInJump;
	int ms_JumpTime;

	int attackRadius;
	
	int jumpPosX;
	int jumpPosY;

	int jumpHeight;

	short type;
};

class Slime : public Enemy
{
public:
	Slime(int mapPosX,int mapPosY,std::string settings);

	void update();
	void interact(interactType interact_Type);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Slime(void);
private:
	void jump(int mapPosX, int mapPosY);

	sf::Clock updateClock;
	
	//Blizeln
	sf::Clock blinkClock;
	int ms_blinkWaitTime;

	//Jump
	sf::Clock jumpClock;
	short jumpState;
	int ms_JumpTime;

	int jumpPosX;
	int jumpPosY;

	int jumpHeight;

	//Anriffsradius
	int attackRadius;
	
	//Schlime-Type
	short type;
	sf::Color slimeColor;
};

class Spike : public Enemy
{
public:
	Spike(int mapPosX,int mapPosY,std::string settings);

	void update();
	void interact(interactType interact_Type);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Spike(void);
private:
	sf::Clock updateClock;
	sf::Clock aniClock;
	
	short mode;
	int updateTime;
	short currentDir;
	bool backwards;

	bool aniState;
};

class Ghost : public Enemy
{
public:
	Ghost(int mapPosX,int mapPosY);

	void update();
	void interact(interactType interact_Type);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Ghost(void);
private:
	sf::Clock updateClock;
	sf::Clock moneyClock;
	sf::Clock checkClock;
	
	void calculateMoveSpeeds();

	float speed;
	float speedX;
	float speedY;

	bool flyToPlayer;
};

//******BOSSES*******//
class Boss_Fly : public Enemy
{
public:
	Boss_Fly(int mapPosX,int mapPosY);

	void update();
	void interact(interactType interact_Type);
	void draw(sf::RenderWindow& mainWindow);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Boss_Fly(void);
private:
	sf::Clock updateClock;
	
	//Flügelschlag
	sf::Clock wingsClock;
	
	//Mini Fliegen
	struct miniFly
	{
		int posX;
		int posY;
		bool arrowDrop;
	};

	std::list<miniFly> miniFlys;
	void spawnNewMiniFly(int tile_posX, int tile_posY);
	void updateMiniFlys();
	short arrowDropAmount;

	sf::Clock miniFlySpawnClock;
	sf::Sprite spr_miniFly;

	//Mode
	short bossMode;
	sf::Clock bossClock;
	bool flyMode;

};

//Gandalf
class Gandalf : public Enemy
{
public:
	Gandalf(int mapPosX,int mapPosY);

	void update();
	void interact(interactType interact_Type);
	void draw(sf::RenderWindow& mainWindow);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Gandalf(void);
private:
	sf::Clock updateClock;
	sf::Clock laserClock;
	
	void calculateNewPosition();
	void tpToFixPoint(short dir);

	int explosionCounter;

	short lookDir;

	sf::Vector2i lastPlayerPos;

	sf::Sprite starSprite;

	//Mode
	short bossMode;
	sf::Clock waitClock;
	short magicStatus;
};

//Pyramide
class Pyramide : public Enemy
{
public:
	Pyramide(int mapPosX,int mapPosY);

	void update();
	void interact(interactType interact_Type);
	void draw(sf::RenderWindow& mainWindow);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Pyramide(void);
private:
	//Private Functions
	void moveHead();
	void calculateFireball();

	short currentDir;
	short explosionCounter;

	//Boss
	struct bodyTile
	{
		sf::Vector2i mapPos;
		std::list<sf::Vector2i> lastMovements;
	};

	//body
	int bodyLength;
	std::list<bodyTile> bodyList;
	sf::Sprite spr_body;

	//Fireball
	struct fireball
	{
		sf::Vector2f spawnPos;
		sf::Vector2f mapPos;
		sf::Vector2f flyMapPos;
	};

	std::list<fireball> fireballList;
	
	short fireballAmount;
	bool fireState;

	sf::Sprite spr_fireball;
	sf::Sprite spr_crosshair;

	//Mode
	short bossMode;
	sf::Clock waitClock;
	sf::Clock updateClock;
	sf::Clock fireballClock;
};

//Valac
class Valac : public Enemy
{
public:
	Valac(int mapPosX,int mapPosY);

	void update();
	void interact(interactType interact_Type);
	void draw(sf::RenderWindow& mainWindow);

	void hurt(float damage);
	void kill(std::vector<int> dropItems,unsigned short itemDropAmount);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Valac(void);
private:
	//Copys
	short realPos;
	
	sf::Vector2i valacCopyPos1;
	sf::Vector2i valacCopyPos2;
	sf::Vector2i valacCopyPos3;
	
	void spawnCopys();
	void updateCopys();

	sf::Sprite valacCopy;

	//Energywave
	struct energyWave
	{
		sf::Vector2i mapPos;
		sf::CircleShape wave;
		short waveMode;
		bool hitTarget;
	};

	std::list<energyWave*> energyWaves;
	short energyWaveStatus;

	void spawnEnergyWave(int mapX, int mapY, short waveMode, sf::Color waveColor);
	void updateEnegryWaves();

	//Mode
	short bossMode;
	bool moveAniState;

	sf::Clock waitClock;
	sf::Clock updateClock;
	sf::Clock aniClock;

	//Shield
	sf::CircleShape shield;
	bool showShield;

	//Sound
	Sound_Manager::sound *MagicLoopSound;
};