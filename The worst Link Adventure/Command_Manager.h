#pragma once

#include <string>
#include <algorithm>

#include "SFML/System.hpp"

#include "Defines.h"

class Command_Manager : public Singleton<Command_Manager>
{

friend class Singleton<Command_Manager>;

public:
	void doCommand(std::string);
	std::string replace$Strings(std::string replaceString);
	void enterCommand();

	void wait(sf::Time waitTime);

	~Command_Manager(void);
private:
	Command_Manager(void);
};

