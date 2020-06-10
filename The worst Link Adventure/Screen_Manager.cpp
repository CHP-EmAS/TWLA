#include "Screen_Manager.h"

#include <sstream>

#include "Screens.h"

#include "File_Manager.h"
#include "Animation_Manager.h"

Screen_Manager::Screen_Manager(void)
{
	Singleton::instanz = this;

	gameWindow = nullptr;

	windowIcon.loadFromFile("Img/ect/schwert.png");

	setWindowParameter(WINDOW_SIZE_X,WINDOW_SIZE_Y,true,false);

	//fading
	fadeAlpha = 0;
	fadeRectangle.setSize(sf::Vector2f(WINDOW_SIZE_X+2,WINDOW_SIZE_Y+2));
	fadeRectangle.setPosition(-1,-1);
	fadeRectangle.setFillColor(sf::Color::Black);

	allScreens[Screen::Game]			  = nullptr;
	allScreens[Screen::Main_Menue]		  = nullptr;
	allScreens[Screen::Inventory]		  = nullptr;
	allScreens[Screen::Options]			  = nullptr;
	allScreens[Screen::Pause]			  = nullptr;
	allScreens[Screen::Credits]			  = nullptr;
	allScreens[Screen::Gameover]		  = nullptr;
	allScreens[Screen::Newgame]			  = nullptr;
	allScreens[Screen::Savegame_Selector] = nullptr;
	allScreens[Screen::Flute]             = nullptr;
	allScreens[Screen::Logos]             = nullptr;
	allScreens[Screen::Shop]			  = nullptr;
	allScreens[Screen::Bomb_Shop]		  = nullptr;
	allScreens[Screen::Potion_Shop]		  = nullptr;
	allScreens[Screen::Quests]		      = nullptr;
	allScreens[Screen::Game_Stats]		  = nullptr;

	gameFpsLimit = 60;
	showParticle = true;

	secureCloseScreen = Screen::None;

	//Effect
	drawEffect = false;
	currentEffect = Rainbow;
	effectUpdateTimeMs = 5;

	drugEffect = sf::VertexArray(sf::PrimitiveType::Quads, 4);
	drugEffect[0].position = sf::Vector2f(-1,-1);
	drugEffect[1].position = sf::Vector2f(-1,WINDOW_SIZE_Y+1);
	drugEffect[2].position = sf::Vector2f(WINDOW_SIZE_X+1,WINDOW_SIZE_Y+1);
	drugEffect[3].position = sf::Vector2f(WINDOW_SIZE_X+1,-1);

	drugEffect[0].color = sf::Color(255,0,0,150);
	drugEffect[1].color = sf::Color(0,255,0,150);
	drugEffect[2].color = sf::Color(0,0,255,150);
	drugEffect[3].color = sf::Color(255,255,0,150);

	tex_heart.loadFromFile("Img/ect/herz-screen.png");
	spr_heart.setTexture(tex_heart);
	spr_heart.setOrigin(tex_heart.getSize().x/2,tex_heart.getSize().y/2);
	spr_heart.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2);
	spr_heart.setScale(10,10);

	NEWOUT("Neuer Screen_Manager wurde erstellt! (" << this << ")");
}

void Screen_Manager::initScreens(bool debugMode)
{
	showDebugConsole = debugMode;

	deleteScreens(); 

	allScreens[Screen::Game]              = new Game_Screen();
	allScreens[Screen::Main_Menue]        = new Main_Menue();
	allScreens[Screen::Inventory]         = new Inventory_Screen();
	allScreens[Screen::Options]           = new Options_Menue();
	allScreens[Screen::Pause]             = new Pause_Screen();
	allScreens[Screen::Credits]           = new Credits_Screen();
	allScreens[Screen::Gameover]          = new Game_Over_Screen();
	allScreens[Screen::Newgame]           = new New_Game_Screen();
	allScreens[Screen::Savegame_Selector] = new Load_Game_Screen();
	allScreens[Screen::Flute]			  = new Flute_Screen();
	allScreens[Screen::Logos]			  = new Logo_Screen();
	allScreens[Screen::Shop]			  = new Shop_Screen();
	allScreens[Screen::Bomb_Shop]		  = new Bomb_Shop_Screen();
	allScreens[Screen::Potion_Shop]		  = new Potion_Shop_Screen();
	allScreens[Screen::Quests]		      = new Quest_Screen();
	allScreens[Screen::Game_Stats]		  = new Game_Stats_Screen();

	activScreen = nullptr;
}

void Screen_Manager::deleteScreens()
{
	for(int i_screen = 0; i_screen < SCREEN_AMOUNT; i_screen++)
	{
		if(allScreens[Screen::Screen_Type(i_screen)] != nullptr)
		{
			allScreens[Screen::Screen_Type(i_screen)]->closeScreen();
			delete allScreens[Screen::Screen_Type(i_screen)];
			allScreens[Screen::Screen_Type(i_screen)] = nullptr;
		}
	}
}

void Screen_Manager::changeScreen(Screen::Screen_Type newScreen, bool closeOldScreen)
{
	if(!(int(newScreen) >= 0 && int(newScreen) <= SCREEN_AMOUNT))
	{
		FOUT("Screen-ID <" << newScreen << "> ist nicht bekannt!");
		return;
	}

	if(closeOldScreen)
	{
		if(activScreen != nullptr) doSecureClose(activScreen->getScreenType());
	}

	if(newScreen != Screen::None)
	{
		allScreens[Screen::Screen_Type(newScreen)]->setLastScreen(activScreen);
		activScreen = allScreens[Screen::Screen_Type(newScreen)];
	}
	else
	{
		activScreen = nullptr;
	}

	if(activScreen != nullptr)
	{
		if(!activScreen->isScreenLoaded())
		{
			activScreen->loadScreen();
		}
	}
}

void Screen_Manager::doSecureClose(Screen::Screen_Type screen)
{
	if(!(int(screen) >= 0 && int(screen) < SCREEN_AMOUNT))
	{
		FOUT("Screen-ID <" << screen << "> ist nicht bekannt!");
		return;
	}

	secureCloseScreen = screen;
}

void Screen_Manager::drawActivScreen()
{
	gameWindow->clear();

	activScreen->drawScreen(*gameWindow);

	//effect
	if(drawEffect)
	{
		switch(currentEffect)
		{
		case Rainbow:
			gameWindow->draw(drugEffect);
			break;
		case Heart:
			gameWindow->draw(spr_heart);
			break;
		}
	}

	//draw fade Rectangle
	if(fadeAlpha > 0) gameWindow->draw(fadeRectangle);

	gameWindow->display();
}

void Screen_Manager::updateActivScreen()
{
	//Sicheres löschen
	if(secureCloseScreen != Screen::Screen_Type::None)
	{
		if(secureCloseScreen != activScreen->getScreenType())
		{
			if(getScreen(secureCloseScreen)->isScreenLoaded())
			{
				getScreen(secureCloseScreen)->closeScreen();
				SOUT("Der Screen <" << secureCloseScreen << "> wurde sicher gelöscht!");
				secureCloseScreen = Screen::Screen_Type::None;
			}
			else
			{
				WOUT("Der Screen <" << secureCloseScreen << "> ist bereits gelöscht!");
				secureCloseScreen = Screen::Screen_Type::None;
			}
		}
		else
		{
			FOUT("Der Screen <" << secureCloseScreen << "> konnte nicht sicher gelöscht werden, da dieser momentan ausgeführt wird!");
			secureCloseScreen = Screen::Screen_Type::None;
		}
	}

	//Update des Activen Screens
	activScreen->updateScreen();

	//Update Effekte
	if(drawEffect)
	{
		if(effectClock.getElapsedTime().asMilliseconds() > effectUpdateTimeMs)
		{
			switch(currentEffect)
			{
			case Rainbow:
			{
				if(rand()%2 == 1)
					drugEffect[0].color = Animation_Manager::getMe().colorChange(drugEffect[0].color,5);

				if(rand()%2 == 1)
					drugEffect[1].color = Animation_Manager::getMe().colorChange(drugEffect[1].color,5);

				if(rand()%2 == 1)
					drugEffect[2].color = Animation_Manager::getMe().colorChange(drugEffect[2].color,5);

				if(rand()%2 == 1)
					drugEffect[3].color = Animation_Manager::getMe().colorChange(drugEffect[3].color,5);

				break;
			}
			case Heart:
			{
				if(spr_heart.getScale().x > 0.9  )
				{ 
					spr_heart.setScale(spr_heart.getScale().x-0.1,spr_heart.getScale().y-0.1);
				}
				else
				{
					spr_heart.setScale(0.8,0.8);
				}
				break;
			}
			}

			effectClock.restart();
		}
	}
}

void Screen_Manager::checkWindowEvents()
{
	sf::Event events;

	while(gameWindow->pollEvent(events))
	{
		activScreen->checkEvents(events);
		
		switch(events.type)
		{
		case sf::Event::Closed:
			gameWindow->close();
			break;
		case sf::Event::KeyPressed:
		{
			//Screenshot
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) //F1
			{
				#pragma warning(disable:4996)

				time_t t = time(0);
				struct tm* now = localtime(&t);
		
				//Map Header speichern
				std::string filename = "Screenshots/screenshot ";

				filename += std::to_string(now->tm_mday) + "_";
				filename += std::to_string(now->tm_mon+1) + "_";
				filename += std::to_string(now->tm_year + 1900) + " # ";
				filename += std::to_string(now->tm_hour) + "-";
				filename += std::to_string(now->tm_min) + "-";
				filename += std::to_string(now->tm_sec) + ".png";

				sf::Image screenshotImage;
				screenshotImage = gameWindow->capture();
				screenshotImage.saveToFile(filename);

				#pragma warning(default:4996)
			}

			break;
		}
		}
	}
}

void Screen_Manager::updateWindowSize()
{
	if(gameWindow != nullptr) delete gameWindow;

	gameWindow = new sf::RenderWindow;

	if(fullscreen)
	{
		gameWindow->create(sf::VideoMode(WINDOW_SIZE_X,WINDOW_SIZE_Y),"The worst Link Adventure!",sf::Style::Fullscreen);
	}
	else if(border)
	{
		gameWindow->create(sf::VideoMode(sizeX,sizeY),"The worst Link Adventure!",sf::Style::Close);
		gameWindow->setSize(sf::Vector2u(sizeX,sizeY));
	}
	else
	{
		gameWindow->create(sf::VideoMode(sizeX,sizeY),"The worst Link Adventure!",sf::Style::None);
		gameWindow->setSize(sf::Vector2u(sizeX,sizeY));
	}

	windowHandle = gameWindow->getSystemHandle();

	float tempSizeX = gameWindow->getSize().x;
	float tempSizeY = gameWindow->getSize().y;

	float percentX = tempSizeY/tempSizeX;
	float posPercentX =  ((tempSizeX-tempSizeY)/2.f)/tempSizeX;

	windowView.setCenter(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2);
	windowView.setSize(WINDOW_SIZE_X,WINDOW_SIZE_Y);
	windowView.setViewport(sf::FloatRect(posPercentX,0, percentX, 1));
	
	gameWindow->setView(windowView);
	
	gameWindow->setIcon(32,32,windowIcon.getPixelsPtr());
	gameWindow->setMouseCursorVisible(false);
}

void Screen_Manager::setWindowParameter(int width,int height,bool border,bool fullscreen)
{
	this->sizeX = width;
	this->sizeY = height;
	this->border = border;
	this->fullscreen = fullscreen;

	updateWindowSize();
}

void Screen_Manager::fadeOut(sf::Color fadeColor)
{
	fadeClock.restart();

	fadeColor.a = 0;
	fadeRectangle.setFillColor(fadeColor);
	
	while(fadeAlpha < 255)
	{
		if(fadeClock.getElapsedTime().asMilliseconds() > 1)
		{
			drawActivScreen();

			fadeColor.a = ++fadeAlpha;
			fadeRectangle.setFillColor(fadeColor);

			fadeClock.restart();
		}
	}
}

void Screen_Manager::fadeIn(sf::Color fadeColor)
{
	fadeClock.restart();

	fadeColor.a = 255;
	fadeRectangle.setFillColor(fadeColor);

	while(fadeAlpha > 0)
	{
		if(fadeClock.getElapsedTime().asMilliseconds() > 1)
		{
			drawActivScreen();

			fadeColor.a = --fadeAlpha;
			fadeRectangle.setFillColor(fadeColor);

			fadeClock.restart();
		}
	}
}

sf::RenderWindow &Screen_Manager::getGameWindow()
{
	return *gameWindow;
}

HWND &Screen_Manager::getGameWindowHandle()
{
	return windowHandle;
}

Screen* Screen_Manager::getLastScreen()
{
	return activScreen->getLastScreen();
}

Screen* Screen_Manager::getActivScreen()
{
	return activScreen;
}

Screen* Screen_Manager::getScreen(Screen::Screen_Type screenType)
{
	if(int(screenType) >= 0 && int(screenType) < SCREEN_AMOUNT)
	{
		return allScreens[screenType];
	}
	else
	{
		FOUT("Screen-ID <" << screenType << "> ist nicht bekannt!");
		return allScreens[Screen::Main_Menue];
	}
}

float Screen_Manager::getActivScreenFPSLimit()
{
	switch(activScreen->getScreenType())
	{
	case Screen::Screen_Type::Game:
		return gameFpsLimit;
	case Screen::Screen_Type::Pause:
	case Screen::Screen_Type::Newgame:
	case Screen::Screen_Type::Flute:
		return 60;
	default:
		return 30;
	}
}

float Screen_Manager::getActivScreenUpdateLimit()
{
	switch(activScreen->getScreenType())
	{
	case Screen::Screen_Type::Game:
		return 2000;
	default:
		return 500;
	}
}

void Screen_Manager::setGameFPSLimit(int fpsLimit)
{
	if(fpsLimit >= 30 && fpsLimit <= 500) gameFpsLimit = fpsLimit;
	else if(fpsLimit == 501)
	{
		gameFpsLimit = 1000000;
	}
	else
	{
		WOUT("FPS-Limit konnte nicht auf " << fpsLimit << " gesetzt werden!")
	}
}

int Screen_Manager::getGameFPSLimit()
{
	return gameFpsLimit;
}

bool Screen_Manager::debugMode()
{
	return showDebugConsole;
}

void Screen_Manager::doEffect(Effect effect)
{
	drawEffect = true;
	currentEffect = effect;

	switch(effect)
	{
	case Heart:
		spr_heart.setScale(10,10);
		effectUpdateTimeMs = 30;
		break;
	case Rainbow:
		effectUpdateTimeMs = 2;
		break;
	}
}

void Screen_Manager::stopEffect()
{
	drawEffect = false;
}

void Screen_Manager::setShowParticle(bool show)
{
	showParticle = show;
}

bool Screen_Manager::getShowParticle()
{
	return showParticle;
}

Screen_Manager::~Screen_Manager(void)
{
	deleteScreens();
}
