#include "Screens.h"

#include "Graphic_Manager.h"
#include "Screen_Manager.h"
#include "File_Manager.h"
#include "Dialog_Manager.h"
#include "Sound_Manager.h"

Main_Menue::Main_Menue() : Screen(Screen::Screen_Type::Main_Menue)
{
	initScreen();
}

void Main_Menue::updateScreen()
{
	if(lastSelectedOption != selectedOption)
	{
		lastSelectedOption = selectedOption;

		newGameText.setFillColor(sf::Color::White);
		loadText.setFillColor(sf::Color::White);
		optionText.setFillColor(sf::Color::White);
		creditsText.setFillColor(sf::Color::White);
		exitText.setFillColor(sf::Color::White);

		switch(selectedOption)
		{
		case 0:
			newGameText.setFillColor(sf::Color::Yellow);
			break;
		case 1:
			loadText.setFillColor(sf::Color::Yellow);
			break;
		case 2:
			optionText.setFillColor(sf::Color::Yellow);
			break;
		case 3:
			creditsText.setFillColor(sf::Color::Yellow);
			break;
		case 4: 
			exitText.setFillColor(sf::Color::Yellow);
			break;
		}
	}
}

void Main_Menue::drawScreen(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(background);
	mainWindow.draw(textFrame);

	mainWindow.draw(newGameText);
	mainWindow.draw(loadText);
	mainWindow.draw(optionText);
	mainWindow.draw(creditsText);
	mainWindow.draw(exitText);
	mainWindow.draw(keyInfo);

	mainWindow.draw(gameVersionText);
}

void Main_Menue::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			if(selectedOption > 0) selectedOption--;
			else selectedOption = 4;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if(selectedOption < 4) selectedOption++;
			else selectedOption = 0;
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
		{
			if(selectedOption >= 0 && selectedOption <= 4)
			{
				Screen_Manager::getMe().fadeOut();
			}
	
			switch(selectedOption)
			{
			case 0:
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Newgame,false);
				break;
			case 1:
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Savegame_Selector,false);
				break;
			case 2:
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Options,false);
				break;
			case 3:
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Credits,false);
				break;
			case 4:
				Screen_Manager::getMe().getGameWindow().close();
				break;
			}

			if(selectedOption >= 0 && selectedOption <= 3)
			{
				Screen_Manager::getMe().fadeIn();
			}
		}
	}
}

void Main_Menue::loadScreen()
{
	DOUT("")
	IOUT("Menü-Screen wird gestartet")

	selectedOption = 0;
	lastSelectedOption = -1;
	setLoaded(true);
}

void Main_Menue::restartScreen()
{
	loadScreen();
}

void Main_Menue::closeScreen()
{
	setLoaded(false);
}

void Main_Menue::initScreen()
{
	newGameText = sf::Text("Neues Spiel starten",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),25);
	loadText = sf::Text("Spiel laden",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),25);
	optionText = sf::Text("Einstellungen",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),25);
	creditsText = sf::Text("Kredits",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),25);
	exitText = sf::Text("Beenden",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),25);

	newGameText.setPosition(WINDOW_SIZE_X/2 - newGameText.getGlobalBounds().width/2,150);
	loadText.setPosition(WINDOW_SIZE_X/2 - loadText.getGlobalBounds().width/2,195);
	optionText.setPosition(WINDOW_SIZE_X/2 - optionText.getGlobalBounds().width/2,240);
	creditsText.setPosition(WINDOW_SIZE_X/2 - creditsText.getGlobalBounds().width/2,285);
	exitText.setPosition(WINDOW_SIZE_X/2 - exitText.getGlobalBounds().width/2,330);
				
	keyInfo = sf::Text("Pfeiltasten -     auswählen\nEnter       -    bestätigen",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	keyInfo.setOrigin(keyInfo.getGlobalBounds().width/2,keyInfo.getGlobalBounds().height/2);
	keyInfo.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4*2.8);
	keyInfo.setOutlineColor(sf::Color::Black);
	keyInfo.setOutlineThickness(2);

	std::string gameVersion = "Ver ";
	gameVersion += GAME_VERSION;

	gameVersionText = sf::Text(gameVersion,Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),14);
	gameVersionText.setPosition(2,WINDOW_SIZE_Y-gameVersionText.getGlobalBounds().height-5);

	selectedOption = 0;
	lastSelectedOption = -1;

	backTexture.loadFromFile("Img/Backgrounds/1.png");

	background.setTexture(backTexture);
	background.setPosition(0,0);

	textFrame.setFillColor(sf::Color(75,75,75,220));
	textFrame.setOutlineThickness(3);
	textFrame.setOutlineColor(sf::Color(50,50,50));
	textFrame.setSize(sf::Vector2f(265,240));
	textFrame.setPosition(WINDOW_SIZE_X/2 - textFrame.getSize().x/2,136);
}

Main_Menue::~Main_Menue(void)
{
	
}
