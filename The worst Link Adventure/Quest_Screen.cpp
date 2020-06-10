#include "Screens.h"

#include "Screen_Manager.h"
#include "Graphic_Manager.h"
#include "Quest_Manager.h"
#include "Message_Manager.h"

Quest_Screen::Quest_Screen(void):Screen(Screen_Type::Quests)
{
	initScreen();
}

void Quest_Screen::updateScreen()
{
	
}

void Quest_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

	mainWindow.draw(frame);

	mainWindow.draw(headline);
	mainWindow.draw(keyInfo);

	mainWindow.draw(desFrame);

	mainWindow.draw(questName);
	mainWindow.draw(questDescription);
	mainWindow.draw(questStatus);

	mainWindow.draw(leftTriangle);
	mainWindow.draw(rightTriangle);
}

void Quest_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
		}

		if(questList.size() > 0)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
			{
				std::list<Quest*>::const_iterator test = questCursor;
				test++;

				if(test != questList.end())
				{
					questCursor++;
					setQuestInfo();
				}
			}

			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
			{
				if(questCursor != questList.begin())
				{
					questCursor--;
					setQuestInfo();
				}
			}
		}
	}
}

void Quest_Screen::loadScreen()
{
	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Das Quest-Menü kann nur bei geladenem Spiel geöffnet werden!");
		Screen_Manager::getMe().changeScreen(Main_Menue);
	}

	getQuests();

	setLoaded(true);
}

void Quest_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Quest_Screen::closeScreen()
{
	setLoaded(false);
}

void Quest_Screen::initScreen()
{
	headline = sf::Text("-- Quests --",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),27);
	headline.setFillColor(sf::Color::Blue);
	headline.setOrigin(headline.getGlobalBounds().width/2,headline.getGlobalBounds().height/2);
	headline.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4-headline.getGlobalBounds().height-30);
	headline.setOutlineColor(sf::Color::Black);
	headline.setOutlineThickness(2);
	
	keyInfo = sf::Text("Pfeiltasten - Quest auswählen\nEscape      -          Zurück",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	keyInfo.setOrigin(keyInfo.getGlobalBounds().width/2,keyInfo.getGlobalBounds().height/2);
	keyInfo.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4*3.2);
	keyInfo.setOutlineColor(sf::Color::Black);
	keyInfo.setOutlineThickness(2);

	frame.setSize(sf::Vector2f(WINDOW_SIZE_X+2, WINDOW_SIZE_Y/2));
	frame.setFillColor(sf::Color(139,90,43));
	frame.setOutlineThickness(3);
	frame.setOutlineColor(sf::Color::Black);
	frame.setOrigin(0,frame.getGlobalBounds().height/2);
	frame.setPosition(-1,WINDOW_SIZE_Y/2);

	questName = sf::Text("keine Quest gefunden",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),24);
	questName.setOrigin(questName.getGlobalBounds().width/2,questName.getGlobalBounds().height/2);
	questName.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4+20);
	questName.setOutlineColor(sf::Color::Black);
	questName.setOutlineThickness(2);

	questDescription = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	questDescription.setPosition(WINDOW_SIZE_X/5,WINDOW_SIZE_Y/3+15);
	questDescription.setOutlineColor(sf::Color::Black);
	questName.setOutlineColor(sf::Color::Black);
	questName.setOutlineThickness(1);

	desFrame.setSize(sf::Vector2f(WINDOW_SIZE_X/1.5, WINDOW_SIZE_Y/3.5));
	desFrame.setFillColor(sf::Color(255,255,0,50));
	desFrame.setOutlineThickness(3);
	desFrame.setOutlineColor(sf::Color(255,255,0));
	desFrame.setPosition(WINDOW_SIZE_X/5-20,WINDOW_SIZE_Y/3);

	questStatus = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),24);
	questStatus.setOrigin(questStatus.getGlobalBounds().width/2,questStatus.getGlobalBounds().height/2);
	questStatus.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/2+WINDOW_SIZE_Y/5-15);
	questStatus.setOutlineColor(sf::Color::Black);
	questStatus.setOutlineThickness(2);

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

void Quest_Screen::getQuests()
{
	questList.clear();

	std::list<Quest*> allQuests = Quest_Manager::getMe().getQuestList();

	for(auto cursor : allQuests)
	{
		if(cursor->getQuestStatus() == Quest::questStatus::InProgress)
		{
			questList.push_back(cursor);
		}
	}

	for(auto cursor : allQuests)
	{
		if(cursor->getQuestStatus() == Quest::questStatus::Solved)
		{
			questList.push_back(cursor);
		}
	}

	if(questList.size() > 0)
	{
		questCursor = questList.begin();
		setQuestInfo();
	}
}

void Quest_Screen::setQuestInfo()
{
	if(questList.size() > 0)
	{
		questName.setString((*questCursor)->getQuestName());
		questName.setOrigin(questName.getGlobalBounds().width/2,questName.getGlobalBounds().height/2);

		std::string tempDes = Message_Manager::getMe().splitText((*questCursor)->getQuestDescription(),23);
		questDescription.setString(tempDes);

		if((*questCursor)->getQuestStatus() == Quest::questStatus::Solved)
		{
			questStatus.setString("Geschafft!");
			desFrame.setFillColor(sf::Color(0,255,0,50));
			desFrame.setOutlineColor(sf::Color(0,255,0));
			questStatus.setFillColor(sf::Color(0,200,0));
		}
		else
		{
			questStatus.setString("Noch nicht geschafft!");
			questStatus.setFillColor(sf::Color::Red);

			if((*questCursor)->getQuestType() == Quest::questType::MainQuest)
			{
				desFrame.setFillColor(sf::Color(255,255,0,50));
				desFrame.setOutlineColor(sf::Color(255,255,0));
			}
			else
			{
				desFrame.setFillColor(sf::Color(0,255,255,50));
				desFrame.setOutlineColor(sf::Color(0,255,255));
			}
		}

		questStatus.setOrigin(questStatus.getGlobalBounds().width/2,questStatus.getGlobalBounds().height/2);
	}
}

Quest_Screen::~Quest_Screen(void)
{
}