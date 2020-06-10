#pragma once

#include "SFML/Graphics.hpp"

#include "Defines.h"
#include "Screen.h"


class Screen_Manager : public Singleton<Screen_Manager>
{

friend class Singleton<Screen_Manager>;

public:
	void initScreens(bool debugMode);
	void deleteScreens();

	void changeScreen(Screen::Screen_Type newScreen, bool closeOldScreen = true);
	void doSecureClose(Screen::Screen_Type screen);

	void drawActivScreen();
	void updateActivScreen();
	void checkWindowEvents();

	void updateWindowSize();
	void setWindowParameter(int width,int height,bool border,bool fullscreen);

	void fadeOut(sf::Color fadeColor = sf::Color::Black);
	void fadeIn(sf::Color fadeColor = sf::Color::Black);

	sf::RenderWindow &getGameWindow();
	HWND &getGameWindowHandle();

	Screen* getLastScreen();
	Screen* getActivScreen();
	Screen* getScreen(Screen::Screen_Type screenType);

	float getActivScreenFPSLimit();
	float getActivScreenUpdateLimit();

	void setGameFPSLimit(int fpsLimit);
	int getGameFPSLimit();

	bool debugMode();

	enum Effect
	{
		Rainbow,
		Heart
	};

	void doEffect(Effect effect);
	void stopEffect();

	void setShowParticle(bool show);
	bool getShowParticle();

	~Screen_Manager(void);
private:
	Screen_Manager(void);

	//Screens
	Screen* activScreen;
	
	Screen* allScreens[SCREEN_AMOUNT];

	Screen::Screen_Type secureCloseScreen;

	//Window
	sf::RenderWindow *gameWindow;
	sf::Image windowIcon;
	sf::View windowView;

	HWND windowHandle;

	int sizeX;
	int sizeY;
	bool fullscreen;
	bool border;

	//fade
	sf::Clock fadeClock;
	int fadeAlpha;
	sf::RectangleShape fadeRectangle;

	bool showDebugConsole;
	bool showParticle;

	//FPS
	int gameFpsLimit;

	//Screen Effekt
	bool drawEffect;
	Effect currentEffect;
	sf::Clock effectClock;
	int effectUpdateTimeMs;

	sf::VertexArray drugEffect;
	sf::Texture tex_heart;
	sf::Sprite spr_heart;
};

