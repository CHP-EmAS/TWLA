#pragma once

#include <string>
#include <list>
#include "SFML/Graphics.hpp"

#include "Defines.h"

class Message_Manager : public Singleton<Message_Manager>
{

friend class Singleton<Message_Manager>;

public:
	
	std::string showQuestion(std::string str_question,sf::Color color,std::string answer1,std::string answer2,std::string answer3 = "",std::string answer4 = "",std::string speakerName = "");
	void showMsg(std::string str_msg,sf::Color color = sf::Color(255,255,255),std::string speakerName = "");
	void showShortHintMsg(std::string str_msg,sf::Color color = sf::Color(255,255,255));
	void showLocationMsg(std::string str_location,sf::Color color = sf::Color(255,255,255));
	void showSaveIcon();

	void drawMessages(sf::RenderWindow &mainWindow);

	std::string splitText(sf::String splitText,int splitLength);

	~Message_Manager(void);
private:
	Message_Manager(void);
	
	sf::Text speakerNameText;
	sf::RectangleShape speakerNameFrame;

	sf::Text msgText;
	sf::RectangleShape msgFrame;
	sf::Clock typeClock;

	sf::Text hintText;
	sf::Clock showHintClock;
	sf::RectangleShape hintMsgFrame;

	sf::Text locationText;
	sf::Clock showLocationClock;
	sf::RectangleShape locationFrame;

	sf::RectangleShape questionFrame;
	sf::RectangleShape answerFrame;
	sf::Text answerText;

	sf::Sprite spr_saveIcon;
	sf::Clock showSaveIconClock;
};

