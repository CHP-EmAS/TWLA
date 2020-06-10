#include "Map.h"

#include "Screen_Manager.h"
#include "Graphic_Manager.h"
#include "NPC_Manager.h"
#include "File_Manager.h"

#include "Objects.h"

Map::Map(std::string mapPath)
:map(nullptr),
mapPath(mapPath + '/')
{
	lightLevel = 5;
	loadCorrect = true;

	DOUT("")
	NEWOUT("Neue Map <" << mapPath << "> wird erstellt! (" << this << ") \n")

	IOUT("Eigenschaften werden geladen...")
	loadMapMainData();

	DOUT("");
	IOUT("Interaktive Objekte werden erstellt...")
	loadInteractivObjects();

	DOUT("");
	IOUT("Map-Data wird geladen...")
	loadMapLayers();

	DOUT("");
	IOUT("Spawn-Punkte werden geladen...")
	loadSpawnPoints();

	DOUT("");
	IOUT("NPCs werden erstellt...")
	loadMapNPCData();

	if(loadCorrect)
	{
		DOUT("");
		SOUT("Map <" << this->name << "> wurde erfolgreich geladen!")
	}
	else
	{
		DOUT("");
		FOUT("Map <" << this->name << "> konnte nicht geladen werden!")
	}
	
	errorObject = new InteractivObject();

	//Initialisierung von error Objekt
	errorObject->setCollisionMode(false);
	errorObject->setInteract(false);
	errorObject->setObjectSprite(275,Graphic_Manager::Overworld);
	errorObject->setPosition(-1,-1);
}

//******GETTER******//
bool Map::isLoadCorrect()
{
	return loadCorrect;
}

int Map::getSpawnPointX(int spawnNumber)
{
	if(spawnPoints.size() > spawnNumber)
	{
		std::list<spawnPoint>::iterator cursor;
		cursor = spawnPoints.begin();
		int i = 0;

		while(i < spawnNumber)
		{
			cursor++;
			i++;
		}
	
		return cursor->x;
	}else
	{
		FOUT("Spawn Point <" << spawnNumber << "> in " << getName() << " konnte nicht gefunden werden! (Out of range!)")
		return 0;
	}
}

int Map::getSpawnPointY(int spawnNumber)
{
	if(spawnPoints.size() > spawnNumber)
	{
		std::list<spawnPoint>::iterator cursor;
		cursor = spawnPoints.begin();
		int i = 0;

		while(i < spawnNumber)
		{
			cursor++;
			i++;
		}
	
		return cursor->y;
	}else
	{
		FOUT("Spawn Point <" << spawnNumber << "> in " << getName() << " konnte nicht gefunden werden! (Out of range!)")
		return 0;
	}
}

int Map::getSizeX()
{
	return sizeX;
}

int Map::getSizeY()
{
	return sizeY;
}

Graphic_Manager::tileset Map::getTileset()
{
	return tileset;
}

tile *Map::getTile(int x, int y)
{
	if(x >= 0 && y >= 0 && x < sizeX && y < sizeY)
		return &(map[x][y]);
	else
		return &(map[0][0]);
}

//*******LOAD*******///
void Map::loadMapMainData()
{
	std::ifstream ifs;

	std::string mainPath = mapPath + "Header" + ZELDA_DATA_ENDING;

	ifs.open(mainPath, std::ios::in);

	if (ifs.good())
	{
		//dummy
		char c_dummy[75];

		//Name einlesen
		ifs.getline(c_dummy,75,'|');
		this->name = std::string(c_dummy);

		DOUT(" - Name: " << this->name)

		ifs.getline(c_dummy,75,'\n');
		this->ID = atoi(c_dummy);

		DOUT(" - ID: " << this->ID)

		//größe einlesen
		ifs.getline(c_dummy,10,'\n');
		sizeX = atoi(c_dummy);

		ifs.getline(c_dummy,10,'\n');
		sizeY = atoi(c_dummy);

		DOUT(" - Size: " << sizeX << "," << sizeY)

		//Tilest einlesen
		ifs.getline(c_dummy,10,'\n');
		this->tileset = Graphic_Manager::tileset(atoi(c_dummy));

		DOUT(" - Tileset: " << this->tileset)

		//big dummy
		char c_bigDummy[1000];
		ifs.getline(c_bigDummy,1000,'\n');
		this->backgroundMusicPath = c_bigDummy;

		DOUT(" - Background Music: " << this->backgroundMusicPath)

		ifs.getline(c_dummy,5,'\n');
		this->lightLevel = atoi(c_dummy);

		if(this->lightLevel > 5 || this->lightLevel < 0)
		{
			this->lightLevel = 5;
		}

		DOUT(" - Light-Level: " << this->lightLevel)

	}
	else
	{
		FOUT("Es ist ein Fehler beim Laden von <" << mainPath << "> aufgetreten! (Not found) (Critical!)" )

		sizeX = 0;
		sizeY = 0;
		tileset = Graphic_Manager::Overworld;
		name = "FAIL TO LOAD";

		loadCorrect = false;
	}

	ifs.close();

	createNewMap();
}

void Map::loadMapLayers()
{
	std::ifstream ifs;

	std::string mapDataPath = mapPath + "Map-Data" + ZELDA_DATA_ENDING;
	
	ifs.open(mapDataPath, std::ios::in);

	if (ifs.good())
	{
		//dummy
		char c_dummy[51];

		for(int y = 0; y < sizeY; y++)
		{
			for(int x = 0; x < sizeX; x++)
			{
				map[x][y].x = x;
				map[x][y].y = y;

				ifs.getline(c_dummy,10,',');
				map[x][y].tileNumber = atoi(c_dummy);

				if(map[x][y].tileNumber == -1) map[x][y].drawTileSprite = false;
				else map[x][y].drawTileSprite = true;

				int collisonInfo;
				ifs.getline(c_dummy,10,';');
				collisonInfo = atoi(c_dummy);

				map[x][y].settings = collisonInfo;

				if(collisonInfo == INTERACTIV)
				{
					InteractivObject* temp = getObject(x,y);

					if(temp != errorObject) map[x][y].boundObject = getObject(x,y);
					else map[x][y].boundObject = nullptr;
				}
				else
				{
					map[x][y].boundObject = nullptr;
				}
			}

			ifs.getline(c_dummy,10,'\n');
		}

		SOUT(sizeX*sizeY << " Felder wurden erfolgreich geladen!")

		ifs.getline(c_dummy,50,'\n');

		if(std::string(c_dummy) == "COLLISIONBOXES")
		{
			while(1)
			{
				ifs.getline(c_dummy,10,',');

				if(std::string(c_dummy) != "END")
				{
					collisionbox* box;

					box = new collisionbox();

					box->width = atoi(std::string(c_dummy).c_str());

					ifs.getline(c_dummy,10,',');
					box->height = atoi(c_dummy);

					ifs.getline(c_dummy,10,',');
					box->mapPosX = atoi(c_dummy);

					ifs.getline(c_dummy,10,'\n');
					box->mapPosY = atoi(c_dummy);

					box->type = Standart_Collisionbox;
					box->boundNPC = nullptr;
					box->boundObj = nullptr;
					box->boundEnemy = nullptr;

					collisionBoxes.push_back(box);
				}
				else
				{
					break;
				}
			}
		}
	}
	else
	{
		loadCorrect = false;
		FOUT("Es ist ein Fehler beim Laden von <" << mapDataPath << "> aufgetreten! (Not found) (Critical!)")
	}

	ifs.close();
}

void Map::loadInteractivObjects()
{
	std::string objectPath = mapPath + "Objects" + ZELDA_DATA_ENDING;
	
	std::list<fullCommand> objectList = File_Manager::getMe().convertFileToCommandlist(objectPath);

	for(auto objectCursor : objectList)
	{
		int cutPos = objectCursor.content.find(",");
		short tempID = atoi(objectCursor.content.substr(0,cutPos).c_str());
		objectCursor.content = objectCursor.content.substr(cutPos+1);

		cutPos = objectCursor.content.find(",");
		int tempPosX = atoi(objectCursor.content.substr(0,cutPos).c_str());
		objectCursor.content = objectCursor.content.substr(cutPos+1);

		cutPos = objectCursor.content.find(",");
		int tempPosY = atoi(objectCursor.content.substr(0,cutPos).c_str());
		objectCursor.content = objectCursor.content.substr(cutPos+1);

		if(objectCursor.command == "chest")
		{
			cutPos = objectCursor.content.find(",");
			int tempKeyItem = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content = objectCursor.content.substr(cutPos+1);

			int tempItem = atoi(objectCursor.content.c_str());

			Chest *obj = new Chest(tempPosX,tempPosY,tempID,tempKeyItem,tempItem);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "door")
		{
			cutPos = objectCursor.content.find(",");
			int tempKey = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			std::string tempPortWorld = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			int tempPortWorldSpanwPoint = atoi(objectCursor.content.c_str());

			Door *obj = new Door(tempPosX,tempPosY,tempID,tempKey,tempPortWorld,tempPortWorldSpanwPoint);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "portal")
		{
			cutPos = objectCursor.content.find(",");
			std::string tempPortWorld = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			int tempPortWorldSpanwPoint = atoi(objectCursor.content.c_str());
		
			Portal *obj = new Portal(tempPosX,tempPosY,tempID,tempPortWorld,tempPortWorldSpanwPoint);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "sign")
		{
			cutPos = objectCursor.content.find(",");
			std::string message = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			int colorR = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			int colorG = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			int colorB = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			int type = atoi(objectCursor.content.substr(0,cutPos).c_str());

			Sign *obj = new Sign(tempPosX,tempPosY,tempID,message,sf::Color(colorR,colorG,colorB),type);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "cactus")
		{
			float tempDamage = atof(objectCursor.content.c_str());

			Cactus *obj = new Cactus(tempPosX,tempPosY,tempID,tempDamage);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "hook")
		{
			cutPos = objectCursor.content.find(",");
			Graphic_Manager::tileset tempTileset = Graphic_Manager::tileset(atoi(objectCursor.content.substr(0,cutPos).c_str()));
			objectCursor.content = objectCursor.content.substr(cutPos+1);
			
			int tempTile = atoi(objectCursor.content.c_str());

			Grabable_Tile *obj = new Grabable_Tile(tempPosX,tempPosY,tempID,tempTileset,tempTile);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "stone")
		{
			short tempType = atof(objectCursor.content.c_str());

			Stone *obj = new Stone(tempPosX,tempPosY,tempID,tempType);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "bush")
		{
			short tempType = atoi(objectCursor.content.c_str());

			Bush *obj = new Bush(tempPosX,tempPosY,tempID,tempType);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "s_bush")
		{
			cutPos = objectCursor.content.find(",");
			std::string tempPortWorld = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			int tempPortWorldSpanwPoint = atoi(objectCursor.content.c_str());

			Secret_Bush *obj = new Secret_Bush(tempPosX,tempPosY,tempID,tempPortWorld,tempPortWorldSpanwPoint);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "key_stone")
		{
			cutPos = objectCursor.content.find(",");
			short tempType = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			int tempKey = atoi(objectCursor.content.c_str());

			KeyStone *obj = new KeyStone(tempPosX,tempPosY,tempID,tempType,tempKey);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "trigger")
		{
			std::list<std::string> commands;

			cutPos = objectCursor.content.find(",");
			interactType triggerType = interactType(atoi(objectCursor.content.substr(0,cutPos).c_str()));
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			bool multiTrigger = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			std::string hintMsg = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			std::string allCommands = objectCursor.content;

			while(true)
			{
				cutPos = allCommands.find(";");

				if(cutPos != std::string::npos)
				{
					commands.push_back(allCommands.substr(0,cutPos));
					allCommands = allCommands.substr(cutPos+1);
				}
				else
				{
					if(allCommands != "")
					{
						commands.push_back(allCommands);
					}

					break;
				}
			}

			Trigger *obj = new Trigger(tempPosX,tempPosY,tempID,commands,multiTrigger,triggerType,hintMsg);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "teleporter")
		{
			int tempSpanwPoint = atoi(objectCursor.content.c_str());
		
			Teleporter *obj = new Teleporter(tempPosX,tempPosY,tempID,tempSpanwPoint);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "button")
		{
			cutPos = objectCursor.content.find(",");
			std::string tempPressCommand = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			std::string tempReleaseCommand = objectCursor.content.substr(0,cutPos);
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			cutPos = objectCursor.content.find(",");
			bool pressed = atoi(objectCursor.content.substr(0,cutPos).c_str());
			objectCursor.content = objectCursor.content.substr(cutPos+1);

			int type = atoi(objectCursor.content.c_str());

			Button *obj = new Button(tempPosX,tempPosY,tempID,tempPressCommand,tempReleaseCommand,pressed,type);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "cmdDoor")
		{
			std::list<std::string> commands;
			std::list<std::string> checkedCommands;

			int cmdCutPos = objectCursor.content.find(";");

			std::string allCommands = objectCursor.content.substr(0,cmdCutPos);
			std::string allCheckedCommands = objectCursor.content.substr(cmdCutPos+1);

			while(true)
			{
				cutPos = allCommands.find(",");

				if(cutPos != std::string::npos)
				{
					commands.push_back(allCommands.substr(0,cutPos));
					allCommands = allCommands.substr(cutPos+1);
				}
				else
				{
					if(allCommands != "")
					{
						commands.push_back(allCommands);
					}

					break;
				}
			}

			while(true)
			{
				cutPos = allCheckedCommands.find(",");

				if(cutPos != std::string::npos)
				{
					checkedCommands.push_back(allCheckedCommands.substr(0,cutPos));
					allCheckedCommands = allCheckedCommands.substr(cutPos+1);
				}
				else
				{
					if(allCheckedCommands != "")
					{
						checkedCommands.push_back(allCheckedCommands);
					}

					break;
				}
			}

			CommandDoor *obj = new CommandDoor(tempPosX,tempPosY,tempID,commands,checkedCommands);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "jar")
		{
			int tempItem = atoi(objectCursor.content.c_str());

			Jar *obj = new Jar(tempPosX,tempPosY,tempID,tempItem);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "fragmentIns")
		{
			FragmentInserter *obj = new FragmentInserter(tempPosX,tempPosY,tempID);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "break")
		{
			BreakAbyss *obj = new BreakAbyss(tempPosX,tempPosY,tempID);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
		else if(objectCursor.command == "globalWall")
		{
			bool tempOpen = atoi(objectCursor.content.c_str());

			GlobalWall *obj = new GlobalWall(tempPosX,tempPosY,tempID,tempOpen);
			obj->setMapID(ID);

			interactivObjects.push_back(obj);
		}
	}
}

void Map::loadMapNPCData()
{
	std::string NPCPath = mapPath + "NPCs" + ZELDA_DATA_ENDING;
	
	std::list<fullCommand> fullCommandsList = File_Manager::getMe().convertFileToCommandlist(NPCPath);
		
	for(auto cursor : fullCommandsList)
	{
		if(cursor.command == "NPC")
		{
			int searchPos = cursor.content.find(",");
			int onMapID = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			int npcID = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			bool isActiv = bool(atoi(cursor.content.substr(0,searchPos).c_str()));
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			int posX = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			int posY = atoi(cursor.content.c_str());

			NPC *newNPC = NPC_Manager::getMe().createNPC(onMapID,npcID,posX,posY);

			if(isActiv) newNPC->setActiv(true);
		}
		else if(cursor.command == "Enemy")
		{
			int searchPos = cursor.content.find(",");
			int onMapID = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			int enemyType = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			bool isActiv = bool(atoi(cursor.content.substr(0,searchPos).c_str()));
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			int posX = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			searchPos = cursor.content.find(",");
			int posY = atoi(cursor.content.substr(0,searchPos).c_str());
			cursor.content = cursor.content.substr(searchPos+1);

			Enemy *newEnemy = NPC_Manager::getMe().createEnemy(onMapID,Enemy::EnemyType(enemyType),posX,posY,cursor.content);

			if(isActiv) newEnemy->setActiv(true);
		}
	}
}

void Map::loadSpawnPoints()
{
	std::ifstream ifs;

	std::string SpawnpointPath = mapPath + "Spawnpoints" + ZELDA_DATA_ENDING;
	
	ifs.open(SpawnpointPath, std::ios::in);

	if (ifs.good())
	{
		//dummy
		char c_dummy[11];

		//Spawnpunkte einlesen
		while(1)
		{
			ifs.getline(c_dummy,10,',');

			if(std::string(c_dummy) != "END")
			{
				spawnPoint sp;

				sp.x = atoi(c_dummy);

				ifs.getline(c_dummy,10,'\n');
				sp.y = atoi(c_dummy);

				spawnPoints.push_back(sp);

				DOUT(" - Spawnpoint: " << sp.x << "," << sp.y)
			}else
			{
				ifs.getline(c_dummy,10,'\n');
				break;
			}
		}
	}
	else
	{
		loadCorrect = false;
		FOUT("Es ist ein Fehler beim Laden von <" << SpawnpointPath << "> aufgetreten! (Not found) (Critical!)" )

		spawnPoint sp;
		sp.x = 0;
		sp.y = 0;
		spawnPoints.push_back(sp);
	}

	ifs.close();
}

void Map::createNewMap()
{
	deleteMap();

	this->map = new tile *[sizeX];
		
	for( int x = 0; x < sizeX;x++)
	{
		this->map[x] = new tile [sizeY];
	}	
}

void Map::deleteMap()
{
	if(map != nullptr)
	{
		for(int x = 0; x < sizeX;x++)
		{
			delete [] this->map[x];
		}
		delete [] this->map;
		
		map = nullptr;
	}
}

InteractivObject *Map::getObject(int x,int y)
{
	std::list<InteractivObject*>::iterator it;

	for(it = interactivObjects.begin(); it != interactivObjects.end(); it++)
	{
		if((*it)->getPosX() == x && (*it)->getPosY() == y)
		{
			return *it;
		}

	}

	FOUT("Objekt <" << x << "|" << y << "> konnte nicht gefunden werden! (Out of range!)")
	
	errorObject->setPosition(x,y);
	return errorObject;
}

InteractivObject *Map::getObject(int objectID)
{
	std::list<InteractivObject*>::iterator it;

	for(it = interactivObjects.begin(); it != interactivObjects.end(); it++)
	{
		if((*it)->getID() == objectID)
		{
			return *it;
		}
	}
	
	FOUT("Objekt <" << objectID << "> konnte nicht gefunden werden!")
	
	errorObject->setPosition(-1,-1);
	return errorObject;
}

const std::list<InteractivObject*> &Map::getObjectList()
{
	return interactivObjects;
}

const std::list<collisionbox*> &Map::getCollisionBoxes()
{
	return collisionBoxes;
}

std::string Map::getName()
{
	return name;
}

short Map::getID()
{
	return ID;
}

std::string Map::getBackgroundMusicPath()
{
	return backgroundMusicPath;
}

short Map::getAlphaLightLevel()
{
	return (255.f - float(lightLevel) * 51.f);
}

Map *Map::getMyPointer()
{
	return this;
}

void Map::changeTileSetting(int x,int y,short setting)
{
	if(x >= 0 && y >= 0 && x < sizeX && y < sizeY)
	{
		 map[x][y].settings = setting;
	}
	else
	{
		WOUT("Tile <" << x << "," << y << "> ist außerhalb der Map!");
	}
}

Map::~Map(void)
{
	deleteMap();

	std::list<InteractivObject*>::iterator it;
	for(it = interactivObjects.begin(); it != interactivObjects.end(); it++)
		delete *it;

	interactivObjects.clear();

	for(auto cursor:collisionBoxes)
		delete cursor;

	collisionBoxes.clear();

	delete errorObject;

	DELOUT("Map <" << this->name << "> wurde geloescht!\t(" << this << ")")
}
