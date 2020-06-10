#pragma once

#include "SFML/Graphics.hpp"

#include "Defines.h"

class InGame_Stats
{
public:
	InGame_Stats(void);

	//methoden
	void update();

	std::string getSaveData();
	bool loadSaveData(std::string data);

	//setter
	void addStep();
	void addShotedArrow();
	void addMonsterKill();
	void addPlacedBomb();
	void addSwordHits();
	void addCuttetGrass();

	//getter
	std::string getStatsString();

	~InGame_Stats(void);
private:
	long gameTime_sec;
	long stepCounter;
	int monsterKills;
	int shotedArrows;
	int placedBombs;
	int swordHits;
	int cuttetGrass;

	sf::Clock counter;
};