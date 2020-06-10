#include "NPC_Manager.h"

#include "Enemys.h"
#include "Map_Manager.h"

NPC_Manager::NPC_Manager(void)
{
	NEWOUT("Neuer NPC_Manager wurde erstellet! (" << this << ")");

	errorNPC = new NPC(-8000,-8000);
	errorEnemy = new Snake(-8000,-8000);

	std::ifstream ifs;

	ifs.open("lists/NPCs/NPC_list.zData", std::ios::in);

	if (ifs.good())
	{
		char c_dummy[500];

		while(true)
		{
			ifs.getline(c_dummy,200,' ');
			int npcID = atoi(c_dummy);

			if(std::strcmp(c_dummy,"") == 0)break;

			ifs.getline(c_dummy,200,'\n');
			std::string npcPath = c_dummy;

			std::map<int,std::string>::iterator it;
			it = npc_Paths.find(npcID);

			if(it != npc_Paths.end())
			{
				FOUT("NPC-ID: " << npcID << " ist doppelt definiert!");
				WOUT("NPC " << npcID << " wird überschrieben!");
			}

			npc_Paths[npcID] = npcPath;
		}
	}
	else
	{
		FOUT("[CRITICAL] - NPC-Liste konnte nicht geöffnet werden!");
	}

	IOUT("Es wurden " << npc_Paths.size() << " NPCs gefunden!");
}

void NPC_Manager::update_Enemys_NPCs()
{
	sf::IntRect mapView = Map_Manager::getMe().getViewMapRectangle();
	mapView.left -= 32;
	mapView.top -= 32;
	mapView.height += 64;
	mapView.width += 64;

	std::list<NPC*>::iterator npcCursor;

	for(npcCursor = activNPCList.begin(); npcCursor != activNPCList.end(); ++npcCursor)
	{
		sf::IntRect npcRect;
		npcRect.left   = (*npcCursor)->getMapPosX();
		npcRect.top    = (*npcCursor)->getMapPosY();
		npcRect.width  = (*npcCursor)->getNPCSize().x;
		npcRect.height = (*npcCursor)->getNPCSize().y;

		if(Collision_System::getMe().intersection(mapView,npcRect)) 
			(*npcCursor)->update();
		
		if(activNPCList.size() == 0) break;
	}

	std::list<Enemy*>::iterator enemyCursor;

	for(enemyCursor = activEnemyList.begin(); enemyCursor != activEnemyList.end(); ++enemyCursor)
	{
		sf::IntRect enemyRect;
		enemyRect.left   = (*enemyCursor)->getMapPosX();
		enemyRect.top    = (*enemyCursor)->getMapPosY();
		enemyRect.width  = (*enemyCursor)->getEnemySize().x;
		enemyRect.height = (*enemyCursor)->getEnemySize().y;

		if(Collision_System::getMe().intersection(mapView,enemyRect) || (*enemyCursor)->alwaysUpdate()) 
			(*enemyCursor)->update();

		if(activEnemyList.size() == 0) break;
	}
}

void NPC_Manager::draw_NPCs(sf::RenderWindow& mainWindow)
{
	sf::IntRect mapView = Map_Manager::getMe().getViewMapRectangle();
	mapView.left -= 32;
	mapView.top -= 32;
	mapView.height += 64;
	mapView.width += 64;

	std::list<NPC*>::iterator npcCursor;
	for(npcCursor = activNPCList.begin(); npcCursor != activNPCList.end(); ++npcCursor)
	{
		sf::IntRect npcRect;
		npcRect.left = (*npcCursor)->getMapPosX();
		npcRect.top = (*npcCursor)->getMapPosY();
		npcRect.width = (*npcCursor)->getNPCSize().x;
		npcRect.height = (*npcCursor)->getNPCSize().y;

		if(Collision_System::getMe().intersection(mapView,npcRect)) 
			(*npcCursor)->draw(mainWindow);
	}
}

void NPC_Manager::draw_Enemys(sf::RenderWindow& mainWindow)
{
	sf::IntRect mapView = Map_Manager::getMe().getViewMapRectangle();
	mapView.left -= 32;
	mapView.top -= 32;
	mapView.height += 64;
	mapView.width += 64;

	std::list<Enemy*>::iterator enemyCursor;

	for(enemyCursor = activEnemyList.begin(); enemyCursor != activEnemyList.end(); ++enemyCursor)
	{
		sf::IntRect enemyRect;
		enemyRect.left = (*enemyCursor)->getMapPosX();
		enemyRect.top = (*enemyCursor)->getMapPosY();
		enemyRect.width = (*enemyCursor)->getEnemySize().x;
		enemyRect.height = (*enemyCursor)->getEnemySize().y;

		if(Collision_System::getMe().intersection(mapView,enemyRect) || (*enemyCursor)->alwaysUpdate()) 
			(*enemyCursor)->draw(mainWindow);
	}
}

void NPC_Manager::delete_Enemys_NPCs()
{
	std::map<int,NPC*>::iterator npcCursor;

	for(npcCursor = npcList.begin(); npcCursor != npcList.end(); ++npcCursor)
	{
		delete (*npcCursor).second;
	}


	std::map<int,Enemy*>::iterator enemyCursor;

	for(enemyCursor = enemyList.begin(); enemyCursor != enemyList.end(); ++enemyCursor)
	{
		delete (*enemyCursor).second;
	}

	
	npcList.clear();
	activNPCList.clear();

	enemyList.clear();
	activEnemyList.clear();

	allCollisionboxes.clear();
}

NPC* NPC_Manager::createNPC(short onMapID,short npcID,int spawnPosX,int spawnPosY)
{
	//Suche NPC ID
	std::map<int,std::string>::iterator it;
	it = npc_Paths.find(npcID);

	if(it == npc_Paths.end())
	{
		FOUT("NPC: " << npcID << " ist nicht in der NPC-Liste vorhanden!");
		return errorNPC;
	}

	NPC *newNPC = new NPC(spawnPosX,spawnPosY);

	std::string path = "lists/NPCs/";
	path += it->second;

	newNPC->loadNPC(path);

	if(npcList.find(onMapID) != npcList.end())
	{
		FOUT("onMap NPC-ID: " << onMapID << " ist doppelt definiert!");
		WOUT("NPC " << onMapID << " wird überschrieben!");
	}

	npcList[onMapID] = newNPC;

	return newNPC;
}

Enemy* NPC_Manager::createEnemy(short onMapID,Enemy::EnemyType enemyType,int spawnPosX,int spawnPosY,std::string enemySettings)
{
	Enemy *newEnemy;

	switch(enemyType)
	{
	case Enemy::Snake:
		newEnemy = new Snake(spawnPosX,spawnPosY);
		break;
	case Enemy::Laser_Tower:
		newEnemy = new Laser_Tower(spawnPosX,spawnPosY,enemySettings);
		break;
	case Enemy::Jumper:
		newEnemy = new Jumper(spawnPosX,spawnPosY,enemySettings);
		break;
	case Enemy::Slime:
		newEnemy = new Slime(spawnPosX,spawnPosY,enemySettings);
		break;
	case Enemy::Boss_Fly:
		newEnemy = new Boss_Fly(spawnPosX,spawnPosY);
		break;
	case Enemy::Spike:
		newEnemy = new Spike(spawnPosX,spawnPosY,enemySettings);
		break;
	case Enemy::Gandalf:
		newEnemy = new Gandalf(spawnPosX,spawnPosY);
		break;
	case Enemy::Ghost:
		newEnemy = new Ghost(spawnPosX,spawnPosY);
		break;
	case Enemy::Pyramide:
		newEnemy = new Pyramide(spawnPosX,spawnPosY);
		break;
	case Enemy::Valac:
		newEnemy = new Valac(spawnPosX,spawnPosY);
		break;
	default:
		FOUT("EnemyType <" << enemyType << "> ist kein bekannter EnemyType!")
		newEnemy = new Snake(spawnPosX,spawnPosY);
		break;
	};

	if(enemyList.find(onMapID) != enemyList.end())
	{
		FOUT("onMap Enemy-ID: " << onMapID << " ist doppelt definiert!");
		WOUT("Enemy " << onMapID << " wird überschrieben!");
	}

	enemyList[onMapID] = newEnemy;

	return newEnemy;
}

NPC* NPC_Manager::getNPC(collisionbox* npcCollisionbox)
{
	int mapPosX = Map_Manager::getMe().getMapPosX();
	int mapPosY = Map_Manager::getMe().getMapPosY();

	for(auto cursor:npcList)
	{
		if(npcCollisionbox == cursor.second->getCollisionbox())
		{
			return cursor.second;
		}
	}

	FOUT("NPC-Collisionbox <" << npcCollisionbox << "> konnte keinem NPC zugeordnet werden!")

	return errorNPC;
}

NPC* NPC_Manager::getNPC(short onMapID)
{
	if(npcList.find(int(onMapID)) != npcList.end()) return npcList[onMapID];
	
	FOUT("NPC-ID <" << onMapID << "> konnte keinem NPC zugeordnet werden!")

	return errorNPC;
}

Enemy* NPC_Manager::getEnemy(collisionbox* enemyCollisionbox)
{
	int mapPosX = Map_Manager::getMe().getMapPosX();
	int mapPosY = Map_Manager::getMe().getMapPosY();

	for(auto cursor:enemyList)
	{
		if(enemyCollisionbox == cursor.second->getCollisionbox())
		{
			return cursor.second;
		}
	}

	FOUT("Enemy-Collisionbox <" << enemyCollisionbox << "> konnte keinem Enemy zugeordnet werden!")

	return errorEnemy;
}

Enemy* NPC_Manager::getEnemy(short onMapID)
{
	if(enemyList.find(int(onMapID)) != enemyList.end()) return enemyList[onMapID];
	
	FOUT("Enemy-ID <" << onMapID << "> konnte keinem Enemy zugeordnet werden!")

	return errorEnemy;
}

const std::list<collisionbox*> &NPC_Manager::get_Activ_Collisionboxes()
{
	return allCollisionboxes;
}

void NPC_Manager::removeActivNPC(NPC* delNPC)
{
	bool found = false;

	std::list<NPC*>::iterator cursor;

	for(cursor = activNPCList.begin(); cursor != activNPCList.end(); ++cursor)
	{
		if((*cursor) == delNPC)
		{
			found = true;

			try
			{
				cursor = activNPCList.erase(cursor);

				if(cursor == activNPCList.end()) 
					break;
			}
			catch (const std::exception& e) 
			{
				FOUT(e.what())
			}
		}
	}

	if(found)
	{
		std::list<collisionbox*>::iterator cursor;

		for(cursor = allCollisionboxes.begin(); cursor != allCollisionboxes.end(); ++cursor)
		{
			if((*cursor) == delNPC->getCollisionbox())
			{
				try
				{
					cursor = allCollisionboxes.erase(cursor);

					if(cursor == allCollisionboxes.end()) 
						break;
				}
				catch (const std::exception& e) 
				{
					FOUT(e.what())
				}
			}
		}
	}
	else
	{
		FOUT("Activ-NPC <" << delNPC << "> konnte nicht in <activ-NPC list> gefunden werden!")
	}
}

void NPC_Manager::addActivNPC(NPC* newNPC)
{
	bool foundInNPCList = false;

	std::map<int,NPC*>::iterator cursor;

	for(cursor = npcList.begin(); cursor != npcList.end(); ++cursor)
	{
		if((*cursor).second == newNPC)
		{
			foundInNPCList = true;
		}
	}

	if(foundInNPCList)
	{
		bool foundInActivNPCList = false;

		std::list<NPC*>::iterator cursor;

		for(cursor = activNPCList.begin(); cursor != activNPCList.end(); ++cursor)
		{
			if((*cursor) == newNPC)
			{
				foundInActivNPCList = true;
			}
		}

		if(!foundInActivNPCList)
		{
			activNPCList.push_back(newNPC);
			allCollisionboxes.push_back(newNPC->getCollisionbox());
		}
		else
		{
			FOUT("NPC <" << newNPC << "> ist bereits in der Activ-NPC Liste!")
		}
	}
	else
	{
		FOUT("NPC <" << newNPC << "> ist nicht bekannt!")
	}
}

void NPC_Manager::removeActivEnemy(Enemy* delEnemy)
{
	bool found = false;

	std::list<Enemy*>::iterator cursor;

	for(cursor = activEnemyList.begin(); cursor != activEnemyList.end(); ++cursor)
	{
		if((*cursor) == delEnemy)
		{
			found = true;

			try
			{
				cursor = activEnemyList.erase(cursor);

				if(cursor == activEnemyList.end()) 
					break;
			}
			catch (const std::exception& e) 
			{
				FOUT(e.what())
			}
		}
	}

	if(found)
	{
		std::list<collisionbox*>::iterator cursor;

		for(cursor = allCollisionboxes.begin(); cursor != allCollisionboxes.end(); ++cursor)
		{
			if((*cursor) == delEnemy->getCollisionbox())
			{
				try
				{
					cursor = allCollisionboxes.erase(cursor);

					if(cursor == allCollisionboxes.end()) 
						break;
				}
				catch (const std::exception& e) 
				{
					FOUT(e.what())
				}
			}
		}
	}
	else
	{
		FOUT("Activ-Enemy <" << delEnemy << "> konnte nicht in <activ-Enemy list> gefunden werden!")
	}
}

void NPC_Manager::addActivEnemy(Enemy* newEnemy)
{
	bool foundInEnemyList = false;

	std::map<int,Enemy*>::iterator cursor;

	for(cursor = enemyList.begin(); cursor != enemyList.end(); ++cursor)
	{
		if((*cursor).second == newEnemy)
		{
			foundInEnemyList = true;
		}
	}

	if(foundInEnemyList)
	{
		bool foundInActivEnemyList = false;

		std::list<Enemy*>::iterator cursor;

		for(cursor = activEnemyList.begin(); cursor != activEnemyList.end(); ++cursor)
		{
			if((*cursor) == newEnemy)
			{
				foundInActivEnemyList = true;
			}
		}

		if(!foundInActivEnemyList)
		{
			activEnemyList.push_back(newEnemy);
			allCollisionboxes.push_back(newEnemy->getCollisionbox());
		}
		else
		{
			FOUT("Enemy <" << newEnemy << "> ist bereits in der Activ-Enemy Liste!")
		}
	}
	else
	{
		FOUT("Enemy <" << newEnemy << "> ist nicht bekannt!")
	}
}

std::string NPC_Manager::getNPCSaveData()
{
	std::string fullData;

	for(auto npcCursor : npcList)
	{
		fullData += "{npc}" + std::to_string(npcCursor.first) + "," + npcCursor.second->getMySaveData() + "\n";
	}

	for(auto enemyCursor : enemyList)
	{
		fullData += "{enemy}" + std::to_string(enemyCursor.first) + "," + enemyCursor.second->getMySaveData() + "\n";
	}

	return fullData;
}

bool NPC_Manager::loadNPCSaveData(std::string data)
{
	int idPos = data.find(",");
	std::string str_ID = data.substr(0,idPos);
	data = data.substr(idPos+1);

	NPC* loadedNPC = getNPC(atoi(str_ID.c_str()));

	loadedNPC->loadMySaveData(data);

	return false;
}

bool NPC_Manager::loadEnemySaveData(std::string data)
{
	int idPos = data.find(",");
	std::string str_ID = data.substr(0,idPos);
	data = data.substr(idPos+1);

	Enemy* loadedEnemy = getEnemy(atoi(str_ID.c_str()));

	loadedEnemy->loadMySaveData(data);

	return false;
}

NPC_Manager::~NPC_Manager(void)
{
	std::map<int,NPC*>::iterator cursor;

	for(cursor = npcList.begin(); cursor != npcList.end(); ++cursor)
	{
		delete cursor->second;
	}

	activNPCList.clear();

	std::map<int,Enemy*>::iterator cursorEnemy;

	for(cursorEnemy = enemyList.begin(); cursorEnemy != enemyList.end(); ++cursorEnemy)
	{
		delete cursorEnemy->second;
	}

	activNPCList.clear();

	delete errorNPC;
	delete errorEnemy;

	DELOUT("NPC_Manager wurde geloescht!\t(" << this << ")");
}
