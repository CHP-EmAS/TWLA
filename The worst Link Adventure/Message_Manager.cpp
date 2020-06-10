#include "Message_Manager.h"

#include "Graphic_Manager.h"
#include "Screen_Manager.h"
#include "Sound_Manager.h"
#include "Command_Manager.h"

#include <thread>

Message_Manager::Message_Manager(void)
{
	float windowSizex = WINDOW_SIZE_X;
	float windowSizey = WINDOW_SIZE_Y;

	//Speaker
	speakerNameText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),12);
	speakerNameText.setFillColor(sf::Color(255, 255, 255,255));
	speakerNameText.setPosition(windowSizex / 30 + 5,windowSizey - windowSizey / 30 - windowSizey / 5 - 20);

	speakerNameFrame.setSize(sf::Vector2f(speakerNameText.getGlobalBounds().width + 10, 20));
	speakerNameFrame.setPosition(windowSizex / 30,windowSizey - windowSizey / 30 - windowSizey / 5 - 23);
	speakerNameFrame.setFillColor(sf::Color(50, 50, 50,200));
	speakerNameFrame.setOutlineColor(sf::Color(100, 100, 100));
	speakerNameFrame.setOutlineThickness(3);

	//Msg
	msgText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);
	msgText.setFillColor(sf::Color(255, 255, 255,255));
	msgText.setPosition(windowSizex / 30 + 15,windowSizey - windowSizey / 30 - windowSizey / 5 + 10);

	msgFrame.setSize(sf::Vector2f(windowSizex - (windowSizex / 15),windowSizey / 5));
	msgFrame.setPosition(windowSizex / 30,windowSizey - windowSizey / 30 - windowSizey / 5);
	msgFrame.setFillColor(sf::Color(50, 50, 50,200));
	msgFrame.setOutlineColor(sf::Color(100, 100, 100));
	msgFrame.setOutlineThickness(3);

	//Hint
	hintText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),14);
	hintText.setFillColor(sf::Color(255, 255, 255,255));

	hintMsgFrame.setFillColor(sf::Color(0,0,0,150));
	hintMsgFrame.setOutlineColor(sf::Color(100,100,100));
	hintMsgFrame.setOutlineThickness(1);

	//Location
	locationText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),32);
	locationText.setFillColor(sf::Color(255, 255, 255,255));

	locationFrame.setSize(sf::Vector2f(windowSizex,50));
	locationFrame.setPosition(0,windowSizey/2 - 25);
	locationFrame.setFillColor(sf::Color(50,50,50,150));
	locationFrame.setOutlineColor(sf::Color(200,200,200));
	locationFrame.setOutlineThickness(3);

	//Question
	questionFrame.setSize(sf::Vector2f(windowSizex - (windowSizex / 3),windowSizey / 5));
	questionFrame.setPosition(windowSizex / 30,windowSizey - windowSizey / 30 - windowSizey / 5);
	questionFrame.setFillColor(sf::Color(50, 50, 50,200));
	questionFrame.setOutlineColor(sf::Color(100, 100, 100));
	questionFrame.setOutlineThickness(3);
	
	answerFrame.setSize(sf::Vector2f(windowSizex / 4,windowSizey / 5));
	answerFrame.setPosition(questionFrame.getPosition().x + questionFrame.getSize().x + 10,windowSizey - windowSizey / 30 - windowSizey / 5);
	answerFrame.setFillColor(sf::Color(50, 50, 50,200));
	answerFrame.setOutlineColor(sf::Color(100, 100, 100));
	answerFrame.setOutlineThickness(3);

	answerText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),18);

	//save icon
	spr_saveIcon.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_saveIcon.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,282));
	spr_saveIcon.setScale(3,3);
	spr_saveIcon.setColor(sf::Color(255,255,255,100));
	spr_saveIcon.setPosition(WINDOW_SIZE_X - spr_saveIcon.getGlobalBounds().width - 10,WINDOW_SIZE_X - spr_saveIcon.getGlobalBounds().height - 10);
	
	NEWOUT("Neuer Message_Manager wurde erstellt! (" << this << ")")
}

std::string Message_Manager::showQuestion(std::string str_question,sf::Color color,std::string answer1,std::string answer2,std::string answer3,std::string answer4,std::string speakerName)
{
	if(speakerName != "")
	{
		speakerNameText.setString(speakerName);
		speakerNameFrame.setSize(sf::Vector2f(speakerNameText.getGlobalBounds().width + 10, 20));
	}

	str_question = Command_Manager::getMe().replace$Strings(str_question);
	str_question = splitText(str_question,25);

	sf::RenderWindow &mainWindow = Screen_Manager::getMe().getGameWindow();
	msgText.setFillColor(color);
	answerText.setFillColor(color);

	std::string drawText = "";
	int selectedAnswer = 0;

	while(true)
	{
		//Update
		if(typeClock.getElapsedTime().asMilliseconds() >= 50)
		{
			if(drawText.size() < str_question.size())
			{
				do
				{
					drawText = drawText + str_question[drawText.size()];
				}
				while(str_question[drawText.size()] == ' ' || str_question[drawText.size()] == '\n');

				if(Sound_Manager::getMe().getDoDialogSound()) Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Text_Letter);
			}
	
			msgText.setString(drawText);

			typeClock.restart();
		}

		sf::Event events;
		while(mainWindow.pollEvent(events))
		{
			switch(events.type)
			{
			case sf::Event::KeyPressed:
			
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					drawText = str_question;
					msgText.setString(str_question);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
				{
					if(selectedAnswer > 0) selectedAnswer--;
					else
					{
						if(answer4 != "")
							selectedAnswer = 3;
						else if(answer3 != "")
							selectedAnswer = 2;
						else
							selectedAnswer = 1;
					}
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
				{
					if(selectedAnswer == 0)
						selectedAnswer++;
					else if(selectedAnswer == 1)
					{
						if(answer3 != "") selectedAnswer++;
						else selectedAnswer = 0;
					}
					else if(selectedAnswer == 2)
					{
						if(answer4 != "") selectedAnswer++;
						else selectedAnswer = 0;
					}
					else if(selectedAnswer == 3)
						selectedAnswer = 0;
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
				{
					switch(selectedAnswer)
					{
					case 0:
						return answer1;
						break;
					case 1:
						return answer2;
						break;
					case 2:
						return answer3;
						break;
					case 3:
						return answer4;
						break;
					}
				}

				break;
			case sf::Event::Closed:
				Screen_Manager::getMe().getGameWindow().close();
				break;
			}
		}

		//Draw
		mainWindow.clear();

		if(Screen_Manager::getMe().getActivScreen() != nullptr)
			Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

		mainWindow.draw(questionFrame);
		mainWindow.draw(msgText);

		if(speakerName != "")
		{
			mainWindow.draw(speakerNameFrame);
			mainWindow.draw(speakerNameText);
		}

		mainWindow.draw(answerFrame);

		if(selectedAnswer == 0) answerText.setFillColor(sf::Color::Green);
		else answerText.setFillColor(sf::Color::White);

		answerText.setString(answer1);
		answerText.setPosition(answerFrame.getPosition().x + answerFrame.getSize().x/2 - answerText.getGlobalBounds().width/2,answerFrame.getPosition().y+10);
		mainWindow.draw(answerText);

		if(selectedAnswer == 1) answerText.setFillColor(sf::Color::Green);
		else answerText.setFillColor(sf::Color::White);

		answerText.setString(answer2);
		answerText.setPosition(answerFrame.getPosition().x + answerFrame.getSize().x/2 - answerText.getGlobalBounds().width/2,answerFrame.getPosition().y+35);
		mainWindow.draw(answerText);

		if(answer3 != "")
		{
			if(selectedAnswer == 2) answerText.setFillColor(sf::Color::Green);
			else answerText.setFillColor(sf::Color::White);

			answerText.setString(answer3);
			answerText.setPosition(answerFrame.getPosition().x + answerFrame.getSize().x/2 - answerText.getGlobalBounds().width/2,answerFrame.getPosition().y+60);
			mainWindow.draw(answerText);
		}

		if(answer4 != "")
		{
			if(selectedAnswer == 3) answerText.setFillColor(sf::Color::Green);
			else answerText.setFillColor(sf::Color::White);

			answerText.setString(answer4);
			answerText.setPosition(answerFrame.getPosition().x + answerFrame.getSize().x/2 - answerText.getGlobalBounds().width/2,answerFrame.getPosition().y+85);
			mainWindow.draw(answerText);
		}

		mainWindow.display();

		//Update
		Sound_Manager::getMe().updateManager();

		//Wait
		std::this_thread::sleep_for(std::chrono::milliseconds(50-typeClock.getElapsedTime().asMilliseconds()));
	}

	return answer1;
}

void Message_Manager::showMsg(std::string str_msg,sf::Color color,std::string speakerName)
{
	if(speakerName != "")
	{
		speakerNameText.setString(speakerName);
		speakerNameFrame.setSize(sf::Vector2f(speakerNameText.getGlobalBounds().width + 10, 20));
	}

	str_msg = Command_Manager::getMe().replace$Strings(str_msg);
	str_msg = splitText(str_msg,36);

	sf::RenderWindow &mainWindow = Screen_Manager::getMe().getGameWindow();
	msgText.setFillColor(color);

	std::string drawText = "";

	sf::Event events;

	while(true)
	{
		//Update
		if(typeClock.getElapsedTime().asMilliseconds() >= 50)
		{
			if(drawText.size() < str_msg.size())
			{
				do
				{
					drawText = drawText + str_msg[drawText.size()];
				}
				while(str_msg[drawText.size()] == ' ' || str_msg[drawText.size()] == '\n');

				if(Sound_Manager::getMe().getDoDialogSound()) Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Text_Letter);
			}
			else
			{
				while(!sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					mainWindow.pollEvent(events);
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
				}

				while(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					mainWindow.pollEvent(events);
					std::this_thread::sleep_for(std::chrono::milliseconds(20));
				}


				break;
			}
				
			msgText.setString(drawText);

			typeClock.restart();
		}

		while(mainWindow.pollEvent(events))
		{
			switch(events.type)
			{
			case sf::Event::KeyPressed:
			
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
				{
					drawText = str_msg;
					msgText.setString(str_msg);

					Screen_Manager::getMe().getGameWindow().clear();

					if(Screen_Manager::getMe().getActivScreen() != nullptr)
						Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

					if(speakerName != "")
					{
						mainWindow.draw(speakerNameFrame);
						mainWindow.draw(speakerNameText);
					}

					mainWindow.draw(msgFrame);
					mainWindow.draw(msgText);

					mainWindow.display();

					while(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
					{
						mainWindow.pollEvent(events);
						std::this_thread::sleep_for(std::chrono::milliseconds(20));
					}
				}

				break;
			case sf::Event::Closed:
				Screen_Manager::getMe().getGameWindow().close();
				break;
			}
		}

		//Draw
		Screen_Manager::getMe().getGameWindow().clear();

		if(Screen_Manager::getMe().getActivScreen() != nullptr)
			Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

		if(speakerName != "")
		{
			mainWindow.draw(speakerNameFrame);
			mainWindow.draw(speakerNameText);
		}

		mainWindow.draw(msgFrame);
		mainWindow.draw(msgText);

		mainWindow.display();

		//Update
		Sound_Manager::getMe().updateManager();

		//Wait
		std::this_thread::sleep_for(std::chrono::milliseconds(50-typeClock.getElapsedTime().asMilliseconds()));
	}
}

void Message_Manager::showShortHintMsg(std::string str_msg,sf::Color color)
{
	str_msg = Command_Manager::getMe().replace$Strings(str_msg);

	hintText.setString(str_msg);
	hintText.setFillColor(color);
	
	float windowSizex = WINDOW_SIZE_X;
	float windowSizey = WINDOW_SIZE_Y;

	hintText.setPosition(windowSizex/2 - hintText.getGlobalBounds().width/2,30);

	hintMsgFrame.setPosition(hintText.getPosition() - sf::Vector2f(5,1));
	hintMsgFrame.setSize(sf::Vector2f(hintText.getGlobalBounds().width+10,hintText.getGlobalBounds().height+8));

	showHintClock.restart();
}

void Message_Manager::showLocationMsg(std::string str_location,sf::Color color)
{
	str_location = Command_Manager::getMe().replace$Strings(str_location);

	locationText.setString(str_location);
	locationText.setFillColor(color);
	
	locationText.setPosition(WINDOW_SIZE_X/2 - locationText.getGlobalBounds().width/2,locationFrame.getPosition().y);

	showLocationClock.restart();
}

void Message_Manager::showSaveIcon()
{
	showSaveIconClock.restart();
}

void Message_Manager::drawMessages(sf::RenderWindow &mainWindow)
{
	if(showHintClock.getElapsedTime().asSeconds() <= 2 && hintText.getString() != "")
	{
		mainWindow.draw(hintMsgFrame);
		mainWindow.draw(hintText);
	}

	if(showLocationClock.getElapsedTime().asSeconds() <= 3 && locationText.getString() != "")
	{
		mainWindow.draw(locationFrame);
		mainWindow.draw(locationText);
	}

	if(showSaveIconClock.getElapsedTime().asSeconds() <= 2)
	{
		mainWindow.draw(spr_saveIcon);
	}
}

std::string Message_Manager::splitText(sf::String splitText,int splitLength)
{
	if(splitText.getSize() <= splitLength-1) return splitText;

	int lastSpacePosition = -1;

	int countPosition = 0;
	int lineCount = 1;

	for(int i = 0;i < splitText.getSize();i++)
	{
		if(splitText[i] == ' ')
			lastSpacePosition = i;
		else if(splitText[i] == '\n')
			countPosition = 0;

		if(countPosition == splitLength)
		{
			if(lastSpacePosition != -1)
			{
				if(splitText[i+1] == ' ')
				{
					splitText[++i] = '\n';
				}
				else
				{
					splitText[lastSpacePosition] = '\n';
					i = lastSpacePosition+1;
				}
				
				lastSpacePosition = -1;
				countPosition = 1;
			}
			else
			{
				splitText.insert(i,"\n");
				i++;
				countPosition = 0;
			}
			
			lineCount++;
		}

		countPosition++;
	}

	return splitText;
}

Message_Manager::~Message_Manager(void)
{
	DELOUT("Message_Manager wurde geloescht!\t(" << this << ")")
}
