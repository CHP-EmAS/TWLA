#include "Screens.h"

#include "Graphic_Manager.h"
#include "Sound_Manager.h"

Logo_Screen::Logo_Screen(void) : Screen(Screen_Type::Logos)
{
	initScreen();
}

void Logo_Screen::updateScreen()
{
	switch(screenState)
	{
	case 0: //Show "CHP presents"
		if(screenStateClock.getElapsedTime().asMilliseconds() > 1500) 
		{
			screenState = 1;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Bing,false,false);
			screenStateClock.restart();
		}
		break;
	case 1://Wait 2 Sec
		if(screenStateClock.getElapsedTime().asMilliseconds() > 2000)
		{
			screenState = 2;
			Sound_Manager::getMe().changeBackgroundMusic("Music/Theme.ogg");
			screenStateClock.restart();
		}
		break;
	case 2://Background fade in
		if(background.getColor().a + 2 < 255) 
			background.setColor(sf::Color(255,255,255,background.getColor().a + 2));
		else
		{
			background.setColor(sf::Color(255,255,255,255));
			
			screenState = 3;
		}
		break;
	case 3://"The worst" flys in
		if(screenStateClock.getElapsedTime().asMilliseconds() > 10)
		{
			bool correct = true;

			if(the_text.getPosition().x + 20 < WINDOW_SIZE_X/4)
			{
				the_text.move(20,0);
				correct = false;
			}

			if(worst_text.getPosition().x - 20 > the_text.getPosition().x + the_text.getGlobalBounds().width + 20)
			{
				worst_text.move(-20,0);
				correct = false;
			}

			if(correct)
			{
				the_text.setPosition(WINDOW_SIZE_X/4,WINDOW_SIZE_Y/4 - 10);
				worst_text.setPosition(the_text.getPosition().x + the_text.getGlobalBounds().width + 20,the_text.getPosition().y+2);
				screenState = 4;
			}
				
			screenStateClock.restart();
		}
		break;
	case 4://Wait 0.5 Sec
		if(screenStateClock.getElapsedTime().asMilliseconds() > 500)
		{
			screenState = 5;
			screenStateClock.restart();
		}
		break;
	case 5://Fade "Link" in
		if(screenStateClock.getElapsedTime().asMilliseconds() > 10)
		{
			if(link_text.getFillColor().a + 10 < 255) link_text.setFillColor(sf::Color(0,200,0,link_text.getFillColor().a + 10));
			else
			{
				link_text.setFillColor(sf::Color(0,200,0,255));
				screenState = 6;
			}
		}
		break;
	case 6://Print "Adventure"
		if(screenStateClock.getElapsedTime().asMilliseconds() > 100)
		{
			if(adventure_text.getString().getSize() < 9)
			{
				switch(adventure_text.getString().getSize())
				{
				case 0:
					adventure_text.setString("A");
					break;
				case 1:
					adventure_text.setString("Ad");
					break;
				case 2:
					adventure_text.setString("Adv");
					break;
				case 3:
					adventure_text.setString("Adve");
					break;
				case 4:
					adventure_text.setString("Adven");
					break;
				case 5:
					adventure_text.setString("Advent");
					break;
				case 6:
					adventure_text.setString("Adventu");
					break;
				case 7:
					adventure_text.setString("Adventur");
					break;
				case 8:
					adventure_text.setString("Adventure");
					break;
				default:
					adventure_text.setString("Adventure");
					break;
				}
			}
			else
			{
				screenState = 7;
			}
				
			screenStateClock.restart();
		}
		break;
	case 7://Fade Sword Img and vorgottenText in
		if(screenStateClock.getElapsedTime().asMilliseconds() > 10)
		{
			if(spr_sword.getColor().a + 2 < 255) spr_sword.setColor(sf::Color(255,255,255,spr_sword.getColor().a + 2));
			else
			{
				spr_sword.setColor(sf::Color(255,255,255,255));
			}

			if(vorgottenWorld_text.getFillColor().a + 2 < 255) vorgottenWorld_text.setFillColor(sf::Color(0,255,255,vorgottenWorld_text.getFillColor().a + 2));
			else
			{
				vorgottenWorld_text.setFillColor(sf::Color(0,255,255,255));
			}

			if(vorgottenWorld_text.getFillColor().a == 255 && spr_sword.getColor().a == 255) screenState = 8;
		}
		break;
	case 8://Wait 3Sec
		if(screenStateClock.getElapsedTime().asMilliseconds() > 3000)
		{
			screenState = 9;
			screenStateClock.restart();
		}
		break;
	case 9://Switch to 10
		if(screenStateClock.getElapsedTime().asMilliseconds() > 750)
		{
			screenState = 10;
			screenStateClock.restart();
		}
		break;
	case 10://Switch to 9
		if(screenStateClock.getElapsedTime().asMilliseconds() > 750)
		{
			screenState = 9;
			screenStateClock.restart();
		}
		break;
	}
}

void Logo_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	if(screenState == 1)
	{
		mainWindow.draw(CHPText);
		mainWindow.draw(presentsText);
	}

	if(screenState >= 2)
	{
		//mainWindow.draw(background);
	}

	if(screenState >= 7)
	{
		mainWindow.draw(spr_sword);
		mainWindow.draw(vorgottenWorld_text);
	}

	if(screenState >= 3)
	{
		mainWindow.draw(the_text);
		mainWindow.draw(worst_text);
	}

	if(screenState >= 5)
	{
		mainWindow.draw(link_text);
	}

	if(screenState >= 6)
	{
		mainWindow.draw(adventure_text);
	}

	if(screenState == 9)
	{
		mainWindow.draw(contiueText);
	}
}
	

void Logo_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
		{
			Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
		}
	break;
	}
}

void Logo_Screen::loadScreen()
{
	screenState = 0;

	the_text.setPosition(-the_text.getGlobalBounds().width-220,WINDOW_SIZE_Y/4-10);
	worst_text.setPosition(WINDOW_SIZE_X + worst_text.getGlobalBounds().width,the_text.getPosition().y+2);
	link_text.setFillColor(sf::Color(0,200,0,0));
	adventure_text.setString("");
	spr_sword.setColor(sf::Color(255,255,255,0));
	vorgottenWorld_text.setFillColor(sf::Color(0,150,150,0));

	screenStateClock.restart();

	setLoaded(true);
}

void Logo_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Logo_Screen::closeScreen()
{
	setLoaded(false);
}

void Logo_Screen::initScreen()
{
	CHPText = sf::Text("- CHP -",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),26);
	CHPText.setOrigin(CHPText.getGlobalBounds().width/2,CHPText.getGlobalBounds().height/2);
	CHPText.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2);

	presentsText = sf::Text("presents",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),16);
	presentsText.setOrigin(presentsText.getGlobalBounds().width/2,presentsText.getGlobalBounds().height/2);
	presentsText.setPosition(WINDOW_SIZE_X/2,CHPText.getPosition().y+25);

	contiueText = sf::Text("press 'Space' to continue",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);
	contiueText.setOrigin(contiueText.getGlobalBounds().width/2,contiueText.getGlobalBounds().height/2);
	contiueText.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y-contiueText.getGlobalBounds().height - 25);
	contiueText.setOutlineColor(sf::Color::Black);
	contiueText.setOutlineThickness(2);

	the_text = sf::Text("The",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),30);
	the_text.setPosition(WINDOW_SIZE_X/4,WINDOW_SIZE_Y/4 - 20);
	the_text.setOutlineThickness(3);
	the_text.setOutlineColor(sf::Color(100,100,100));

	worst_text = sf::Text("worst",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),30);
	worst_text.setPosition(the_text.getPosition().x + the_text.getGlobalBounds().width + 20,the_text.getPosition().y+2);
	worst_text.setFillColor(sf::Color(200,0,0));
	worst_text.setStyle(sf::Text::Style::Bold);
	worst_text.setOutlineThickness(3);
	worst_text.setOutlineColor(sf::Color(0,0,0));

	link_text = sf::Text("Link",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),100);
	link_text.setPosition(the_text.getPosition().x,the_text.getPosition().y + the_text.getGlobalBounds().height);
	link_text.setFillColor(sf::Color(0,200,0));
	link_text.setOutlineThickness(6);
	link_text.setOutlineColor(sf::Color(0,100,0));

	adventure_text = sf::Text("Adventure",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),60);
	adventure_text.setPosition(link_text.getPosition().x + 10,link_text.getPosition().y + link_text.getGlobalBounds().height + 25);
	adventure_text.setFillColor(sf::Color(255,255,0));
	adventure_text.setStyle(sf::Text::Style::Italic);
	adventure_text.setOutlineThickness(3);
	adventure_text.setOutlineColor(sf::Color(50,50,0));
	
	vorgottenWorld_text = sf::Text("Die vergessene Welt",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),30);
	vorgottenWorld_text.setPosition(adventure_text.getPosition().x - 85,adventure_text.getPosition().y + adventure_text.getGlobalBounds().height + 100);
	vorgottenWorld_text.setFillColor(sf::Color(0,100,100));
	vorgottenWorld_text.setOutlineThickness(3);
	vorgottenWorld_text.setOutlineColor(sf::Color(50,50,50));

	tex_sword.loadFromFile("Img/ect/Master Schwert.png");

	spr_sword.setTexture(tex_sword);
	spr_sword.setOrigin(tex_sword.getSize().x/2,tex_sword.getSize().y/2);
	spr_sword.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/3+50);
	spr_sword.setScale(0.25,0.25);

	backTexture.loadFromFile("Img/Backgrounds/Intro.png");

	background.setTexture(backTexture);
	background.setPosition(0,0);
	background.setColor(sf::Color(255,255,255,0));

	screenState = 0;
}


Logo_Screen::~Logo_Screen(void)
{
}
