#pragma once

#include "SFML/Graphics.hpp"

#define SCREEN_AMOUNT 16

class Screen
{
public:
	enum Screen_Type
	{
		Main_Menue,
		Game,
		Inventory,
		Pause,
		Options,
		Credits,
		Savegame_Selector,
		Newgame,
		Gameover,
		Flute,
		Shop,
		Bomb_Shop,
		Potion_Shop,
		Logos,
		Quests,
		Game_Stats,
		None
	};

	Screen(Screen_Type type);

	virtual void updateScreen() = 0;
	virtual void drawScreen(sf::RenderWindow &mainWindow) = 0;

	virtual void checkEvents(sf::Event newEvent) = 0;

	virtual void loadScreen() = 0;
	virtual void restartScreen() = 0;
	virtual void closeScreen() = 0;

	void setLastScreen(Screen* lastScreen);
	Screen* getLastScreen();

	void setLoaded(bool load);
	bool isScreenLoaded();

	Screen::Screen_Type getScreenType();

	virtual ~Screen(void);
protected:
	virtual void initScreen() = 0;

	bool isLoaded;
	Screen_Type type;
	Screen* screenBeforThis;
};

