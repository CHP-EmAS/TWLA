#include "Quest_Manager.h"
#include "File_Manager.h"

#include <fstream>
#include <conio.h>

Quest_Manager::Quest_Manager()
{
	 loadQuests();

	 NEWOUT("Neuer Quest_Manager wurde erstellet! (" << this << ")");
}

std::list<Quest*> Quest_Manager::getQuestList()
{
	std::list<Quest*> allQuests;

	for(auto cursor : questList)
	{
		allQuests.push_back(cursor.second);
	}

	return allQuests;
}

bool Quest_Manager::doQuestCommand(std::string command)
{
	bool commandSuccess = false;

	DOUT("Quest-Command: " << command)

	for(auto cursor : questList)
	{
		if(!(cursor.second->getQuestStatus() == Quest::questStatus::Solved))
		{
			commandSuccess = cursor.second->checkRequirement(command);
			commandSuccess = cursor.second->checkTask(command);
		}
	}

	doneQuestCommands.push_back(command);

	return commandSuccess;
}

bool Quest_Manager::isQuestCommandDone(std::string command)
{
	for(auto cursor : doneQuestCommands)
	{
		if(cursor == command) 
			return true;
	}

	return false;
}

std::string Quest_Manager::getQuest_ManagerSaveData()
{
	std::string data = "";

	for(auto cursor : doneQuestCommands)
	{
		data += "{command}" + cursor + "\n";
	}

	for(auto cursor : questList)
	{
		data += "{quest}" + std::to_string(cursor.second->getQuestID()) + "," + cursor.second->getSaveData() + "\n";
	}

	return data;
}

bool Quest_Manager::loadQuest_ManagerSaveData(std::string path)
{
	std::list<fullCommand> saveDatas = File_Manager::getMe().convertFileToCommandlist(path);

	for(auto cursor : saveDatas)
	{
		if(cursor.command == "command")
		{
			doneQuestCommands.push_back(cursor.content);
		}
		else if(cursor.command == "quest")
		{
			int pos = cursor.content.find(",");
			std::string str_questID = cursor.content.substr(0,pos);
			cursor.content = cursor.content.substr(pos+1);

			int id = atoi(str_questID.c_str());

			std::map<int,Quest*>::iterator it;
			it = questList.find(id);

			if(it == questList.end())
			{
				FOUT("Quest-ID: " << id << " konnte nicht gefunden werden!");
			}
			else
			{
				questList[id]->loadSaveData(cursor.content);
			}
		}
	}

	return true;
}

void Quest_Manager::loadQuests()
{
	delQuests();

	IOUT("Quests werden geladen...")

	LPCWSTR dir = TEXT("lists/Quests/*.zQuest*");

	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = INVALID_HANDLE_VALUE;

	//Zur ersten Datei im Ordner springen
	hFind = FindFirstFile(dir, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			//Convert
			char ch[260];
			WideCharToMultiByte(CP_ACP,0,FindFileData.cFileName,-1, ch,260,NULL, NULL);
    
			std::string path(ch);

			path = "lists/Quests/" + path;

			Quest *newQuest = new Quest(path);

			std::map<int,Quest*>::iterator it;
			it = questList.find(newQuest->getQuestID());

			if(it != questList.end())
			{
				FOUT("Quest-ID: " << newQuest->getQuestID() << " ist doppelt definiert!");
				WOUT("Quest " << newQuest->getQuestID() << " wird überschrieben!");
			}

			questList[newQuest->getQuestID()] = newQuest;
		}
		while(FindNextFile(hFind, &FindFileData));

		FindClose(hFind);
	}	
}

void Quest_Manager::delQuests()
{
	for(auto cursor : questList)
	{
		delete cursor.second;
	}

	questList.clear();
}

Quest_Manager::~Quest_Manager()
{
	delQuests();
	DELOUT("Quest_Manager wurde geloescht!\t(" << this << ")")
}



