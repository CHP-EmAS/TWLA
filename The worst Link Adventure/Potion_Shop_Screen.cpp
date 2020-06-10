#include "Screens.h"

#include "Screen_Manager.h"
#include "Graphic_Manager.h"
#include "Item_List.h"
#include "Player_Manager.h"
#include "Quest_Manager.h"

Potion_Shop_Screen::Potion_Shop_Screen(void):Screen(Screen_Type::Potion_Shop)
{
	initScreen();
}

void Potion_Shop_Screen::updateScreen()
{
	
}

void Potion_Shop_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

	mainWindow.draw(frame);

	mainWindow.draw(shopHeadline);
	mainWindow.draw(shopKeyInfo);

	mainWindow.draw(item[0]);
	mainWindow.draw(item[1]);
	mainWindow.draw(item[2]);

	mainWindow.draw(money[0]);
	mainWindow.draw(money[1]);
	mainWindow.draw(money[2]);

	for(short i = 0; i < 3; i++)
	{
		if(selectedRow == i) itemFrame.setOutlineColor(sf::Color(255,215,0));
		else itemFrame.setOutlineColor(sf::Color(175,175,175));
	
		itemFrame.setPosition(item[i].getPosition()-sf::Vector2f(50,50));

		mainWindow.draw(itemFrame);
	}
	
}

void Potion_Shop_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
		{
			int c_price = this->price[selectedRow];

			if(Player_Manager::getMe().getGameHUD().getEdelsteine() >= c_price)
			{
				Player_Manager::getMe().getGameHUD().removeEdelsteine(c_price);
				
				switch(selectedRow)
				{
				case 0:
					Player_Manager::getMe().give(30,1);
					break;
				case 1:
					Player_Manager::getMe().give(29,1);
					break;
				case 2:
					if(showKey) Player_Manager::getMe().give(53,10);
					else Player_Manager::getMe().give(992,1);
					break;
				}

				loadScreen();
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			if(selectedRow <= 0) selectedRow = 2;
			else selectedRow--;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			if(selectedRow >= 2) selectedRow = 0;
			else selectedRow++;
		}
	}
}

void Potion_Shop_Screen::loadScreen()
{
	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Der Shop kann nur bei geladenem Spiel geöffnet werden!");
		Screen_Manager::getMe().changeScreen(Main_Menue);
	}

	if(Quest_Manager::getMe().isQuestCommandDone("found-992"))
	{
		showKey = true;
		item[2].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,409));
		price[2] = 40;
		money[2].setString("0040");
		money[2].setOrigin(money[2].getGlobalBounds().width/2,money[2].getGlobalBounds().height/2);
	}

	int currentMoney = Player_Manager::getMe().getGameHUD().getEdelsteine();

	for(short i = 0; i < 3; i++)
	{
		if(currentMoney >= price[i]) money[i].setFillColor(sf::Color::Green);
		else money[i].setFillColor(sf::Color::Red);
	}

	selectedRow = 0;

	setLoaded(true);
}

void Potion_Shop_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Potion_Shop_Screen::closeScreen()
{
	setLoaded(false);
}

void Potion_Shop_Screen::initScreen()
{
	shopHeadline = sf::Text("--> Mindy's Tränke Shop <--",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),27);
	shopHeadline.setOrigin(shopHeadline.getGlobalBounds().width/2,shopHeadline.getGlobalBounds().height/2);
	shopHeadline.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/3-shopHeadline.getGlobalBounds().height-30);
	shopHeadline.setOutlineColor(sf::Color::Black);
	shopHeadline.setOutlineThickness(2);
	
	shopKeyInfo = sf::Text("Pfeiltasten - Item auswählen\nEnter       -    Item kaufen\nEscape      -         Zurück",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	shopKeyInfo.setOrigin(shopKeyInfo.getGlobalBounds().width/2,shopKeyInfo.getGlobalBounds().height/2);
	shopKeyInfo.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4*3);
	shopKeyInfo.setOutlineColor(sf::Color::Black);
	shopKeyInfo.setOutlineThickness(2);

	frame.setSize(sf::Vector2f(WINDOW_SIZE_X+2, WINDOW_SIZE_Y/3));
	frame.setFillColor(sf::Color(139,90,43));
	frame.setOutlineThickness(3);
	frame.setOutlineColor(sf::Color::Black);
	frame.setOrigin(0,frame.getGlobalBounds().height/2);
	frame.setPosition(-1,WINDOW_SIZE_Y/2);

	item[0].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	item[0].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,259));
	item[0].setScale(5,5);
	item[0].setOrigin(8,8);
	item[0].setPosition(WINDOW_SIZE_X/4,WINDOW_SIZE_Y/2-30);

	item[1].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	item[1].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,258));
	item[1].setScale(5,5);
	item[1].setOrigin(8,8);
	item[1].setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2-30);

	item[2].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	item[2].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,358));
	item[2].setScale(5,5);
	item[2].setOrigin(8,8);
	item[2].setPosition(WINDOW_SIZE_X/4*3,WINDOW_SIZE_Y/2-30);

	price[0] = 25;
	price[1] = 50;
	price[2] = 999;

	money[0] = sf::Text("0025",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	money[1] = sf::Text("0050",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	money[2] = sf::Text("999",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);

	for(int i = 0; i < 3; i++)
	{
		money[i].setOrigin(money[i].getGlobalBounds().width/2,money[i].getGlobalBounds().height/2);
		money[i].setPosition(item[i].getPosition() + sf::Vector2f(0,60));
		money[i].setOutlineColor(sf::Color::Black);
		money[i].setOutlineThickness(2);
	}

	itemFrame.setSize(sf::Vector2f(100,135));
	itemFrame.setPosition(item[0].getPosition()-sf::Vector2f(50,50));
	itemFrame.setFillColor(sf::Color(0,0,0,0));
	itemFrame.setOutlineThickness(3);
	itemFrame.setOutlineColor(sf::Color(255,215,0));

	selectedRow = 0;

	showKey = false;
}

Potion_Shop_Screen::~Potion_Shop_Screen(void)
{
}