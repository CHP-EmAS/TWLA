#pragma once

#include <list>
#include <string.h>
#include <fstream>
#include <sstream>

#include "Defines.h"
#include "Graphic_Manager.h"
#include "InteractivObject.h"

class Map
{
public:
	Map(std::string mapPath);

	bool isLoadCorrect();

	short getID();

	int getSpawnPointX(int spawnNumber);
	int getSpawnPointY(int spawnNumber);

	int getSizeX();
	int getSizeY();

	Graphic_Manager::tileset getTileset();

	tile *getTile(int x, int y);

	InteractivObject *getObject(int x,int y);
	InteractivObject *getObject(int objectID);
	const std::list<InteractivObject*> &getObjectList();

	const std::list<collisionbox*> &getCollisionBoxes();

	std::string getName();
	std::string getBackgroundMusicPath();
	short getAlphaLightLevel();

	Map *getMyPointer();

	void changeTileSetting(int x,int y,short setting);

	~Map(void);
private:
	
	void loadMapMainData();
	void loadMapLayers();
	void loadInteractivObjects();
	void loadMapNPCData();
	void loadSpawnPoints();

	void createNewMap();
	void deleteMap();

	short ID;

	const std::string mapPath;
	bool loadCorrect;

	tile **map;

	int sizeX;
	int sizeY;

	short lightLevel;

	Graphic_Manager::tileset tileset;

	std::list<spawnPoint> spawnPoints;
	std::list<InteractivObject*> interactivObjects;
	std::list<collisionbox*> collisionBoxes;

	std::string name;

	std::string backgroundMusicPath;

	InteractivObject *errorObject;
};

