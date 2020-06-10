#include "Quest.h"

#include "Quest_Manager.h"
#include "File_Manager.h"
#include "Message_Manager.h"

#include <fstream>
#include <sstream>
#include <string>

Quest::Quest(std::string fileQuestPath)
{
	NEWOUT("Quest <" << fileQuestPath << "> wird erstellt!")

	questDescription = "Empty";
	questID = -1;
	questName = "Empty";
	do_Requirements_first = false;

	loadQuest(fileQuestPath);
}

void Quest::loadQuest(std::string fileQuestPath)
{
	if(fileQuestPath.substr(fileQuestPath.length() - 7) != ".zQuest")
	{
		FOUT("Unbekannte Dateinendung <" << fileQuestPath << ">! Benötigt: .zQuest")

		status = questStatus::Error;
		type = questType::HiddenQuest;

		return;
	}

	std::list<fullCommand> questData = File_Manager::getMe().convertFileToCommandlist(fileQuestPath);

	for(auto cursor : questData)
	{
		if(cursor.command == "name")
		{
			this->questName = cursor.content;
		}
		else if(cursor.command == "description")
		{
			this->questDescription = cursor.content;
		}
		else if(cursor.command == "ID")
		{
			this->questID = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "type")
		{
			switch(cursor.content[0])
			{
			case 'm':
			case 'M':
				type = questType::MainQuest;
				break;
			case 's':
			case 'S':
				type = questType::SideQuest;
				break;
			case 'a':
			case 'A':
				type = questType::AchievementQuest;
				break;
			case 'h':
			case 'H':
				type = questType::HiddenQuest;
				break;
			default:
				FOUT("Unbekannter Questtyp <" << cursor.content[0] << ">! Typ H wird verwendet.")
				type = questType::HiddenQuest;
				break;
			}
		}
		else if(cursor.command == "status")
		{
			switch(cursor.content[0])
			{
			case 'h':
			case 'H':
				status = questStatus::Hidden;
				break;
			case 'p':
			case 'P':
				status = questStatus::InProgress;
				break;
			case 's':
			case 'S':
				status = questStatus::Solved;
				break;
			case 'c':
			case 'C':
				status = questStatus::Cancled;
				break;
			case 'e':
			case 'E':
				status = questStatus::Error;
				break;
			default:
				FOUT("Unbekannter Queststatus <" << cursor.content[0] << ">! Typ E wird verwendet.")
				status = questStatus::Error;
				break;
			}
		}
		else if(cursor.command == "setting")
		{
			do_Requirements_first = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "requirement")
		{
			questCommand *newRequirement = new questCommand;

			newRequirement->command = cursor.content;
			newRequirement->solved = false;
		
			questRequirements.push_back(newRequirement);
		}
		else if(cursor.command == "task")
		{
			questCommand *newTask = new questCommand;
			
			newTask->command = cursor.content;
			newTask->solved = false;
		
			questTasks.push_back(newTask);
		}
		else if(cursor.command == "command")
		{

		}
	}

	SOUT("Quest " << questName << " (" << type << "," << status << ") wurde geladen.")

	recheckQuest();
}

bool Quest::checkRequirement(std::string requirement)
{
	if(status == questStatus::Error || status == questStatus::InProgress) return false;

	bool update = false;

	for(auto cursor : questRequirements)
	{
		if(requirement == cursor->command && !cursor->solved)
		{
			cursor->solved = true;
			recheckQuest();
			return true;
		}
	}

	return false;
}

bool Quest::checkTask(std::string task)
{
	if(do_Requirements_first && status == questStatus::Hidden || status == questStatus::Error) return false;

	for(auto cursor : questTasks)
	{
		if(task == cursor->command && !cursor->solved)
		{
			cursor->solved = true;
			recheckQuest();
			return true;
		}
	}

	return false;
}

void Quest::recheckQuest()
{
	if(status == questStatus::Error || status == questStatus::Cancled) return;

	bool allRequirements = true;
	bool allTasks = true;

	for(auto cursor : questRequirements)
	{
		if(cursor->solved == false)
			allRequirements = false;
	}

	for(auto cursor : questTasks)
	{
		if(cursor->solved == false)
			allTasks = false;
	}

	if(allRequirements && allTasks)
	{
		status = questStatus::Solved;
		IOUT("Quest <" << questName << "> wurde abgeschlossen!")
		Message_Manager::getMe().showLocationMsg("Check: " + questName);

		std::string command = "qSolved-";
		command += std::to_string(questID);

		Quest_Manager::getMe().doQuestCommand(command);
	}
	else if(allRequirements && !allTasks && status == questStatus::Hidden)
	{
		status = questStatus::InProgress;
		IOUT("Quest <" << questName << "> wurde entdeckt!")
		Message_Manager::getMe().showLocationMsg("Quest: " + questName);

		std::string command = "qProgress-";
		command += std::to_string(questID);

		Quest_Manager::getMe().doQuestCommand(command);
	}
	else if(!allRequirements && !allTasks)
	{
		status = questStatus::Hidden;
	}
}

Quest::questStatus Quest::getQuestStatus()
{
	return status;
}

Quest::questType Quest::getQuestType()
{
	return type;
}

short Quest::getQuestID()
{
	return questID;
}

std::string Quest::getQuestName()
{
	return questName;
}

std::string Quest::getQuestDescription()
{
	return questDescription;
}

std::string Quest::getSaveData()
{
	std::string data = "";

	data += std::to_string(status) + ",";

	for(auto cursor : questRequirements)
	{
		data += std::to_string(cursor->solved) + ",";
	}

	for(auto cursor : questTasks)
	{
		data += std::to_string(cursor->solved) + ",";
	}

	return data;
}

bool Quest::loadSaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_questStatus = data.substr(0,pos);

	for(auto cursor : questRequirements)
	{
		data = data.substr(pos+1);
		pos = data.find(",");
		std::string str_solve = data.substr(0,pos);

		cursor->solved = bool(atoi(str_solve.c_str()));
	}

	for(auto cursor : questTasks)
	{
		data = data.substr(pos+1);
		pos = data.find(",");
		std::string str_solve = data.substr(0,pos);

		cursor->solved = bool(atoi(str_solve.c_str()));
	}

	this->status = Quest::questStatus(atoi(str_questStatus.c_str()));

	return true;
}

Quest::~Quest(void)
{
	DELOUT("Quest <" << questName << "> wird gelöscht!")

	for(auto cursor : questRequirements)
	{
		delete cursor;
	}

	for(auto cursor : questTasks)
	{
		delete cursor;
	}
}
