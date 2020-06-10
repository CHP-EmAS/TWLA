#pragma once

#include <list>
#include <map>

#include "Defines.h"

#include "NPC.h"
#include "Enemy.h"

class NPC_Manager : public Singleton<NPC_Manager>
{

friend class Singleton<NPC_Manager>;

public:

	void update_Enemys_NPCs();
	void draw_NPCs(sf::RenderWindow& mainWindow);
	void draw_Enemys(sf::RenderWindow& mainWindow);

	void delete_Enemys_NPCs();

	NPC* createNPC(short onMapID,short npcID,int spawnPosX,int spawnPosY);
	Enemy* createEnemy(short onMapID,Enemy::EnemyType enemyType,int spawnPosX,int spawnPosY,std::string enemySettings);

	NPC* getNPC(collisionbox* npcCollisionbox);
	NPC* getNPC(short onMapID);

	Enemy* getEnemy(collisionbox* enemyCollisionbox);
	Enemy* getEnemy(short onMapID);

	const std::list<collisionbox*> &get_Activ_Collisionboxes();

	void removeActivNPC(NPC* delNPC);
	void addActivNPC(NPC* newNPC);

	void removeActivEnemy(Enemy* delEnemy);
	void addActivEnemy(Enemy* newEnemy);

	std::string getNPCSaveData();

	bool loadNPCSaveData(std::string data);
	bool loadEnemySaveData(std::string data);

	~NPC_Manager(void);
private:
	NPC_Manager(void);

	std::map<int,NPC*> npcList;
	std::list<NPC*> activNPCList;

	std::map<int,Enemy*> enemyList;
	std::list<Enemy*> activEnemyList;

	std::list<collisionbox*> allCollisionboxes;

	NPC* errorNPC;
	Enemy* errorEnemy;

	std::map<int,std::string> npc_Paths;
};

