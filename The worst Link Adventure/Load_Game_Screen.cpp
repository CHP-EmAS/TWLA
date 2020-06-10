#include "Screens.h"

#include "File_Manager.h"
#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Sound_Manager.h"

Load_Game_Screen::Load_Game_Screen(void) : Screen(Screen::Savegame_Selector)
{
	initScreen();
}

void Load_Game_Screen::updateScreen()
{
	
}

void Load_Game_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(background);
	mainWindow.draw(mainFrame);

	mainWindow.draw(headline);

	if(foundGameDatas.size() > 0)
	{
		mainWindow.draw(nameText);

		mainWindow.draw(dateText);

		mainWindow.draw(mapText);

		mainWindow.draw(energyFrame);
		mainWindow.draw(energyShape);

		heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,252));
		for(int i = 0; i < currentSelectedData->maxLive;i++)
		{
			if(i < 10)
			{
				heart.setPosition((TILESIZE + 1) * float(i) + 60,WINDOW_SIZE_Y / 2);
			}
			else 
			{
				heart.setPosition((TILESIZE + 1) * (float(i) - 10.f) + 60,WINDOW_SIZE_Y / 2 + TILESIZE);
			}
			
			if(i == int(currentSelectedData->live))
			{
				if(currentSelectedData->live - int(currentSelectedData->live) == 0.25f)
					heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,16));
				else if(currentSelectedData->live - int(currentSelectedData->live) == 0.5f)
					heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,17));
				else if(currentSelectedData->live - int(currentSelectedData->live) == 0.75f)
					heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,18));
				else if(currentSelectedData->live - int(currentSelectedData->live) == 0.f)
					heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,15));
			}
			else if(i == int(currentSelectedData->live) + 1)
			{
				heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,15));
			}

			mainWindow.draw(heart);
		}

		mainWindow.draw(edelstein);
		mainWindow.draw(edelsteineText);

		mainWindow.draw(spr_screenshot);

		mainWindow.draw(leftTriangle);
		mainWindow.draw(rightTriangle);
	}
	else
	{
		mainWindow.draw(errorText);
	}

	mainWindow.draw(infoText);
}

void Load_Game_Screen::checkEvents(sf::Event newEvent)
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

		if(foundGameDatas.size() > 0)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				std::list<gameData>::const_iterator test = currentSelectedData;
				test++;

				if(test != foundGameDatas.end())
				{
					printNextData(++currentSelectedData);
				}
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				if(currentSelectedData != foundGameDatas.begin())
				{
					printNextData(--currentSelectedData);
				}
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return))
			{
				Screen_Manager::getMe().fadeOut();
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
				File_Manager::getMe().load_Savegame("Savegames/" + currentSelectedData->path);
				File_Manager::getMe().loadCurrentMapChanges();
				Screen_Manager::getMe().fadeIn();
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Delete))
			{
				std::string delCommand = "rmdir Savegames\\" + currentSelectedData->path + " /s /q"; 

				system(delCommand.c_str());

				listAllGameDatas();
			}
		}
	}
}

void Load_Game_Screen::loadScreen()
{
	DOUT("")
	IOUT("Savegame-Selector-Screen wird gestartet")

	listAllGameDatas();

	setLoaded(true);

	Sound_Manager::getMe().changeBackgroundMusic("Music/File_Select.ogg");
}

void Load_Game_Screen::restartScreen()
{
	loadScreen();
}

void Load_Game_Screen::closeScreen()
{
	setLoaded(false);
}

void Load_Game_Screen::initScreen()
{
	backTexture.loadFromFile("Img/Backgrounds/2.png");

	background.setTexture(backTexture);
	background.setPosition(0,0);

	mainFrame.setSize(sf::Vector2f(WINDOW_SIZE_X+2,WINDOW_SIZE_Y/3));
	mainFrame.setOrigin(0,mainFrame.getGlobalBounds().height/2);
	mainFrame.setPosition(-1,WINDOW_SIZE_Y/2);
	mainFrame.setFillColor(sf::Color(75,75,75,220));
	mainFrame.setOutlineThickness(3);
	mainFrame.setOutlineColor(sf::Color(50,50,50));

	nameText = sf::Text("Error",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),24);
	nameText.setPosition(60,WINDOW_SIZE_Y/3+20);

	dateText = sf::Text("00.00.0000",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);
	dateText.setPosition(60,nameText.getPosition().y + nameText.getGlobalBounds().height+10);

	mapText = sf::Text("------",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),14);
	mapText.setPosition(60,dateText.getPosition().y + dateText.getGlobalBounds().height+10);

	infoText = sf::Text("Enter  -   Laden\nEntf   - Löschen\nEscape -  Zurück",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	infoText.setPosition(WINDOW_SIZE_X / 2 - infoText.getGlobalBounds().width / 2,WINDOW_SIZE_Y / 2 + 120);
	infoText.setOutlineColor(sf::Color::Black);
	infoText.setOutlineThickness(2);

	headline = sf::Text("Spielstand laden",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),26);;
	headline.setPosition(WINDOW_SIZE_X / 2 - headline.getGlobalBounds().width / 2,100);
	headline.setFillColor(sf::Color(255,215,0));
	headline.setOutlineColor(sf::Color::Black);
	headline.setOutlineThickness(2);

	errorText = sf::Text("Keine Spielstände gefunden!",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),20);
	errorText.setPosition(WINDOW_SIZE_X / 2 - errorText.getGlobalBounds().width / 2,WINDOW_SIZE_Y / 2 - errorText.getGlobalBounds().height / 2);

	//Heart
	heart.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,18));

	//Energy
	energyShape.setFillColor(sf::Color::Yellow);

	energyFrame.setFillColor(sf::Color(100,100,100,150));
	energyFrame.setSize(sf::Vector2f((TILESIZE+1)*10,7));
	energyFrame.setOutlineColor(sf::Color(200,200,200));
	energyFrame.setOutlineThickness(1);

	//Edelstein
	edelstein.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	edelstein.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,418));
	edelstein.setScale(2,2);

	edelsteineText = sf::Text("0000",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),20);
	edelsteineText.setStyle(sf::Text::Bold);

	//Left
	leftTriangle = sf::VertexArray(sf::PrimitiveType::Triangles,3);

	leftTriangle[0].position = sf::Vector2f(10,WINDOW_SIZE_Y/2);
	leftTriangle[1].position = sf::Vector2f(40,WINDOW_SIZE_Y/2-30);
	leftTriangle[2].position = sf::Vector2f(40,WINDOW_SIZE_Y/2+30);

	//Right
	rightTriangle = sf::VertexArray(sf::PrimitiveType::Triangles,3);

	rightTriangle[0].position = sf::Vector2f(WINDOW_SIZE_X - 10,WINDOW_SIZE_Y/2);
	rightTriangle[1].position = sf::Vector2f(WINDOW_SIZE_X - 40,WINDOW_SIZE_Y/2-30);
	rightTriangle[2].position = sf::Vector2f(WINDOW_SIZE_X - 40,WINDOW_SIZE_Y/2+30);
}

void Load_Game_Screen::listAllGameDatas()
{
	foundGameDatas.clear();

	IOUT("Spielstände werden gesucht...")

	WIN32_FIND_DATA FData; 

	std::string stemp = "Savegames/*.*";

	std::wstring wPath;
	wPath.resize(stemp.size());
	int c =  MultiByteToWideChar( CP_ACP , 0 , stemp.c_str() , stemp.size() , &wPath[0], 260 ); 

	LPCWSTR lpc_searchPath = wPath.c_str();
	
    HANDLE hSearch = FindFirstFile(lpc_searchPath,&FData); 
    BOOL MoreFiles = FALSE; 
    int cnt_dir = 0, cnt_file  = 0;
    
    if (hSearch == INVALID_HANDLE_VALUE) return;
      
    do 
    { 
		// Ist das ein Ordner ?
		if((FData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0)
		{ 
			char name[260];
			WideCharToMultiByte(CP_ACP,0,FData.cFileName,-1, name,260,NULL, NULL);
			
			std::string str_dirName(name);

			if(str_dirName.length() > 5)
			{
				std::string beginn = str_dirName.substr(0,5);

				if(beginn == "save#")
				{
					DOUT("");
					IOUT("Spielstand <" << str_dirName << "> gefunden!");

					gameData newData;

					newData.date = "0.0.0000";
					newData.name = "ERROR";
					newData.path = str_dirName;
					newData.live = 0;
					newData.maxLive = 0;
					newData.energy = 0;
					newData.maxEnergy = 0;
					newData.n_Edelsteine = 0;

					std::list<fullCommand> mainCommands = File_Manager::getMe().convertFileToCommandlist("Savegames/" + str_dirName + "/main.zData");

					for(auto cursor : mainCommands)
					{
						if(cursor.command == "name")
						{
							newData.name = cursor.content;
						}
						else if(cursor.command == "date")
						{
							newData.date = cursor.content;
						}
						else if(cursor.command == "hud")
						{
							int cutPos;

							for(int i = 0; i < 6; i++)
							{
								cutPos = cursor.content.find(",");
								std::string hudData = cursor.content.substr(0,cutPos);
								cursor.content = cursor.content.substr(cutPos+1);

								switch(i)
								{
								case 0:
									newData.maxLive = atoi(hudData.c_str());
									break;
								case 1:
									newData.live = atof(hudData.c_str());
									break;
								case 2:
									newData.maxEnergy = atoi(hudData.c_str());
									break;
								case 3:
									newData.energy = atoi(hudData.c_str());
									break;
								case 4:
									newData.n_Edelsteine = atoi(hudData.c_str());
									break;
								}
							}
						}
						else if(cursor.command == "player")
						{
							int cutPos = cursor.content.find(",");
							std::string str_tempMapID = cursor.content.substr(0,cutPos);
							
							newData.mapName = Map_Manager::getMe().getMapName(atoi(str_tempMapID.c_str())) ;
						}
					}

					DOUT("Name: " << newData.name);
					DOUT("Datum: " << newData.date);
					DOUT("Max. Leben: " << newData.maxLive);
					DOUT("Leben: " << newData.live);
					DOUT("Max. Energy: " << newData.maxEnergy);
					DOUT("Energy: " << newData.energy);
					DOUT("Edelsteine: " << newData.n_Edelsteine);

					foundGameDatas.push_back(newData);
				}
			}
		}
     
  
		MoreFiles = FindNextFile(hSearch,&FData); 

    }while(MoreFiles); 
    
	if(foundGameDatas.size() > 0)
		printNextData(foundGameDatas.begin());

    FindClose(hSearch);   
}

void Load_Game_Screen::printNextData(std::list<gameData>::const_iterator newPos)
{
	if(foundGameDatas.size() == 0)
	{
		return;
	}

	currentSelectedData = newPos;

	//Name
	nameText.setString(currentSelectedData->name);

	//Date
	dateText.setString(currentSelectedData->date);

	//Map
	mapText.setString(currentSelectedData->mapName);

	//Energy
	float percent = float(currentSelectedData->energy) / float(currentSelectedData->maxEnergy) * 100.f;
	float sizeX = float((TILESIZE+1)*10) / 100.f * percent;
	int green = 255 * percent/100;

	energyShape.setFillColor(sf::Color(255,green,0));
	energyShape.setSize(sf::Vector2f(sizeX,7));

	if(currentSelectedData->maxLive <= 10)
		energyFrame.setPosition(60,WINDOW_SIZE_Y/2 + TILESIZE+10);
	else
		energyFrame.setPosition(60,WINDOW_SIZE_Y/2 + TILESIZE*2+10);
	
	energyShape.setPosition(energyFrame.getPosition());

	//Edelstein
	std::string edelsteinString;

	if(currentSelectedData->n_Edelsteine < 10) edelsteinString = "000";
	else if(currentSelectedData->n_Edelsteine < 100) edelsteinString = "00";
	else if(currentSelectedData->n_Edelsteine < 1000) edelsteinString = "0";

	edelsteinString += std::to_string(currentSelectedData->n_Edelsteine);

	edelsteineText.setString(edelsteinString);

	if(currentSelectedData->maxLive <= 10)
		edelstein.setPosition(52,WINDOW_SIZE_Y/2 + TILESIZE+20+energyShape.getSize().y);
	else
		edelstein.setPosition(52,WINDOW_SIZE_Y/2 + TILESIZE*2+20+energyShape.getSize().y);

	edelsteineText.setPosition(edelstein.getPosition().x + edelstein.getGlobalBounds().width,edelstein.getPosition().y);

	//Screenshot
	if(tex_screenshot.loadFromFile("Savegames/" + currentSelectedData->path + "/ss.png"))
	{
		spr_screenshot.setTexture(tex_screenshot);
		spr_screenshot.setTextureRect(sf::IntRect(0,0,9*ZOOMSIZE,9*ZOOMSIZE));
		spr_screenshot.setScale(0.6,0.6);
	}
	else
	{
		spr_screenshot.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
		spr_screenshot.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
		spr_screenshot.setScale(7,7);
	}

	spr_screenshot.setPosition(WINDOW_SIZE_X/3*2-30,WINDOW_SIZE_Y / 2 - spr_screenshot.getGlobalBounds().height/2);

	//Left
	if(newPos == foundGameDatas.begin())
	{
		leftTriangle[0].color = sf::Color(0,0,0,75);
		leftTriangle[1].color = sf::Color(0,0,0,75);
		leftTriangle[2].color = sf::Color(0,0,0,75);
	}
	else
	{
		leftTriangle[0].color = sf::Color(0,0,0,175);
		leftTriangle[1].color = sf::Color(0,0,0,175);
		leftTriangle[2].color = sf::Color(0,0,0,175);
	}

	//Right
	std::list<gameData>::const_iterator test = currentSelectedData;
	test++;

	if(test == foundGameDatas.end())
	{
		rightTriangle[0].color = sf::Color(0,0,0,75);
		rightTriangle[1].color = sf::Color(0,0,0,75);
		rightTriangle[2].color = sf::Color(0,0,0,75);
	}
	else
	{
		rightTriangle[0].color = sf::Color(0,0,0,175);
		rightTriangle[1].color = sf::Color(0,0,0,175);
		rightTriangle[2].color = sf::Color(0,0,0,175);
	}
}

Load_Game_Screen::~Load_Game_Screen(void)
{
}
