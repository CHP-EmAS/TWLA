#pragma once

#include <list>

#include "Defines.h"


class Quest
{
public:
	Quest(std::string fileQuestPath);

	enum questType
	{
		MainQuest,
		SideQuest,
		HiddenQuest,
		AchievementQuest
	};

	enum questStatus
	{
		Hidden,
		InProgress,
		Solved,
		Cancled,
		Error
	};

	struct questCommand
	{
		std::string command;
		bool solved;
	};

	bool checkRequirement(std::string requirement);
	bool checkTask(std::string requirement);

	void recheckQuest();

	questStatus getQuestStatus();
	questType getQuestType();
	short getQuestID();
	std::string getQuestName();
	std::string getQuestDescription();

	std::string getSaveData();
	bool loadSaveData(std::string data);

	~Quest(void);
private:
	void loadQuest(std::string fileQuestPath);

	std::list<questCommand*> questRequirements;
	std::list<questCommand*> questTasks;

	std::list<std::string> solveCommands;

	std::string questDescription;

	short questID;
	std::string questName;

	questType type;
	questStatus status;

	bool do_Requirements_first;
};

