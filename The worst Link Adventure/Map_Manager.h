#pragma once

#include "SFML/Graphics.hpp"
#include <list>

#include "Defines.h"

#include "Map.h"

class Map_Manager : public Singleton<Map_Manager>
{

friend class Singleton<Map_Manager>;

public:
	Map &getCurrentMap();

	std::string getMapName(int mapID);

	int getMapPosX();
	int getMapPosY();

	int getOnMapPosX(int x);
	int getOnMapPosY(int y);

	int getLastSpawnpoint();

	sf::IntRect getViewMapRectangle();

	sf::Vector2i convert_Pos_to_MapPos(sf::Vector2i pos);
	sf::Vector2i convert_MapPos_to_Pos(sf::Vector2i mapPos);

	sf::Vector2f convert_Pos_to_MapPos(sf::Vector2f pos);
	sf::Vector2f convert_MapPos_to_Pos(sf::Vector2f mapPos);

	bool isDrawingFrames();

	void changeCurrentMap(std::string name, int spawnPoint,bool saveGame = true);
	void changeCurrentMap(int ID, int spawnPoint,bool saveGame = true);

	void setMapPos(int x, int y);
	void showFrames(bool frame);

	void updateViewMap();
	void drawMap(sf::RenderWindow &mainWindow);
	void drawShadowOverlay(sf::RenderWindow &mainWindow);

	std::string get_Objects_Savedata_from_current_Map();
	bool load_Object_Savedata_from_current_Map(std::string singleObjectData);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Map_Manager(void);
private:
	Map_Manager(void);

	Map *currentMap;

	sf::Sprite aniSprite;
	sf::Sprite *viewMap[20][20];
	
	void initViewMap();
	
	sf::RectangleShape lightLevelRectangle;

	sf::Sprite small_CandleLightLevel_spr;
	sf::Sprite big_CandleLightLevel_spr;
	sf::Texture tex_smallCandle;
	sf::Texture tex_bigCandle;

	bool drawFrames;

	int mapPosX;
	int mapPosY;
	int lastMapPosX;
	int lastMapPosY;
	int lastSpawnPointID;

	std::map<int,std::string> map_Names;

	sf::Text coordDebugText;

	sf::Vector2i lastMapPos;
	std::string lastMapName;
};

