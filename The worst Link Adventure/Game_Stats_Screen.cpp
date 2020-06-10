#include "Screens.h"

#include "Screen_Manager.h"
#include "Graphic_Manager.h"
#include "Quest_Manager.h"
#include "Message_Manager.h"
#include "Player_Manager.h"

Game_Stats_Screen::Game_Stats_Screen(void):Screen(Screen_Type::Game_Stats)
{
	initScreen();
}

void Game_Stats_Screen::updateScreen()
{
	if(updateClock.getElapsedTime().asMilliseconds() > 500)
	{
		Player_Manager::getMe().getGameStats().update();
		getGameStats();
		updateClock.restart();
	}
}

void Game_Stats_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

	mainWindow.draw(frame);

	mainWindow.draw(headline);
	mainWindow.draw(keyInfo);

	mainWindow.draw(statsInfoText);
	mainWindow.draw(statsText);
}

void Game_Stats_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
		}
	}
}

void Game_Stats_Screen::loadScreen()
{
	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Das Statistik-Menü kann nur bei geladenem Spiel geöffnet werden!");
		Screen_Manager::getMe().changeScreen(Main_Menue);
	}

	getGameStats();

	setLoaded(true);
}

void Game_Stats_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Game_Stats_Screen::closeScreen()
{
	setLoaded(false);
}

void Game_Stats_Screen::getGameStats()
{
	statsText.setString(Player_Manager::getMe().getGameStats().getStatsString());
	statsText.setPosition((WINDOW_SIZE_X-120)-statsText.getGlobalBounds().width,WINDOW_SIZE_Y/3+10);
}

void Game_Stats_Screen::initScreen()
{
	headline = sf::Text("-- Spiel Statistik --",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),27);
	headline.setFillColor(sf::Color(150,150,150));
	headline.setOrigin(headline.getGlobalBounds().width/2,headline.getGlobalBounds().height/2);
	headline.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4-headline.getGlobalBounds().height-30);
	headline.setOutlineColor(sf::Color::Black);
	headline.setOutlineThickness(2);
	
	keyInfo = sf::Text("Escape - Zurück",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	keyInfo.setOrigin(keyInfo.getGlobalBounds().width/2,keyInfo.getGlobalBounds().height/2);
	keyInfo.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4*3.2);
	keyInfo.setOutlineColor(sf::Color::Black);
	keyInfo.setOutlineThickness(2);

	frame.setSize(sf::Vector2f(WINDOW_SIZE_X/1.5, WINDOW_SIZE_Y/3));
	frame.setFillColor(sf::Color(75,75,75,220));
	frame.setOutlineThickness(3);
	frame.setOutlineColor(sf::Color(50,50,50));
	frame.setOrigin(frame.getGlobalBounds().width/2,frame.getGlobalBounds().height/2);
	frame.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2);

	statsText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);
	statsText.setPosition(WINDOW_SIZE_X/1.5-20,WINDOW_SIZE_Y/3+10);
	statsText.setOutlineColor(sf::Color::Black);
	statsText.setFillColor(sf::Color::Yellow);
	statsText.setOutlineThickness(1);

	statsInfoText = sf::Text("Spielzeit\nSchritte im Spiel\nPfeile geschossen\nBomben gezündet\nSchwerthiebe\nMonster getötet\nGras zerschnitten\nHerzteile gefunden\nQuests erledigt",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);
	statsInfoText.setPosition(WINDOW_SIZE_X/5-10,WINDOW_SIZE_Y/3+10);
	statsInfoText.setOutlineColor(sf::Color::Black);
	statsInfoText.setOutlineThickness(1);
}

Game_Stats_Screen::~Game_Stats_Screen(void)
{
}