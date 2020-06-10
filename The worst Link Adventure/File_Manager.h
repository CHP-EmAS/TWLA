#pragma once

#include <string>
#include <list>
#include <iostream>
#include <fstream>

#include "Defines.h"

class File_Manager : public Singleton<File_Manager>
{

friend class Singleton<File_Manager>;

public:
	std::string create_SaveGame(std::string name, int difficulty);
	bool load_Savegame(std::string saveGamePath);
	bool save_Savegame();

	sf::Image doScreenShotFromPlayer();

	bool loadCurrentMapChanges();
	bool saveCurrentMapChanges();
	
	bool saveSettings();
	bool loadSettings();

	std::list<fullCommand> convertFileToCommandlist(std::string filePath);

	std::string getPlayerName();
	int getGameDifficulty();

	std::ofstream& getLogStream();

	void forceReset();

	~File_Manager(void);
private:
	File_Manager(void);
	
	struct saveGame
	{
		std::string path;
		std::string name;
		std::string date;
		int difficulty;
	};

	saveGame currentSaveGame;
	std::ofstream logStream;
};

