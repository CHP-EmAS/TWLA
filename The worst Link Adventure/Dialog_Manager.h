#pragma once

#include <string>
#include <list>
#include <map>

#include "Defines.h"

class Dialog_Manager : public Singleton<Dialog_Manager>
{

friend class Singleton<Dialog_Manager>;

public:
	struct dialogSection
	{
		std::list<fullCommand> commands;
		std::list<std::string> requirements;
	};

	struct dialogQuestion
	{
		std::string questionName;
		std::string questionText;

		std::map<std::string,std::list<fullCommand>> answerCommands;
	};

	struct dialog
	{
		int ID;
		std::list<dialogSection*> sections;
		std::list<dialogQuestion*> questions;
	};

	void doDialog(unsigned short dialogID);

	
	~Dialog_Manager(void);
private:
	Dialog_Manager(void);

	bool loadDialog(unsigned short dialogID);
	void deleteCurrentDialog();

	short getRightDialogSection();

	void doDialogSection(short dialogSectionNumber);
	void doDialogCommands(std::list<fullCommand> commandList);

	dialog *currentDialog;
	std::map<int,std::string> dialog_Paths;
};

