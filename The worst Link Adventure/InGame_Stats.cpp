#include "InGame_Stats.h"

#include "Graphic_Manager.h"
#include "Quest_Manager.h"
#include "Player_Manager.h"

#include <sstream>

InGame_Stats::InGame_Stats(void)
{
	gameTime_sec = 0;
	stepCounter = 0;
	shotedArrows = 0;
	monsterKills = 0;
	placedBombs = 0;
	swordHits = 0;
	cuttetGrass = 0;

	counter.restart();
}

//methoden
void InGame_Stats::update()
{
	if(counter.getElapsedTime().asMilliseconds() > 1000)
	{
		gameTime_sec += counter.getElapsedTime().asSeconds();
		counter.restart();
	}
}

std::string InGame_Stats::getSaveData()
{
	std::string fullData = "";

	fullData += std::to_string(gameTime_sec) + ",";
	fullData += std::to_string(stepCounter) + ",";
	fullData += std::to_string(shotedArrows) + ",";
	fullData += std::to_string(monsterKills) + ",";
	fullData += std::to_string(placedBombs) + ",";
	fullData += std::to_string(swordHits) + ",";
	fullData += std::to_string(cuttetGrass) + ",";

	return fullData;
}

bool InGame_Stats::loadSaveData(std::string data)
{
	int cutPos;

	for(int i = 0; i < 7; i++)
	{
		cutPos = data.find(",");
		std::string c_data = data.substr(0,cutPos);
		data = data.substr(cutPos+1);

		switch(i)
		{
		case 0:
			gameTime_sec = atol(c_data.c_str());
			break;
		case 1:
			stepCounter = atol(c_data.c_str());
			break;
		case 2:
			shotedArrows = atoi(c_data.c_str());
			break;
		case 3:
			monsterKills = atoi(c_data.c_str());
			break;
		case 4:
			placedBombs = atoi(c_data.c_str());
			break;
		case 5:
			swordHits = atoi(c_data.c_str());
			break;
		case 6:
			cuttetGrass = atoi(c_data.c_str());
			break;
		}
	}

	return true;
}

//setter
void InGame_Stats::addStep()
{
	stepCounter++;
}

void InGame_Stats::addShotedArrow()
{
	shotedArrows++;
}

void InGame_Stats::addMonsterKill()
{
	monsterKills++;
}

void InGame_Stats::addPlacedBomb()
{
	placedBombs++;
}

void InGame_Stats::addSwordHits()
{
	swordHits++;
}

void InGame_Stats::addCuttetGrass()
{
	cuttetGrass++;
}

//getter
std::string InGame_Stats::getStatsString()
{
	std::string content;

	content =  std::to_string(int(gameTime_sec / 3600)) + ":" + std::to_string(int((gameTime_sec / 60)%60)) + ":" + std::to_string(int(gameTime_sec % 60)) + "\n";
	content += std::to_string(stepCounter) + "\n";
	content += std::to_string(shotedArrows) + "\n";
	content += std::to_string(placedBombs) + "\n";
	content += std::to_string(swordHits) + "\n";
	content += std::to_string(monsterKills) + "\n";
	content += std::to_string(cuttetGrass) + "\n";
	content += std::to_string(Player_Manager::getMe().getGameHUD().getMaxLive()) + "|20\n";

	std::list<Quest*> quests = Quest_Manager::getMe().getQuestList();

	int solvedQuests = 0;

	for(auto cursor:quests)
	{
		if(cursor->getQuestStatus() == Quest::questStatus::Solved)
		{
			solvedQuests++;
		}
	}

	content += std::to_string(solvedQuests) + "|" + std::to_string(quests.size()) + "\n";

	return content;
}

InGame_Stats::~InGame_Stats()
{

}