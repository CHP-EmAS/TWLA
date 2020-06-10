#pragma once

#include "SFML/Graphics.hpp"

#include "Defines.h"

#include "Item_List.h"

class HUD
{
public:
	HUD(void);

	//methoden
	void draw(sf::RenderWindow &mainWindow);

	std::string getHUDSaveData();
	bool loadHUDSaveData(std::string data);

	//setter
	void setLive(float live);
	void setMaxLive(int live);

	void setEnergy(float energy);
	void setMaxEnergy(int energy);

	void addEdelsteine(int amount);
	void removeEdelsteine(int amount);

	//getter
	float getLive();
	int getMaxLive();

	float getEnergy();
	int getMaxEnergy();

	int getEdelsteine();

	sf::Sprite &getSelectedWeaponSprite();

	~HUD(void);
private:
	const int windowSizeX;
	const int windowSizeY;
	float hudScale;
	
	float live;
	sf::Sprite heart;

	int edelsteine;
	sf::Sprite edelstein;
	sf::Text edelsteineText;
	sf::RectangleShape edelsteinFrame;

	int maxLive;
	int selectedItemNumber;
	sf::Sprite selectedItemSprite;
	sf::Text selectedItemStack;
	sf::RectangleShape selectedItemFrame;

	sf::Sprite selectedWeaponSprite;

	float energy;
	int maxEnergy;
	sf::RectangleShape energyFrame;
	sf::RectangleShape energyShape;
};

