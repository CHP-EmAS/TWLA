#pragma once

#include "Defines.h"

#include <list>

#include "Quest.h"


class Quest_Manager : public Singleton<Quest_Manager>
{

friend class Singleton<Quest_Manager>;

public:
	std::list<Quest*> getQuestList();

	bool doQuestCommand(std::string command);
	bool isQuestCommandDone(std::string command);

	std::string getQuest_ManagerSaveData();
	bool loadQuest_ManagerSaveData(std::string path);

	~Quest_Manager();
private:
	Quest_Manager(void);

	void loadQuests();
	void delQuests();

	std::map<int,Quest*> questList;

	std::list<std::string> doneQuestCommands;
};