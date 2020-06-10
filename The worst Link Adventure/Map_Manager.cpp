#include "Map_Manager.h"

#include "Graphic_Manager.h"
#include "InteractivObject.h"
#include "NPC_Manager.h"
#include "Particle_Manager.h"
#include "Animation_Manager.h"
#include "Weapon_Manager.h"
#include "Sound_Manager.h"
#include "Player_Manager.h"
#include "File_Manager.h"
#include "Message_Manager.h"
#include "Dialog_Manager.h"

Map_Manager::Map_Manager(void)
{
	NEWOUT("Neuer Map_Manager wurde erstellet! (" << this << ")");
	
	currentMap = nullptr;
	lastSpawnPointID = -1;

	initViewMap();
	
	drawFrames = false;
	lastMapPosX = -1000;
	lastMapPosY = -1000;

	lightLevelRectangle.setSize(sf::Vector2f(WINDOW_SIZE_X+2,WINDOW_SIZE_Y+2));
	lightLevelRectangle.setPosition(-1,-1);
	lightLevelRectangle.setFillColor(sf::Color(0,0,0,102));

	if(!tex_smallCandle.loadFromFile("Img/ect/candle small.png"))
	{
		FOUT("candle small.png konnte nicht geladen werden!");
	}

	if(!tex_bigCandle.loadFromFile("Img/ect/candle big.png"))
	{
		FOUT("candle big.png konnte nicht geladen werden!");
	}

	small_CandleLightLevel_spr.setTexture(tex_smallCandle);
	small_CandleLightLevel_spr.setOrigin(small_CandleLightLevel_spr.getGlobalBounds().width/2,small_CandleLightLevel_spr.getGlobalBounds().height/2);

	big_CandleLightLevel_spr.setTexture(tex_bigCandle);
	big_CandleLightLevel_spr.setOrigin(big_CandleLightLevel_spr.getGlobalBounds().width/2,big_CandleLightLevel_spr.getGlobalBounds().height/2);
	
	std::ifstream ifs;

	ifs.open("Maps/map_list.zData", std::ios::in);

	if (ifs.good())
	{
		char c_dummy[500];

		while(true)
		{
			ifs.getline(c_dummy,200,' ');
			int mapID = atoi(c_dummy);

			if(std::strcmp(c_dummy,"") == 0)break;

			ifs.getline(c_dummy,200,'\n');
			std::string mapName = c_dummy;

			std::map<int,std::string>::iterator it;
			it = map_Names.find(mapID);

			if(it != map_Names.end())
			{
				FOUT("Map-ID: " << mapID << " ist doppelt definiert!");
				WOUT("Map " << mapID << " wird überschrieben!");
			}

			map_Names[mapID] = mapName;
		}
	}
	else
	{
		FOUT("[CRITICAL] - Mapliste konnte nicht geöffnet werden!");
	}

	coordDebugText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),10);
}

Map &Map_Manager::getCurrentMap()
{
	if(currentMap == nullptr)
	{
		CRITOUT("Es wird auf eine nicht geladene Map zugegriffen!\nDies kann zum Absturz füren!\n\n!Programm Fehler!")
	}

	return *currentMap;
}

std::string Map_Manager::getMapName(int mapID)
{
	return map_Names[mapID];
}

int Map_Manager::getMapPosX()
{
	return mapPosX;
}

int Map_Manager::getMapPosY()
{
	return mapPosY;
}

int Map_Manager::getLastSpawnpoint()
{
	return lastSpawnPointID;
}

sf::IntRect Map_Manager::getViewMapRectangle()
{
	sf::IntRect view;

	view.left = -mapPosX;
	view.top = -mapPosY;
	view.width = WINDOW_SIZE_X;
	view.height = WINDOW_SIZE_Y;

	return view;
}

sf::Vector2i Map_Manager::convert_Pos_to_MapPos(sf::Vector2i pos)
{
	pos.x = pos.x - mapPosX;
	pos.y = pos.y - mapPosY;
	
	return pos;
}

sf::Vector2i Map_Manager::convert_MapPos_to_Pos(sf::Vector2i mapPos)
{
	mapPos.x = mapPos.x + mapPosX;
	mapPos.y = mapPos.y + mapPosY;
	
	return mapPos;
}

sf::Vector2f Map_Manager::convert_Pos_to_MapPos(sf::Vector2f pos)
{
	pos.x = pos.x - mapPosX;
	pos.y = pos.y - mapPosY;
	
	return pos;
}

sf::Vector2f Map_Manager::convert_MapPos_to_Pos(sf::Vector2f mapPos)
{
	mapPos.x = mapPos.x + mapPosX;
	mapPos.y = mapPos.y + mapPosY;
	
	return mapPos;
}

int Map_Manager::getOnMapPosX(int x)
{
	int posXNow = x - mapPosX;
	return (posXNow - (posXNow % 32)) / 32;
}

int Map_Manager::getOnMapPosY(int y)
{
	int posYNow = y - mapPosY;
	return (posYNow - (posYNow % 32)) / 32;
}

bool Map_Manager::isDrawingFrames()
{
	return drawFrames;
}

void Map_Manager::changeCurrentMap(std::string name, int spawnPoint,bool saveGame)
{
	bool loadLastMap = false;

	std::string str_lastMapName = "NONE";
	int lastMapPosX = -1;
	int lastMapPosY = -1;

	if(name == "{lastMap}")
	{
		//Letzt Karte laden
		name = lastMapName;
		loadLastMap = true;
	}
	else
	{
		//Prüfen ob neue Karte vorhanden
		bool found = false;

		for(auto cursor:map_Names)
		{
			if(cursor.second == name)
			{
				found = true;
				break;
			}
		}

		if(!found && name != "{lastMap}")
		{
			FOUT("Map <" << name << "> ist in der Map-Liste nicht vorhanden!")
			return;
		}
	}

	//Vorherige Map löschen wenn vorhanden
	if(currentMap != nullptr)
	{
		IOUT("Map <" << currentMap->getName() << "> wird gelöscht!")

		//Map speichern
		File_Manager::getMe().saveCurrentMapChanges();

		//letzte Map-Daten speichern
		str_lastMapName = currentMap->getName();
		lastMapPosX = Player_Manager::getMe().getLastMapPos().x;
		lastMapPosY = Player_Manager::getMe().getLastMapPos().y;

		//Map löschen
		delete currentMap;
	}

	//Sonstige Listen löschen (NPCs, Partikel, Animation, ect.)
	NPC_Manager::getMe().delete_Enemys_NPCs();
	Particle_Manager::getMe().clearParticleList();
	Animation_Manager::getMe().cancelAllAnimation();
	Weapon_Manager::getMe().deleteAllWeapons();
	Player_Manager::getMe().clearPlayerHitboxes();
	Player_Manager::getMe().resetItemVariables();
	Sound_Manager::getMe().clearAllEffectSounds();

	DOUT("");
	IOUT("Map <" << name << "> wird geladen...")

	//Neue Map laden
	Map *loadMap = new Map("Maps/" + name);

	if(loadMap->isLoadCorrect())
	{
		//neue Map auf aktuelle Map setzen
		currentMap = loadMap;

		//Map-Tileset erneuern
		for(int x = 0; x < 20; x++)
		{
			for(int y = 0; y < 20; y++)
			{
				viewMap[x][y]->setTexture(Graphic_Manager::getMe().getTilesetTexture(currentMap->getTileset()));
			}
		}

		//Spieler zu gewünschten Spawnpoint teleportieren
		Player_Manager::getMe().tp(currentMap->getSpawnPointX(spawnPoint),currentMap->getSpawnPointY(spawnPoint));
		lastSpawnPointID = spawnPoint;
		updateViewMap();

		//Musik-Thema ändern
		if(currentMap->getBackgroundMusicPath() != "NONE" && currentMap->getBackgroundMusicPath() != "CONTINUE")
		{
			Sound_Manager::getMe().changeBackgroundMusicSmoothly(currentMap->getBackgroundMusicPath());
		}
		else if(currentMap->getBackgroundMusicPath() != "CONTINUE")
		{
			Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Status::Stopped);
		}

		//Lichtlevel setzen
		if(currentMap->getAlphaLightLevel() >= 0 && currentMap->getAlphaLightLevel() <= 255)
		{
			lightLevelRectangle.setFillColor(sf::Color(0,0,0,currentMap->getAlphaLightLevel()));
			small_CandleLightLevel_spr.setColor(sf::Color(255,255,255,currentMap->getAlphaLightLevel()));
			big_CandleLightLevel_spr.setColor(sf::Color(255,255,255,currentMap->getAlphaLightLevel()));
		}
		
		//Änderungen der neuen Karte laden
		File_Manager::getMe().loadCurrentMapChanges();

		//Spielstand speichern
		if(saveGame) File_Manager::getMe().save_Savegame();

		//Auf letzte Karte teleportieren, wenn Spieler keine Kerze besitzt und Karte zu dunkel
		if(currentMap->getAlphaLightLevel() == 255 )
		{
			if(str_lastMapName != "NONE" && lastMapPosX != -1 && lastMapPosY != -1 && !Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::Candle))
			{
				Message_Manager::getMe().showMsg("Hier ist es viel zu dunkel,ich kann nichts sehen! Ein Kerze würde hier weiter helfen!",sf::Color(255,255,0));
				changeCurrentMap(str_lastMapName,0,true);
				Player_Manager::getMe().tp(lastMapPosX,lastMapPosY);
				lastSpawnPointID = -1;
			}
		}
		else if(loadLastMap)
		{
			//Letzte Karte laden
			Player_Manager::getMe().tp(lastMapPos.x,lastMapPos.y);
			lastSpawnPointID = -1;
		}
	}
	else
	{
		//Wenn Laden fehlgeschlagen

		if(str_lastMapName == "NONE")
		{
			//Letzt Karte laden
			delete loadMap;
			NPC_Manager::getMe().delete_Enemys_NPCs();
			Particle_Manager::getMe().clearParticleList();
			Animation_Manager::getMe().cancelAllAnimation();
			Weapon_Manager::getMe().deleteAllWeapons();
			Player_Manager::getMe().clearPlayerHitboxes();

			Map *lastMap = new Map("Maps/" + lastMapName);
			currentMap = lastMap;
		}
		else
		{
			//Fatal Error
			CRITOUT("Anfangsmap konnte nicht geladen werden!")
		}
	}

	//Karten Name anzeigen
	Message_Manager::getMe().showLocationMsg(name);

	//Spieler aktualisieren
	Player_Manager::getMe().updatePlayerHitBoxes();
	Player_Manager::getMe().update();

	//Feinde aktualisieren
	Screen_Manager::getMe().updateActivScreen();
	NPC_Manager::getMe().update_Enemys_NPCs();

	//NPCs neu zeichnen
	NPC_Manager::getMe().draw_Enemys(Screen_Manager::getMe().getGameWindow());
	NPC_Manager::getMe().draw_NPCs(Screen_Manager::getMe().getGameWindow());

	this->lastMapName = str_lastMapName;
	this->lastMapPos.x = lastMapPosX;
	this->lastMapPos.y = lastMapPosY;

	if(currentMap != nullptr)
	{
		if(currentMap->getID() == 11)
		{
			Dialog_Manager::getMe().doDialog(9998);
		}
	}
}

void Map_Manager::changeCurrentMap(int ID, int spawnPoint,bool saveGame)
{
	changeCurrentMap(map_Names[ID],spawnPoint,saveGame);
}

void Map_Manager::setMapPos(int x, int y)
{
	mapPosX = x;
	mapPosY = y;
}

void Map_Manager::showFrames(bool frame)
{
	if(!frame)
	{
		drawFrames = false;
	}
	else if(Screen_Manager::getMe().debugMode())
	{
		drawFrames = true;
	}
}

void Map_Manager::drawMap(sf::RenderWindow &mainWindow)
{
	//Prüfen ob Karte geladen
	if(currentMap == nullptr)
	{
		FOUT("Keine Map geladen! Map kann nicht gezeichnet werden!");
		return;
	}

	short tileNumber;
	
	//aktuelle Kartenposition berechnen
	int currentMapPosX = -(mapPosX - mapPosX % ZOOMSIZE) / ZOOMSIZE;
	int currentMapPosY = -(mapPosY - mapPosY % ZOOMSIZE) / ZOOMSIZE;

	//Tile-Grafiken neu setzen wenn nötig
	if(currentMapPosX != lastMapPosX || currentMapPosY != lastMapPosY)
	{
		updateViewMap();
	}

	//Karte im Anzeigebereich zeichnen
	for(int x = 0; x < 20; x++)
	{
		for(int y = 0; y < 20; y++)
		{
			//Tileposition ermitteln
			int xT = currentMapPosX + x;
			int yT = currentMapPosY + y;

			//Wenn Tileposition existiert
			if(xT >= 0 && yT >= 0 && xT < currentMap->getSizeX() && yT < currentMap->getSizeY())
			{
				//Wasseranimation zeichnen falls nötig
				if(currentMap->getTile(xT,yT)->settings == WATER)
				{
					aniSprite = Graphic_Manager::getMe().getAnimationSprite(Graphic_Manager::Water_Animation);
					aniSprite.setPosition( ( x * ZOOMSIZE ) + ( mapPosX % ZOOMSIZE ), ( y*ZOOMSIZE ) + ( mapPosY % ZOOMSIZE ));

					mainWindow.draw(aniSprite);
				}
				else if(currentMap->getTile(xT,yT)->settings == LAVA) //Lavaanimation zeichnen falls nötig
				{
					aniSprite = Graphic_Manager::getMe().getAnimationSprite(Graphic_Manager::Lava_Animation);
					aniSprite.setPosition( ( x * ZOOMSIZE ) + ( mapPosX % ZOOMSIZE ), ( y*ZOOMSIZE ) + ( mapPosY % ZOOMSIZE ));

					mainWindow.draw(aniSprite);
				}
				
				//Tilenummer setzen
				tileNumber = currentMap->getTile(xT,yT)->tileNumber;
				
				if(tileNumber >= 0)
				{
					//Tile zeichnen
					viewMap[x][y]->setPosition( ( x * ZOOMSIZE ) + ( mapPosX % ZOOMSIZE ), ( y*ZOOMSIZE ) + ( mapPosY % ZOOMSIZE ));
					mainWindow.draw(*viewMap[x][y]);
				}

				//Objekt zeichnen falls nötig
				if(currentMap->getTile(xT,yT)->settings == INTERACTIV)
				{
					currentMap->getTile(xT,yT)->boundObject->draw(mainWindow,( x * ZOOMSIZE ) + ( mapPosX % ZOOMSIZE ), ( y*ZOOMSIZE ) + ( mapPosY % ZOOMSIZE ));
				}
			}
		}
	}
}

void Map_Manager::drawShadowOverlay(sf::RenderWindow &mainWindow)
{
	if(currentMap->getAlphaLightLevel() > 0)
	{
		bool isCandleUnlocked = Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::Candle);
		int candle = Inventory_Manager::getMe().getSlotItem(Inventory_Manager::Candle);

		if(isCandleUnlocked)
		{
			switch(candle)
			{
			case 37:
				small_CandleLightLevel_spr.setPosition(Player_Manager::getMe().getPosX()+16,Player_Manager::getMe().getPosY()+16);
				mainWindow.draw(small_CandleLightLevel_spr);
				break;
			case 38:
				big_CandleLightLevel_spr.setPosition(Player_Manager::getMe().getPosX()+16,Player_Manager::getMe().getPosY()+16);
				mainWindow.draw(big_CandleLightLevel_spr);
				break;
			}
		}
		else
		{
			mainWindow.draw(lightLevelRectangle);
		}
		
	}
}

std::string Map_Manager::get_Objects_Savedata_from_current_Map()
{
	std::string content = "";

	std::list<InteractivObject*> objList = currentMap->getObjectList();

	for(auto objCursor : objList)
	{
		content += "{object}" + std::to_string(objCursor->getID()) + "," + objCursor->getMySaveData() + "\n";
	}

	return content;
}

bool Map_Manager::load_Object_Savedata_from_current_Map(std::string singleObjectData)
{
	int idPos = singleObjectData.find(",");
	std::string str_ID = singleObjectData.substr(0,idPos);
	singleObjectData = singleObjectData.substr(idPos+1);

	InteractivObject* loadObject = currentMap->getObject(atoi(str_ID.c_str()));

	loadObject->loadMySaveData(singleObjectData);

	return true;
}

std::string Map_Manager::getMySaveData()
{
	std::string data = "";

	data += lastMapName + ",";
	data += std::to_string(lastMapPos.x) + ",";
	data += std::to_string(lastMapPos.y);

	return data;
}

bool Map_Manager::loadMySaveData(std::string data)
{
	int cutPos = data.find(",");
	this->lastMapName = data.substr(0,cutPos);
	data = data.substr(cutPos+1);

	cutPos = data.find(",");
	this->lastMapPos.x = atoi(data.substr(0,cutPos).c_str());
	data = data.substr(cutPos+1);

	this->lastMapPos.y = atoi(data.c_str());

	return true;
}

void Map_Manager::updateViewMap()
{
	int tileNumber;

	int currentMapPosX = -(mapPosX - mapPosX % ZOOMSIZE) / ZOOMSIZE;
	int currentMapPosY = -(mapPosY - mapPosY % ZOOMSIZE) / ZOOMSIZE;

	lastMapPosX = currentMapPosX;
	lastMapPosY = currentMapPosY;

	for(int x = 0; x < 20; x++)
	{
		for(int y = 0; y < 20; y++)
		{
			int xT = currentMapPosX + x;
			int yT = currentMapPosY + y;

			tileNumber = currentMap->getTile(xT,yT)->tileNumber;
			viewMap[x][y]->setTextureRect(Graphic_Manager::getMe().getIntRect(currentMap->getTileset(),tileNumber));
		}
	}
}

void Map_Manager::initViewMap()
{
	for(int x = 0; x < 20; x++)
	{
		for(int y = 0; y < 20; y++)
		{
			viewMap[x][y] = new sf::Sprite();
			viewMap[x][y]->setScale(SCALE,SCALE);
		}
	}
}

Map_Manager::~Map_Manager(void)
{
	if(currentMap != nullptr) delete currentMap;

	for(int x = 0; x < 20; x++)
	{
		for(int y = 0; y < 20; y++)
		{
			delete viewMap[x][y];
		}
	}
	
	DELOUT("Map_Manager wurde geloescht!\t(" << this << ")");
}
