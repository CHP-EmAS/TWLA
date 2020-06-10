#pragma once
#include <list>

#include "SFML/Audio.hpp"
#include "SFML/Graphics.hpp"

#include "Defines.h"
#include "screen.h"
#include "Inventory_Manager.h"
#include "Quest.h"

class Bomb_Shop_Screen : public Screen
{
public:
	Bomb_Shop_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Bomb_Shop_Screen(void);
private:
	void initScreen();

	sf::Text shopHeadline;
	sf::Text shopKeyInfo;
	sf::RectangleShape frame;

	sf::Sprite item[3];
	sf::Text money[3];

	int price[3];

	sf::RectangleShape itemFrame;
	sf::RectangleShape moneyframe;

	short selectedRow;

	bool showArrow;
	bool showBomb;
	bool showWaterBomb;
};

class Credits_Screen : public Screen
{
public:
	Credits_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Credits_Screen(void);
private:
	void initScreen();

	struct credit
	{
		std::list<std::string> names;
		std::string title;
		
		sf::Color titleColor;
	};

	void nextCredit(std::list<credit>::iterator it);

	sf::Text titleText;
	std::list<sf::Text*> nameTexts;

	sf::Clock textMoveClock;

	std::list<credit> textList;
	std::list<credit>::iterator creditIterator;

	int creditSize;
	int waitTime;

	sf::Text nopeText;
	int nopeAlpha;
	short easterEgg;

	sf::Texture eponaTexture;
	sf::Sprite eponaSprite;
	bool showPicture;
	int pictureNumber;
	sf::Clock frameClock;
};

#define MAX_MELODIES 7

class Flute_Screen : public Screen
{
public:
	Flute_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	void activateMelody(short melodyID);

	bool loadMySaveData(std::string data);
	std::string getMySaveData();

	~Flute_Screen(void);
private:
	void initScreen();

	void createNewLine();
	void changeCurrentLine(short line);
	void changeMelody(short Melody, short dir = 0);
	void resetLines();

	//Text
	sf::Text melodyName;
	sf::Text keyInfo;

	//Frame
	sf::RectangleShape frame;

	//Flute
	sf::Sprite flute;

	//clef
	sf::Texture tex_clef;
	sf::Sprite spr_clef;

	//notes
	sf::Texture tex_note;
	sf::Sprite spr_note;

	sf::RectangleShape noteLines;

	//UpdateLines
	std::list<sf::RectangleShape> updateLines;

	int updateLineEnd;
	unsigned short currentSelectedLine;

	sf::Clock lineMoveClock;

	//Melody
	struct notePosition
	{
		short posLine;
		short status;
	};

	struct melody
	{
		std::string name;
		notePosition notes[9];
		bool found;
		short ID;
	};

	melody melodies[MAX_MELODIES];
	melody currentMelody;

	//Triangle
	sf::VertexArray lineTriangle;
};

class Game_Over_Screen : public Screen
{
public:
	Game_Over_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Game_Over_Screen(void);
private:
	void initScreen();

	sf::Text gameoverText;
	sf::Text infoText;

	sf::Clock fadeTextClock;
	int fadeAlpha;

	sf::Sprite grabstein;
	bool musicIsPlaying;

	short speedX;
	short speedY;

	int easterEgg;
	bool setSetting;
};

class Game_Screen : public Screen
{
public:
	Game_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Game_Screen(void);
private:
	void initScreen();

	//fps
	sf::Text fpsText;
	sf::Text collisionText;
	sf::Clock fpsClock;
	int fps;
	bool showFPS;
};

class Inventory_Screen : public Screen
{
public:
	Inventory_Screen(void);

	void initScreen();

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Inventory_Screen(void);
private:
	void moveSelectedSlot(int dir);

	//Player Dummy
	sf::Sprite playerDummySprite;
	sf::Clock playerMoveAniClock;
	short movingStep;

	//Icons
	sf::Sprite settingsIcon;
	sf::Sprite saveIcon;
	sf::Sprite xIcon;
	sf::Sprite questIcon;
	sf::Sprite archievementIcon;

	//Triforce
	sf::Sprite triForce;
	sf::Clock triforceAniClock;
	int shiftTile;
	bool shiftMode;

	//Rectangles
	sf::RectangleShape mainFrame;
	sf::RectangleShape slotFrame;
	sf::RectangleShape line;
	
	//Text
	sf::Text headlineText;
	sf::Text infoText;

	sf::Text statsText;

	sf::RectangleShape infoFrame;

	Inventory_Manager::slotID selectedSlot;
};

class Load_Game_Screen : public Screen
{
public:
	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	Load_Game_Screen(void);
	~Load_Game_Screen(void);
private:
	void initScreen();

	void listAllGameDatas();

	struct gameData
	{
		std::string path;
		std::string name;
		std::string date;
		std::string mapName;

		float maxLive;
		float live;
		int maxEnergy;
		int energy;
		int n_Edelsteine;
	};

	void printNextData(std::list<gameData>::const_iterator newPos);

	std::list<gameData> foundGameDatas;
	std::list<gameData>::const_iterator currentSelectedData;

	sf::Sprite background;
	sf::Texture backTexture;

	sf::RectangleShape mainFrame;

	sf::Text nameText;
	sf::Text dateText;
	sf::Text mapText;
	sf::Text infoText;
	sf::Text headline;

	sf::Text errorText;

	sf::Sprite heart;

	sf::Sprite edelstein;
	sf::Text edelsteineText;

	sf::RectangleShape energyFrame;
	sf::RectangleShape energyShape;

	sf::Texture tex_screenshot;
	sf::Sprite spr_screenshot;

	sf::VertexArray leftTriangle;
	sf::VertexArray rightTriangle;
};

class Logo_Screen : public Screen
{
public:
	Logo_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Logo_Screen(void);
private:

	void initScreen();

	sf::Text presentsText;
	sf::Text CHPText;

	sf::Text contiueText;

	//Logo
	sf::Text the_text;
	sf::Text worst_text;
	sf::Text link_text;
	sf::Text adventure_text;
	sf::Text vorgottenWorld_text;

	sf::Texture tex_sword;
	sf::Sprite  spr_sword;

	sf::Sprite background;
	sf::Texture backTexture;

	sf::Clock screenStateClock;
	short screenState;
};

class Main_Menue : public Screen
{
public:
	Main_Menue(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Main_Menue(void);
private:
	void initScreen();

	int selectedOption;
	int lastSelectedOption;

	sf::Text optionText;
	sf::Text newGameText;
	sf::Text exitText;
	sf::Text loadText;
	sf::Text creditsText;
	sf::Text keyInfo;

	sf::Text gameVersionText;

	sf::RectangleShape textFrame;

	sf::Sprite background;
	sf::Texture backTexture;
};

class New_Game_Screen : public Screen
{
public:
	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	New_Game_Screen(void);
	~New_Game_Screen(void);
private:
	void initScreen();

	void changeDifficultyText();

	sf::Sprite spr_Triforce;
	int triforceMovingLength;
	bool triforceMovingMode;
	sf::Clock triforceAniClock;

	sf::RectangleShape mainFrame;
	sf::RectangleShape nameFrame;

	sf::Text infoText;
	sf::Text nameText;

	sf::Text difficultyText;
	int difficulty;
	sf::RectangleShape difficultyFrame;

};

class Options_Menue : public Screen
{
public:
	Options_Menue();

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	int getVideoModeSetting();

	int getFrameWindowSizeX();
	int getFrameWindowSizeY();

	void setVideoSettings(int windowSizeX,int windowSizeY,short videomode);

	~Options_Menue(void);
private:
	void initScreen();

	sf::RectangleShape mainFrame;

	//Text
	sf::Text headlineText;

	sf::Text soundText;
	sf::Text videoText;
	sf::Text keyText;

	//Linie
	sf::RectangleShape line;

	//Volumebar
	sf::RectangleShape volumeFrame;
	sf::RectangleShape volumeBar;

	//Ausgewählte Option
	short selectedOption;

	int videoMode;
	int windowSizeX;
	int windowSizeY;
};

class Pause_Screen : public Screen
{
public:
	Pause_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Pause_Screen(void);
private:
	void initScreen();

	sf::Text pauseText;
	sf::Text infoText;

	//Midna
	sf::Texture midnaTexture;
	sf::Texture midnaTextureEyesClose;

	sf::Sprite midnaSprite;

	sf::Clock eyeAniClock;
	sf::Clock flyClock;
	
	bool eyes_open;
	bool fadeMode;
	bool flyMode;

	short flyPosY;
	short flyTick;

	short ms_eyeAniWaitTime;

	short midnaPosX;
	short midnaPosY;

	//Edestein
	sf::Sprite jewel;

	short easterEgg;

	sf::Text jewelAmount;
};

class Shop_Screen : public Screen
{
public:
	Shop_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	bool loadMySaveData(std::string data);
	std::string getMySaveData();

	~Shop_Screen(void);
private:
	void initScreen();

	struct shopItem
	{
		int itemNumber;
		int price;
	};

	std::list<shopItem> createItemRow(std::string str_row);

	sf::Text shopHeadline;
	sf::Text shopKeyInfo;
	sf::RectangleShape frame;

	std::list<shopItem> itemRow[3];
	std::list<Shop_Screen::shopItem>::iterator it_list[3];

	sf::Sprite item[3];
	sf::Text money[3];
	short itemRowPosition[3];

	sf::RectangleShape itemFrame;
	sf::RectangleShape moneyframe;

	short selectedRow;
};

class Potion_Shop_Screen : public Screen
{
public:
	Potion_Shop_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Potion_Shop_Screen(void);
private:
	void initScreen();

	sf::Text shopHeadline;
	sf::Text shopKeyInfo;
	sf::RectangleShape frame;

	sf::Sprite item[3];
	sf::Text money[3];

	int price[3];

	sf::RectangleShape itemFrame;
	sf::RectangleShape moneyframe;

	short selectedRow;

	bool showKey;
};

class Quest_Screen : public Screen
{
public:
	Quest_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Quest_Screen(void);
private:
	void initScreen();

	void getQuests();
	void setQuestInfo();

	sf::Text headline;
	sf::Text keyInfo;
	sf::RectangleShape frame;
	sf::RectangleShape desFrame;

	sf::Text questName;
	sf::Text questDescription;
	sf::Text questStatus;

	sf::VertexArray leftTriangle;
	sf::VertexArray rightTriangle;

	std::list<Quest*>::const_iterator questCursor;

	std::list<Quest*>questList;
};

class Game_Stats_Screen : public Screen
{
public:
	Game_Stats_Screen(void);

	void updateScreen();
	void drawScreen(sf::RenderWindow &mainWindow);

	void checkEvents(sf::Event newEvent);

	void loadScreen();
	void restartScreen();
	void closeScreen();

	~Game_Stats_Screen(void);
private:
	void initScreen();

	void getGameStats();

	sf::Text headline;
	sf::Text keyInfo;
	sf::RectangleShape frame;

	sf::Text statsText;
	sf::Text statsInfoText;

	sf::Clock updateClock;
};