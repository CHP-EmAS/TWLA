#include "Screens.h"

#include "Graphic_Manager.h"
#include "Sound_Manager.h"
#include "File_Manager.h"
#include "Player_Manager.h"

Pause_Screen::Pause_Screen(void) : Screen(Screen::Screen_Type::Pause)
{
	initScreen();
}

void Pause_Screen::updateScreen()
{
	if(eyeAniClock.getElapsedTime().asMilliseconds() > ms_eyeAniWaitTime)
	{
		if(eyes_open)
		{
			midnaSprite.setTexture(midnaTextureEyesClose);
			ms_eyeAniWaitTime = 200;
		}
		else
		{
			midnaSprite.setTexture(midnaTexture);
			ms_eyeAniWaitTime = rand() % 3000 + 3000;
		}

		eyes_open = !eyes_open;
		eyeAniClock.restart();
	}

	if(flyClock.getElapsedTime().asMilliseconds() > 5)
	{
		if(fadeMode)
		{
			midnaSprite.setColor(sf::Color(255,255,255,midnaSprite.getColor().a - 1));

			if(midnaSprite.getColor().a <= 50)
			{
				midnaSprite.setColor(sf::Color(255,255,255,50));
				fadeMode = false;
			}
		}
		else
		{
			midnaSprite.setColor(sf::Color(255,255,255,midnaSprite.getColor().a + 1));

			if(midnaSprite.getColor().a >= 255)
			{
				midnaSprite.setColor(sf::Color(255,255,255,255));
				fadeMode = true;
			}
		}

		if(flyTick>5)
		{
			if(flyMode)
			{
				flyPosY+=1;
				if(flyPosY >= 20) flyMode = false;
			}
			else
			{
				flyPosY-=1;
				if(flyPosY <= 0) flyMode = true;
			}

			flyTick = 0;
		}

		flyTick++;

		midnaSprite.setPosition(midnaPosX,midnaPosY-flyPosY);

		flyClock.restart();
	}

	if(easterEgg >= 5)
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			if(midnaPosY > 30) midnaPosY -= 4;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if(midnaPosY < 460) midnaPosY += 4;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			if(midnaPosX > 20) midnaPosX -= 4;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			if(midnaPosX < 520) midnaPosX += 4;
		}

		if(jewel.getGlobalBounds().intersects(midnaSprite.getGlobalBounds()))
		{
			jewel.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,rand() % 5 + 418));
			jewel.setPosition(rand() % 570,rand() % 570);
			Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Get_Rupee);

			std::string str_jewelAmount = jewelAmount.getString();
			int newJewelAmount = atoi(str_jewelAmount.c_str()) + 1;
			
			if(newJewelAmount>999) str_jewelAmount = std::to_string(newJewelAmount);
			else if(newJewelAmount>99) str_jewelAmount = "0" + std::to_string(newJewelAmount);
			else if(newJewelAmount>9) str_jewelAmount = "00" + std::to_string(newJewelAmount);
			else str_jewelAmount = "000" + std::to_string(newJewelAmount);

			jewelAmount.setString(str_jewelAmount);
			jewelAmount.setPosition(WINDOW_SIZE_X/2 - jewelAmount.getGlobalBounds().width / 2, WINDOW_SIZE_Y - jewelAmount.getGlobalBounds().height*2);
		}
	}
}

void Pause_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	if(easterEgg >= 5)
	{
		mainWindow.draw(jewel);
		mainWindow.draw(jewelAmount);
	}
	
	mainWindow.draw(midnaSprite);

	mainWindow.draw(pauseText);
	mainWindow.draw(infoText);
}

void Pause_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			if(screenBeforThis != nullptr)
			{
				Screen_Manager::getMe().changeScreen(screenBeforThis->getScreenType());

				std::string str_jewelAmount = jewelAmount.getString();
				Player_Manager::getMe().getGameHUD().addEdelsteine(atoi(str_jewelAmount.c_str()));
			}
			else
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Main_Menue);
		}

		switch(easterEgg)
		{
		case 0:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::M)) easterEgg++;
			else easterEgg = 0;
			break;
		case 1:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) easterEgg++;
			else easterEgg = 0;
			break;
		case 2:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D)) easterEgg++;
			else easterEgg = 0;
			break;
		case 3:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::N)) easterEgg++;
			else easterEgg = 0;
			break;
		case 4:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A)) easterEgg++;
			else easterEgg = 0;
			break;
		}

		break;
	}
}

void Pause_Screen::loadScreen()
{
	DOUT("")
	IOUT("Pause-Screen wird gestartet")

	Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Paused);
	Sound_Manager::getMe().clearAllEffectSounds();
	Sound_Manager::getMe().playEffectSound(Sound_Manager::Pause_Open);

	midnaPosX = 266;
	midnaPosY = 70;
	isLoaded = true;

	jewel.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,rand() % 5 + 418));
	jewel.setPosition(rand() % 570,rand() % 570);

	jewelAmount.setString("0000");

	easterEgg = 0;
}

void Pause_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Pause_Screen::closeScreen()
{
	Sound_Manager::getMe().playEffectSound(Sound_Manager::Pause_Close);
	Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Playing);

	setLoaded(false);
}

void Pause_Screen::initScreen()
{
	pauseText = sf::Text("--- Pause ---",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),25);
	infoText = sf::Text("Drücken sie ESC zum Fortfahren",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);

	pauseText.setPosition(WINDOW_SIZE_X/2 - pauseText.getGlobalBounds().width/2,250);
	infoText.setPosition(pauseText.getPosition() + sf::Vector2f(-(infoText.getGlobalBounds().width/4),50));

	if(!midnaTexture.loadFromFile("Img/Animations/Midna/Midna eyes opened.png"))
	{
		FOUT("Midna Texture konnte nicht geladen werden!")
	}else
	{
		SOUT("Midna Texture wurde erfolgreich geladen!")
	}

	if(!midnaTextureEyesClose.loadFromFile("Img/Animations/Midna/Midna eyes cloesed.png"))
	{
		FOUT("Midna Texture 2 konnte nicht geladen werden!")
	}else
	{
		SOUT("Midna Texture 2 wurde erfolgreich geladen!")
	}

	midnaPosX = 266;
	midnaPosY = 70;

	flyPosY = 0;
	flyTick = 0;
	flyMode = true;

	eyes_open = true;
	ms_eyeAniWaitTime = rand() % 3000 + 3000;
	
	midnaSprite.setTexture(midnaTexture);
	midnaSprite.setTextureRect(sf::IntRect(31,15,31,57));
	midnaSprite.setScale(2.5,2.5);

	jewel.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	jewel.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,rand() % 5 + 418));
	jewel.setScale(2.5,2.5);
	jewel.setPosition(rand() % 570,rand() % 570);

	jewelAmount = sf::Text("0000",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),25);
	jewelAmount.setFillColor(sf::Color(255,215,0));
	jewelAmount.setPosition(WINDOW_SIZE_X/2 - jewelAmount.getGlobalBounds().width / 2, WINDOW_SIZE_Y - jewelAmount.getGlobalBounds().height*2);

	easterEgg = 0;
}

Pause_Screen::~Pause_Screen(void)
{

}
