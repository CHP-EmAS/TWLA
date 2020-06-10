#include "Screens.h"

#include "Graphic_Manager.h"
#include "File_Manager.h"
#include "Dialog_Manager.h"
#include "Player_Manager.h"
#include "Message_Manager.h"
#include "Defines.h"

New_Game_Screen::New_Game_Screen(void) : Screen(Screen::Screen_Type::Newgame)
{
	initScreen();
}

void New_Game_Screen::updateScreen()
{
	if(triforceAniClock.getElapsedTime().asMilliseconds() > 50)
	{
		if(triforceMovingMode == 0)
		{
			if(triforceMovingLength < 100) triforceMovingLength += 1;
			else triforceMovingMode = 1;
		}
		else
		{
			if(triforceMovingLength > 0) triforceMovingLength -= 1;
			else triforceMovingMode = 0;
		}

		triforceAniClock.restart();
	}

	if(nameFrame.getOutlineColor() == sf::Color(0,200,0) && nameText.getString().getSize() <= 2)
	{
		nameFrame.setOutlineColor(sf::Color(200,0,0));
	}
	else if(nameFrame.getOutlineColor() == sf::Color(200,0,0) && nameText.getString().getSize() > 2)
	{
		nameFrame.setOutlineColor(sf::Color(0,200,0));
	}
}

void New_Game_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	//Triforce
	spr_Triforce.setPosition(WINDOW_SIZE_X/2,210 - triforceMovingLength);
	mainWindow.draw(spr_Triforce);

	spr_Triforce.setPosition((WINDOW_SIZE_X/4) + 85 - triforceMovingLength,330 + triforceMovingLength);
	mainWindow.draw(spr_Triforce);

	spr_Triforce.setPosition((WINDOW_SIZE_X/4*3) - 85 + triforceMovingLength,330 + triforceMovingLength);
	mainWindow.draw(spr_Triforce);

	//Main Frame
	mainWindow.draw(mainFrame);

	//Name Frame
	mainWindow.draw(nameFrame);

	//Name
	mainWindow.draw(nameText);

	//Difficulty
	mainWindow.draw(difficultyText);

	//Info Text
	mainWindow.draw(infoText);
}

void New_Game_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			Screen_Manager::getMe().fadeOut();
			Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
			Screen_Manager::getMe().fadeIn();
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
		{
			if(nameFrame.getOutlineColor() == sf::Color(0,200,0))
			{
				Screen_Manager::getMe().fadeOut();

				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);

				File_Manager::getMe().load_Savegame(File_Manager::getMe().create_SaveGame(nameText.getString(),difficulty));
				File_Manager::getMe().save_Savegame();

				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::None,false);

				Dialog_Manager::getMe().doDialog(4);

				if(Screen_Manager::getMe().getGameWindow().isOpen())
				{
					Dialog_Manager::getMe().doDialog(2);

					Player_Manager::getMe().doSleepAni();

					Message_Manager::getMe().showLocationMsg("W,A,S,D - Bewegen");
				}

				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
				
				Screen_Manager::getMe().fadeIn();
				
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			if(difficulty > 0)
			{
				difficulty--;
				changeDifficultyText();
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if(difficulty < 4)
			{
				difficulty++;
				changeDifficultyText();
			}
		}

		break;
	case sf::Event::TextEntered:
		if(newEvent.text.unicode >= 32 && newEvent.text.unicode <= 122 || newEvent.text.unicode == 124 || newEvent.text.unicode == 126)
		{
			if(nameText.getString().getSize() < 11)
				nameText.setString(nameText.getString() + (char)newEvent.text.unicode);
		}
		else if(newEvent.text.unicode == 8)
		{
			if(nameText.getString().getSize() > 0)
				nameText.setString(nameText.getString().substring(0,nameText.getString().getSize()-1));
		}

		nameText.setPosition(WINDOW_SIZE_X / 2 - nameText.getGlobalBounds().width / 2,WINDOW_SIZE_Y/2- nameText.getGlobalBounds().height / 2);

		break;
	}
}

void New_Game_Screen::loadScreen()
{
	DOUT("")
	IOUT("New-Game-Screen wird gestartet")

	nameText.setString("");
	nameFrame.setOutlineColor(sf::Color(200,0,0));

	setLoaded(true);
}

void New_Game_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void New_Game_Screen::closeScreen()
{
	setLoaded(false);
}

void New_Game_Screen::initScreen()
{
	spr_Triforce.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_Triforce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,379));
	spr_Triforce.setScale(10,10);
	spr_Triforce.setOrigin(8,8);

	triforceMovingLength = 0;
	triforceMovingMode = false;

	mainFrame.setFillColor(sf::Color(100,100,100,75));
	mainFrame.setOutlineColor(sf::Color(100,100,100));
	mainFrame.setOutlineThickness(3);
	mainFrame.setSize(sf::Vector2f(WINDOW_SIZE_X - 50,WINDOW_SIZE_Y - 50));
	mainFrame.setPosition(25,25);

	nameText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),22);
	nameText.setFillColor(sf::Color(255,215,0));
	nameText.setOutlineColor(sf::Color::Black);
	nameText.setOutlineThickness(1);

	nameFrame.setFillColor(sf::Color(0,0,0,75));
	nameFrame.setOutlineColor(sf::Color(200,0,0));
	nameFrame.setOutlineThickness(2);
	nameFrame.setSize(sf::Vector2f(350,50));
	nameFrame.setPosition(WINDOW_SIZE_X/2 - nameFrame.getGlobalBounds().width/2+5,WINDOW_SIZE_Y/2 - nameFrame.getGlobalBounds().height/2 + 10);

	infoText = sf::Text("Ein gutes Abenteuer beginnt\n   mit einem guten Namen\n\n\n\n\n\n   Bitte Namen eingeben:\n\n\n\n\n    Schwierigkeitsgrad:\n\n\n       ESC - Zurück\n     ENTER - Beginnen\n   Auswahl - Pfeiltasten",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),22);
	infoText.setPosition(WINDOW_SIZE_X/2 - infoText.getGlobalBounds().width/2 + 5,100);
	infoText.setOutlineColor(sf::Color::Black);
	infoText.setOutlineThickness(2);

	//difficulty
	difficultyText = sf::Text("§ Normal ²",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),22);
	difficultyText.setPosition(WINDOW_SIZE_X/2 - difficultyText.getGlobalBounds().width/2 + 5,WINDOW_SIZE_Y/2 + 75);
	difficultyText.setOutlineColor(sf::Color::Black);
	difficultyText.setOutlineThickness(2);

	difficulty = NORMAL;
	changeDifficultyText();

	difficultyFrame.setFillColor(sf::Color(0,0,0,75));
	difficultyFrame.setOutlineColor(sf::Color(200,0,0));
	difficultyFrame.setOutlineThickness(2);
	difficultyFrame.setSize(sf::Vector2f(350,50));
	difficultyFrame.setPosition(WINDOW_SIZE_X/2 - nameFrame.getGlobalBounds().width/2+5,WINDOW_SIZE_Y/2 - nameFrame.getGlobalBounds().height/2 + 10);
}

void New_Game_Screen::changeDifficultyText()
{
	switch(difficulty)
	{
	case IMMORTAL:
		difficultyText.setString("Unsterblich ²");
		difficultyText.setFillColor(sf::Color(0,166,255));
		break;
	case EASY:
		difficultyText.setString("§ Einfach ²");
		difficultyText.setFillColor(sf::Color(66,255,3));
		break;
	case NORMAL:
		difficultyText.setString("§ Normal ²");
		difficultyText.setFillColor(sf::Color(255,184,3));
		break;
	case DIFFICULT:
		difficultyText.setString("§ Schwer ²");
		difficultyText.setFillColor(sf::Color::Red);
		break;
	case HARDCORE:
		difficultyText.setString("§ Hardcore");
		difficultyText.setFillColor(sf::Color(99,0,0));
		break;
	default:
		difficultyText.setString("%ERROR%");
		difficultyText.setFillColor(sf::Color::White);
		FOUT("Schwierigkeitsstufe <" << difficulty << "> konnte keiner Schwierigkeit zugeordnet werden!")
		break;
	}

	difficultyText.setPosition(WINDOW_SIZE_X/2 - difficultyText.getGlobalBounds().width/2 + 5,WINDOW_SIZE_Y/2 + 75);
}

New_Game_Screen::~New_Game_Screen(void)
{

}
