#include "Screens.h"

#include "Graphic_Manager.h"
#include "Screen_Manager.h"
#include "Sound_Manager.h"

Credits_Screen::Credits_Screen(void) : Screen(Screen::Screen_Type::Credits)
{
	initScreen();
}

void Credits_Screen::updateScreen()
{
	if(easterEgg == 4)Screen_Manager::getMe().changeScreen(Screen::Main_Menue);

	if(textMoveClock.getElapsedTime().asMilliseconds() >= waitTime)
	{
		if(!showPicture)
		{
			if(titleText.getPosition().y + creditSize / 2 > WINDOW_SIZE_Y / 2)
			{
				titleText.setPosition(titleText.getPosition() - sf::Vector2f(0,1));

				for(auto cursor : nameTexts)
				{
					cursor->setPosition(cursor->getPosition() - sf::Vector2f(0,1));
				}
	
				if(!(titleText.getPosition().y + creditSize / 2 > WINDOW_SIZE_Y / 2))
				{
					waitTime = 4750;
				}
			}
			else if(titleText.getPosition().y + creditSize > 0)
			{
				waitTime = 9;

				titleText.setPosition(titleText.getPosition() - sf::Vector2f(0,1));

				for(auto cursor : nameTexts)
				{
					cursor->setPosition(cursor->getPosition() - sf::Vector2f(0,1));
				}
			}
			else
			{
				if(textList.size()>0)
				{
					creditIterator++;

					if(creditIterator != textList.end())
						nextCredit(creditIterator);
					else
					{
						waitTime = 25;
						showPicture = true;
					}
				}
				else
				{
					Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
				}
			}
		}
		else
		{
			if(eponaSprite.getColor().a < 255)
			{
				eponaSprite.setColor(sf::Color(255,255,255,eponaSprite.getColor().a+1));

				if(eponaSprite.getColor().a == 255)
				{
					waitTime = 3000;
				}
			}
			else
			{
				Screen_Manager::getMe().fadeOut();
				Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
				Screen_Manager::getMe().fadeIn();
			}
		}

		textMoveClock.restart();
	}

	if(nopeAlpha>0)
	{
		nopeText.setFillColor(sf::Color(150,0,0,nopeAlpha));
		nopeAlpha-=5;
	}

	if(showPicture)
	{
		if(frameClock.getElapsedTime().asMilliseconds() >= 100)
		{
			if(pictureNumber < 4)
				pictureNumber++;
			else
				pictureNumber = 0;

			eponaTexture.loadFromFile("Img/Animations/Epona/" + std::to_string(pictureNumber) + ".png");
			eponaSprite.setTexture(eponaTexture);

			frameClock.restart();
		}
	}
}

void Credits_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	for(auto cursor : nameTexts)
		mainWindow.draw(*cursor);

	mainWindow.draw(titleText);

	if(nopeAlpha>0)
		mainWindow.draw(nopeText);

	if(showPicture)
		mainWindow.draw(eponaSprite);
}

void Credits_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
		{
			if(nopeAlpha == 0)
			{
				nopeAlpha = 255;

				nopeText.setPosition(rand()%(WINDOW_SIZE_X-50),rand()%(WINDOW_SIZE_Y-50));
			}
		}

		switch(easterEgg)
		{
		case 0:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S)) easterEgg++;
			else easterEgg = 0;
			break;
		case 1:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K)) easterEgg++;
			else easterEgg = 0;
			break;
		case 2:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::I)) easterEgg++;
			else easterEgg = 0;
			break;
		case 3:
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::P)) easterEgg++;
			else easterEgg = 0;
			break;
		}

		
		break;
	}
}

void Credits_Screen::loadScreen()
{
	DOUT("")
	IOUT("Kredits-Screen wird gestartet")

	Sound_Manager::getMe().changeBackgroundMusic("Music/Staff Credits.ogg");

	nextCredit(textList.begin());

	easterEgg = 0;
	waitTime = 9;

	showPicture = false;
	eponaSprite.setColor(sf::Color(255,255,255,0));

	isLoaded = true;
}

void Credits_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Credits_Screen::closeScreen()
{
	isLoaded = false;

	Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Stopped);
}

void Credits_Screen::initScreen()
{
	titleText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),24);
	titleText.setStyle(sf::Text::Style::Italic);

	nopeText = sf::Text("Nope!",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);  
	nopeAlpha = 0;

	credit newCredit;

	newCredit.title = "- The worst Link Adventure -";
	newCredit.names.push_back("Die vergessene Welt");
	newCredit.titleColor = sf::Color::Green;
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Idee -";
	newCredit.names.push_back("Clemens Hübner");
	newCredit.titleColor = sf::Color::Yellow;
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Programmierung -";
	newCredit.names.push_back("Clemens Hübner");
	newCredit.titleColor = sf::Color::Blue;
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Game-Designe -";
	newCredit.names.push_back("Clemens Hübner");
	newCredit.titleColor = sf::Color(100,100,100);
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Story -";
	newCredit.names.push_back("Clemens Hübner");
	newCredit.titleColor = sf::Color::Magenta;
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Karten Gestaltung -";
	newCredit.names.push_back("Clemens Hübner");
	newCredit.titleColor = sf::Color(0,75,0);
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Spiel Tester -";
	newCredit.names.push_back("Salih Selvi");
	newCredit.names.push_back("Lara Kohlus");
	newCredit.names.push_back("Julia Golfmann");
	newCredit.names.push_back("Lukas Klinger");
	newCredit.names.push_back("Sebastian Brede");
	newCredit.names.push_back("Michelle Bochnik");
	newCredit.titleColor = sf::Color::Cyan;
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Musik & Sounds -";
	newCredit.names.push_back("siehe Quellen-Verzeichniss");
	newCredit.titleColor = sf::Color::Red;
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "- Grafiken & Zeichnungen -";
	newCredit.names.push_back("siehe Quellen-Verzeichniss");
	newCredit.titleColor = sf::Color(255,140,0);
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "Ein kleines Dankeschön an";
	newCredit.names.push_back("Jonathan Ludwig");
	newCredit.names.push_back("Aeneas Schwaiger");
	newCredit.names.push_back("Micha Lengemann");
	newCredit.names.push_back("Titus Schlotthauer");
	newCredit.names.push_back("Niklas Löbermann");
	newCredit.names.push_back("Selina Mlotek");
	newCredit.titleColor = sf::Color(200,0,200);
	textList.push_back(newCredit);
	newCredit.names.clear();

	newCredit.title = "Dankeschön fürs Spielen";
	newCredit.names.push_back("<3");
	newCredit.titleColor = sf::Color::Green;
	textList.push_back(newCredit);
	newCredit.names.clear();


	waitTime = 8;

	easterEgg = 0;

	pictureNumber = 0;
	eponaTexture.loadFromFile("Img/Animations/Epona/" + std::to_string(pictureNumber) + ".png");

	eponaSprite.setTexture(eponaTexture);
	eponaSprite.setScale(1,1);
	eponaSprite.setPosition(WINDOW_SIZE_X/2 - eponaSprite.getGlobalBounds().width/2 , WINDOW_SIZE_Y/2 - eponaSprite.getGlobalBounds().height/2);
	eponaSprite.setColor(sf::Color(255,255,255,0));

	showPicture = false;
}

void Credits_Screen::nextCredit(std::list<credit>::iterator it)
{
	if(textList.size() > 0)
	{
		creditIterator = it;

		titleText.setString(creditIterator->title);
		titleText.setOrigin(titleText.getGlobalBounds().width/2,titleText.getGlobalBounds().height/2);
		titleText.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y);
		titleText.setFillColor(creditIterator->titleColor);

		for(auto cursor : nameTexts)
		{
			delete cursor;
		}
		nameTexts.clear();

		int nameAmount = 0;

		for(auto cursor : creditIterator->names)
		{
			sf::Text *newName = new sf::Text(cursor,Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);

			newName->setOrigin(newName->getGlobalBounds().width/2,newName->getGlobalBounds().height/2);
			newName->setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y + titleText.getGlobalBounds().height + 16 + (25*nameAmount));

			nameTexts.push_back(newName);

			if(nameAmount+1 == creditIterator->names.size())
			{
				creditSize = newName->getPosition().y + newName->getGlobalBounds().height - titleText.getPosition().y; 
			}

			nameAmount++;
		}

		if(nameAmount == 0)
		{
			creditSize = titleText.getGlobalBounds().height;
		}
	}
	else
	{
		FOUT("Es sind keine Kredit-Einträge vorhanden!")
		Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
	}
}

Credits_Screen::~Credits_Screen(void)
{
	for(auto cursor : nameTexts)
	{
		delete cursor;
	}
	nameTexts.clear();
}
