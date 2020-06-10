#include "Screens.h"

#include "Defines.h"

#include "Graphic_Manager.h"
#include "Sound_Manager.h"

Game_Over_Screen::Game_Over_Screen(void) : Screen(Screen::Screen_Type::Gameover)
{
	initScreen();
	setSetting = false;
}

void Game_Over_Screen::updateScreen()
{
	if(fadeTextClock.getElapsedTime().asMilliseconds() > 10)
	{
		if(fadeAlpha < 255)
		{
			fadeAlpha++;

			gameoverText.setFillColor(sf::Color(170,0,0,fadeAlpha));
			infoText.setFillColor(sf::Color(255,255,255,fadeAlpha));
			grabstein.setColor(sf::Color(255,255,255,fadeAlpha));
		}

		fadeTextClock.restart();
	}

	if(!setSetting)
	{
		setSetting = true;

		Sound_Manager::getMe().playEffectSound(Sound_Manager::Game_Over);
		Sound_Manager::getMe().changeBackgroundMusic("Music/Nyan Cat.ogg");
		Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Status::Paused);
	}
}

void Game_Over_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(grabstein);
	mainWindow.draw(gameoverText);
	mainWindow.draw(infoText);
}

void Game_Over_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			if(screenBeforThis != nullptr)
			Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete))
		{
			if(!musicIsPlaying)
			{
				Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Status::Playing);
				musicIsPlaying = true;
			}

			grabstein.setPosition(rand()%WINDOW_SIZE_X - 20,rand()%WINDOW_SIZE_Y - 20);

			switch(rand() % 8)
			{
			case 0:
				grabstein.setColor(sf::Color(10,10,10));
				break;
			case 1:
				grabstein.setColor(sf::Color(200,0,0));
				break;
			case 2:
				grabstein.setColor(sf::Color(0,200,0));
				break;
			case 3:
				grabstein.setColor(sf::Color(0,0,200));
				break;
			case 4:
				grabstein.setColor(sf::Color(200,0,200));
				break;
			case 5:
				grabstein.setColor(sf::Color(0,200,200));
				break;
			case 6:
				grabstein.setColor(sf::Color(200,200,0));
				break;
			case 7:
				grabstein.setColor(sf::Color(255,255,255));
				break;
			}
		}

		switch(easterEgg)
		{
		case 0:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) easterEgg++;
			else easterEgg = 0;
			break;
		case 1:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) easterEgg++;
			else easterEgg = 0;
			break;
		case 2:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) easterEgg++;
			else easterEgg = 0;
			break;
		case 3:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::E)) easterEgg++;
			else easterEgg = 0;
			break;
		case 4:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R)) easterEgg++;
			else easterEgg = 0;
			break;
		case 5:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::T)) easterEgg++;
			else easterEgg = 0;
			break;
		case 6:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) easterEgg++;
			else easterEgg = 0;
			break;
		case 7:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C)) easterEgg++;
			else easterEgg = 0;
			break;
		case 8:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::O)) easterEgg++;
			else easterEgg = 0;
			break;
		case 9:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) easterEgg++;
			else easterEgg = 0;
			break;
		case 10:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) easterEgg++;
			else easterEgg = 0;
			break;
		}
		break;
	case sf::Event::KeyReleased:

		if(!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete))
		{
			if(musicIsPlaying)
			{
				Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Status::Paused);
				grabstein.setPosition(WINDOW_SIZE_X/2 - grabstein.getGlobalBounds().width/2,100);
				grabstein.setColor(sf::Color(255,255,255));

				musicIsPlaying = false;
			}
		}

		break;
	}
}

void Game_Over_Screen::loadScreen()
{
	DOUT("")
	IOUT("Gameover-Screen wird gestartet")
	DOUT("")

	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Der Gameover-Screen kann nur bei geladenem Spiel geöffnet werden!");

		if(screenBeforThis != nullptr)
			Screen_Manager::getMe().changeScreen(screenBeforThis->getScreenType(),true);
		else
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Main_Menue,true);

		return;
	}
	
	isLoaded = true;

	fadeAlpha = 0;
	gameoverText.setFillColor(sf::Color(210,0,0,fadeAlpha));
	infoText.setFillColor(sf::Color(255,255,255,fadeAlpha));
	grabstein.setColor(sf::Color(255,255,255,fadeAlpha));

	easterEgg = 0;
	musicIsPlaying = false;
	setSetting = false;
	grabstein.setPosition(WINDOW_SIZE_X/2 - grabstein.getGlobalBounds().width/2,100);
}

void Game_Over_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Game_Over_Screen::closeScreen()
{
	isLoaded = false;

	Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Stopped);
}

void Game_Over_Screen::initScreen()
{
	gameoverText = sf::Text("--- Game Over ---",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),25);
	infoText = sf::Text("Drücken sie ESC um ins Hauptmenü zu gelangen",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),16);

	gameoverText.setPosition(WINDOW_SIZE_X/2 - gameoverText.getGlobalBounds().width/2,270);
	infoText.setPosition(gameoverText.getPosition().x + gameoverText.getGlobalBounds().width / 2 - infoText.getGlobalBounds().width/2,gameoverText.getPosition().y + 55);

	gameoverText.setFillColor(sf::Color::Red);

	grabstein.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::tileset::Overworld));
	grabstein.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::Overworld,103));
	grabstein.setScale(7,7);
	grabstein.setPosition(WINDOW_SIZE_X/2 - grabstein.getGlobalBounds().width/2,100);

	fadeAlpha = 0;

	easterEgg = 0;
	musicIsPlaying = false;
	setSetting = false;
}

Game_Over_Screen::~Game_Over_Screen(void)
{

}
