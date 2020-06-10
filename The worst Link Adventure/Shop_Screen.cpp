#include "Screens.h"

#include "Screen_Manager.h"
#include "Graphic_Manager.h"
#include "Item_List.h"
#include "Player_Manager.h"

Shop_Screen::Shop_Screen(void):Screen(Screen_Type::Shop)
{
	initScreen();
}

void Shop_Screen::updateScreen()
{
	
}

void Shop_Screen::drawScreen(sf::RenderWindow &mainWindow)
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

void Shop_Screen::checkEvents(sf::Event newEvent)
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
			if(money[selectedRow].getString() != "SOLD")
			{
				int price = it_list[selectedRow]->price;

				if(Player_Manager::getMe().getGameHUD().getEdelsteine() >= price)
				{
					Player_Manager::getMe().getGameHUD().removeEdelsteine(price);
					
					Player_Manager::getMe().give(it_list[selectedRow]->itemNumber,1);

					item[selectedRow].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
					item[selectedRow].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
					money[selectedRow].setString("SOLD");
					money[selectedRow].setFillColor(sf::Color::Red);

					itemRowPosition[selectedRow]++;

					for(short i = 0; i < 3; i++)
					{
						if(money[i].getString() != "SOLD")
						{
							int price = it_list[i]->price;
							int currentMoney = Player_Manager::getMe().getGameHUD().getEdelsteine();

							if(currentMoney >= price) money[i].setFillColor(sf::Color::Green);
							else money[i].setFillColor(sf::Color::Red);
						}
					}
				}
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

void Shop_Screen::loadScreen()
{
	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Der Shop kann nur bei geladenem Spiel geöffnet werden!");
		Screen_Manager::getMe().changeScreen(Main_Menue);
	}

	for(short i = 0; i < 3; i++)
	{
		bool soldout = false;
		it_list[i] = itemRow[i].begin();

		for(short itemPos = 0; itemPos < itemRowPosition[i]; itemPos++)
		{
			if(itemPos < itemRow[i].size() -1) it_list[i]++;
			else soldout = true;
		}

		if(soldout)
		{
			item[i].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
			item[i].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
			money[i].setString("SOLD");
			money[i].setFillColor(sf::Color::Red);
		}
		else
		{
			item[i].setTexture(Graphic_Manager::getMe().getTilesetTexture(Item_List::getMe().getItemTileset(it_list[i]->itemNumber)));
			item[i].setTextureRect(Graphic_Manager::getMe().getIntRect(Item_List::getMe().getItemTileset(it_list[i]->itemNumber),Item_List::getMe().getItemTile(it_list[i]->itemNumber)));

			int price = it_list[i]->price;
			std::string str_price; 

			if(price < 10) str_price = "000" + std::to_string(price);
			else if(price < 100) str_price = "00" + std::to_string(price);
			else if(price < 1000) str_price = "0" + std::to_string(price);
			else str_price = std::to_string(price);

			money[i].setString(str_price);

			int currentMoney = Player_Manager::getMe().getGameHUD().getEdelsteine();

			if(currentMoney >= price) money[i].setFillColor(sf::Color::Green);
			else money[i].setFillColor(sf::Color::Red);
		}
	}

	selectedRow = 0;

	setLoaded(true);
}

void Shop_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Shop_Screen::closeScreen()
{
	setLoaded(false);
}

void Shop_Screen::initScreen()
{
	shopHeadline = sf::Text("--> Shoiop Shop <--",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),27);
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
	item[0].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
	item[0].setScale(5,5);
	item[0].setOrigin(8,8);
	item[0].setPosition(WINDOW_SIZE_X/4,WINDOW_SIZE_Y/2-30);

	item[1].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	item[1].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
	item[1].setScale(5,5);
	item[1].setOrigin(8,8);
	item[1].setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2-30);

	item[2].setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	item[2].setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
	item[2].setScale(5,5);
	item[2].setOrigin(8,8);
	item[2].setPosition(WINDOW_SIZE_X/4*3,WINDOW_SIZE_Y/2-30);

	for(int i = 0; i < 3; i++)
	{
		money[i] = sf::Text("SOLD",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
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

	std::string str_itemRow = "37,15;38,75;42,500";
	itemRow[0] = createItemRow(str_itemRow);

	str_itemRow = "13,100;14,200;15,350;27,150";
	itemRow[1] = createItemRow(str_itemRow);

	str_itemRow = "31,150;8,200;11,150;17,175";
	itemRow[2] = createItemRow(str_itemRow);

	itemRowPosition[0] = 0;
	itemRowPosition[1] = 0;
	itemRowPosition[2] = 0;
}

std::list<Shop_Screen::shopItem> Shop_Screen::createItemRow(std::string str_row)
{
	std::list<shopItem> newRow;

	shopItem newItem;

	int itemPos = 0;
	int cutPos = 0;

	while((itemPos = str_row.find(";")) != std::string::npos)
	{
		std::string str_item = str_row.substr(0,itemPos);
		str_row = str_row.substr(itemPos+1);

		if((cutPos = str_item.find(",")) != std::string::npos)
		{
			newItem.itemNumber = atoi(str_item.substr(0,cutPos).c_str());
			newItem.price = atoi(str_item.substr(cutPos+1).c_str());

			newRow.push_back(newItem);
		}
	}

	if((cutPos = str_row.find(",")) != std::string::npos)
	{
		newItem.itemNumber = atoi(str_row.substr(0,cutPos).c_str());
		newItem.price = atoi(str_row.substr(cutPos+1).c_str());

		newRow.push_back(newItem);
	}

	return newRow;
}

bool Shop_Screen::loadMySaveData(std::string data)
{
	for(int i = 0; i < 3; i++)
	{
		int cutPos = data.find(",");
		itemRowPosition[i] = atoi(data.substr(0,cutPos).c_str());
		data = data.substr(cutPos+1);
	}
	
	return true;
}

std::string Shop_Screen::getMySaveData()
{
	std::string data = "";

	data += std::to_string(itemRowPosition[0]) + ",";
	data += std::to_string(itemRowPosition[1]) + ",";
	data += std::to_string(itemRowPosition[2]) + ",";

	return data;
}

Shop_Screen::~Shop_Screen(void)
{
}
