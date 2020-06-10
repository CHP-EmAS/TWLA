#include "HUD.h"

#include "Graphic_Manager.h"
#include "Screen_Manager.h"
#include "Inventory_Manager.h"
#include "Player_Manager.h"

#include <sstream>

HUD::HUD(void):
	windowSizeX(WINDOW_SIZE_X),
	windowSizeY(WINDOW_SIZE_Y)
{
	hudScale = 1;

	//Heart
	heart.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,18));
	heart.setScale(hudScale,hudScale);

	//Edelstein
	edelstein.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	edelstein.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,418));
	edelstein.setPosition(windowSizeX - 110,25);
	edelstein.setScale(2,2);

	edelsteineText = sf::Text("0000",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),20);
	edelsteineText.setPosition(edelstein.getPosition().x + 32,edelstein.getPosition().y + edelstein.getGlobalBounds().height / 2 -15);
	edelsteineText.setStyle(sf::Text::Bold);

	edelsteinFrame.setFillColor(sf::Color(50,50,50,150));
	edelsteinFrame.setSize(sf::Vector2f(edelstein.getGlobalBounds().width + 52,edelstein.getGlobalBounds().height + 6));
	edelsteinFrame.setPosition(edelstein.getPosition().x+2,edelstein.getPosition().y - 5);
	edelsteinFrame.setOutlineColor(sf::Color(150,150,150));
	edelsteinFrame.setOutlineThickness(2);

	edelsteine = 0;

	//Item
	selectedItemSprite = Item_List::getMe().getItemSprite(0);
	selectedItemSprite.setScale(2,2);
	selectedItemSprite.setPosition(windowSizeX - 150,23);

	selectedWeaponSprite = Item_List::getMe().getItemSprite(0);
	selectedWeaponSprite.setScale(2,2);
	selectedWeaponSprite.setPosition(windowSizeX - 179,39);
	selectedWeaponSprite.setOrigin(8,8);
	selectedWeaponSprite.setRotation(45);
	
	selectedItemFrame.setFillColor(sf::Color(50,50,50,150));
	selectedItemFrame.setOutlineColor(sf::Color(150,150,150));
	selectedItemFrame.setOutlineThickness(2);

	//energy;
	energyShape.setFillColor(sf::Color::Yellow);

	energyFrame.setFillColor(sf::Color(100,100,100,150));
	energyFrame.setSize(sf::Vector2f((TILESIZE+1)*hudScale*10,7));
	energyFrame.setOutlineColor(sf::Color(200,200,200));
	energyFrame.setOutlineThickness(1);

	selectedItemStack = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),15);
	selectedItemStack.setPosition(selectedItemSprite.getPosition().x+6,selectedItemSprite.getPosition().y+35);
	selectedItemStack.setStyle(sf::Text::Bold);

	setMaxLive(20);
	setLive(maxLive);
	setMaxEnergy(1000);
	setEnergy(maxEnergy);
}

//methoden
void HUD::draw(sf::RenderWindow &mainWindow)
{
	//Energy Bar
	float energyPercent = energy / maxEnergy;
	int green = 255 * energyPercent;

	energyShape.setFillColor(sf::Color(255,green,0));

	mainWindow.draw(energyFrame);
	mainWindow.draw(energyShape);

	//Edelstein Anzeige
	mainWindow.draw(edelsteinFrame);
	mainWindow.draw(edelstein);
	mainWindow.draw(edelsteineText);

	//Ausgewählte Waffe

	//get Slot-Amount
	int amountCharSize = 0;

	if(Inventory_Manager::getMe().getSelectedWeapon() != Inventory_Manager::slotID::None)
	{
		Inventory_Manager::slot slot = Inventory_Manager::getMe().getSlot(Inventory_Manager::getMe().getSelectedWeapon());

		if(slot.amount > 1 || Inventory_Manager::getMe().getSelectedWeapon() == Inventory_Manager::Bow ||  Inventory_Manager::getMe().getSelectedWeapon() == Inventory_Manager::Bombbow)
		{
			int amount = 0;

			if(Inventory_Manager::getMe().getSelectedWeapon() != Inventory_Manager::Bombbow)
			{
				amount = slot.amount;
			}
			else
			{
				amount = Inventory_Manager::getMe().getSlot(Inventory_Manager::Bomb).amount;
			}

			if(amount < 10 && amount >= 0) amountCharSize = 1;
			else if(amount < 100) amountCharSize = 2;
			else if(amount < 1000) amountCharSize = 3;

			std::stringstream ss;

			ss << amount;
			selectedItemStack.setString(ss.str());
			
			ss.str("");
			ss.clear();
		}
	}

	//Resize frame for amount Text
	switch(amountCharSize)
	{
	case 0:
		selectedItemFrame.setSize(sf::Vector2f(38,38));
		selectedItemFrame.setPosition(sf::Vector2f(windowSizeX - 153,20));
		break;
	case 1:
		selectedItemFrame.setSize(sf::Vector2f(49,38));
		selectedItemFrame.setPosition(sf::Vector2f(windowSizeX - 164,20));
		break;
	case 2:
		selectedItemFrame.setSize(sf::Vector2f(60,38));
		selectedItemFrame.setPosition(sf::Vector2f(windowSizeX - 175,20));
		break;
	case 3:
		selectedItemFrame.setSize(sf::Vector2f(71,38));
		selectedItemFrame.setPosition(sf::Vector2f(windowSizeX - 186,20));
		break;
	}

	//draw Frame
	mainWindow.draw(selectedItemFrame);

	if(Inventory_Manager::getMe().getSelectedWeapon() != Inventory_Manager::slotID::None)
	{
		Inventory_Manager::slot slot = Inventory_Manager::getMe().getSlot(Inventory_Manager::getMe().getSelectedWeapon());

		selectedItemSprite.setPosition(selectedItemFrame.getPosition() + sf::Vector2f(3,3));
		selectedItemSprite.setTexture(*slot.itemSprite.getTexture());
		selectedItemSprite.setTextureRect(slot.itemSprite.getTextureRect());

		mainWindow.draw(selectedItemSprite);

		if(amountCharSize > 0)
		{
			selectedItemStack.setPosition(selectedItemSprite.getPosition() + sf::Vector2f(32,7));
			mainWindow.draw(selectedItemStack);
		}
	}

	//Ausgewältes Schwert
	selectedItemFrame.setPosition(selectedItemFrame.getPosition().x - selectedItemSprite.getGlobalBounds().width - 13,selectedItemFrame.getPosition().y);
	selectedItemFrame.setSize(sf::Vector2f(38,38));
	
	mainWindow.draw(selectedItemFrame);

	if(Inventory_Manager::getMe().getSelectedSword() != Inventory_Manager::slotID::None)
	{
		selectedWeaponSprite.setPosition(selectedItemFrame.getPosition() + sf::Vector2f(19,19));
		selectedWeaponSprite.setTexture(*Inventory_Manager::getMe().getSlot(Inventory_Manager::getMe().getSelectedSword()).itemSprite.getTexture());
		selectedWeaponSprite.setTextureRect(Inventory_Manager::getMe().getSlot(Inventory_Manager::getMe().getSelectedSword()).itemSprite.getTextureRect());

		mainWindow.draw(selectedWeaponSprite);
	}

	
	//Herzen
	if(File_Manager::getMe().getGameDifficulty() == IMMORTAL) heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,342));
	else heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,252));

	for(int i = 0; i < maxLive;i++)
	{
		if(i < 10)
		{
			heart.setPosition((TILESIZE + 1) * hudScale * float(i) + 20,20);
		}
		else 
		{
			heart.setPosition((TILESIZE + 1) * hudScale * (float(i) - 10.f) + 20, TILESIZE * hudScale + 21);
		}

		if(i > 2 && File_Manager::getMe().getGameDifficulty() == HARDCORE) heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,392));

		if(i == int(live))
		{
			if(live - int(live) >= 0.75f)
				heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,18));
			else if(live - int(live) >= 0.5f)
				heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,17));
			else if(live - int(live) >= 0.25f)
				heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,16));
			else if(live - int(live) >= 0.f)
			{
				if(i > 2 && File_Manager::getMe().getGameDifficulty() == HARDCORE) heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,392));
				else heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,15));
			}
		}
		else if(i == int(live) + 1)
		{
			if(i > 2 && File_Manager::getMe().getGameDifficulty() == HARDCORE) heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,392));
			else heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,15));
		}

		mainWindow.draw(heart);
	}
}

std::string HUD::getHUDSaveData()
{
	std::string fullData;

	fullData += std::to_string(maxLive) + ",";
	fullData += std::to_string(live) + ",";
	fullData += std::to_string(maxEnergy) + ",";
	fullData += std::to_string(energy) + ",";
	fullData += std::to_string(edelsteine) + ",";

	return fullData;
}

bool HUD::loadHUDSaveData(std::string data)
{
	int cutPos;

	for(int i = 0; i < 6; i++)
	{
		cutPos = data.find(",");
		std::string hudData = data.substr(0,cutPos);
		data = data.substr(cutPos+1);

		switch(i)
		{
		case 0:
			setMaxLive(atoi(hudData.c_str()));
			break;
		case 1:
			setLive(atof(hudData.c_str()));
			break;
		case 2:
			setMaxEnergy(atoi(hudData.c_str()));
			break;
		case 3:
			setEnergy(atoi(hudData.c_str()));
			break;
		case 4:
			removeEdelsteine(getEdelsteine());
			addEdelsteine(atoi(hudData.c_str()));
			break;
		}
	}

	return true;
}

//setter
void HUD::setLive(float live)
{
	if(File_Manager::getMe().getGameDifficulty() == IMMORTAL)
	{
		this->live = maxLive;
		return;
	}
	else if(File_Manager::getMe().getGameDifficulty() == HARDCORE)
	{
		if(live > 3) live = 3.0;
	}

	if(live >= maxLive)
		this->live = maxLive;
	else if(live < 0)
		this->live = 0;
	else
		this->live = live;

	if(this->live < 0.25)
	{
		Player_Manager::getMe().doDieAni(Screen_Manager::getMe().getGameWindow());
	}
}

void HUD::setMaxLive(int live)
{
	if(live > 0 && live <= 20)
		this->maxLive = live;

	if(maxLive <= 10)
	{
		energyFrame.setPosition(20,TILESIZE*hudScale+25);
		energyShape.setPosition(20,TILESIZE*hudScale+25);
	}
	else
	{
		energyFrame.setPosition(20,TILESIZE*hudScale*2+25);
		energyShape.setPosition(20,TILESIZE*hudScale*2+25);
	}
}

void HUD::setEnergy(float energy)
{
	if(energy >= maxEnergy && this->energy == maxEnergy) return;
	if(energy <= 0 && this->energy == 0) return;

	if(energy >= maxEnergy)
		this->energy = maxEnergy;
	else if(energy <= 0)
		this->energy = 0;
	else
		this->energy = energy;

	float percent = float(energy) / float(maxEnergy) * 100.f;
	float sizeX = float((TILESIZE+1)*hudScale*10) / 100.f * percent;

	if(sizeX == energyShape.getSize().x)
	{
		return;
	}

	energyShape.setSize(sf::Vector2f(sizeX,7));
}

void HUD::setMaxEnergy(int energy)
{
	if(energy > 0)
		this->maxEnergy = energy;

	if(this->energy > maxEnergy) 
		this->energy = maxEnergy;
}

void HUD::addEdelsteine(int amount)
{
	edelsteine += amount;


	std::string edelsteinString;

	if(edelsteine < 10) edelsteinString = "000";
	else if(edelsteine < 100) edelsteinString = "00";
	else if(edelsteine < 1000) edelsteinString = "0";

	edelsteinString += std::to_string(edelsteine);

	edelsteineText.setString(edelsteinString);
}

void HUD::removeEdelsteine(int amount)
{
	edelsteine -= amount;

	if(edelsteine < 0) edelsteine = 0;

	std::string edelsteinString;

	if(edelsteine < 10) edelsteinString = "000";
	else if(edelsteine < 100) edelsteinString = "00";
	else if(edelsteine < 1000) edelsteinString = "0";

	edelsteinString += std::to_string(edelsteine);

	edelsteineText.setString(edelsteinString);
}

//getter
float HUD::getLive()
{
	return live;
}

int HUD::getMaxLive()
{
	return maxLive;
}

float HUD::getEnergy()
{
	return energy;
}

int HUD::getMaxEnergy()
{
	return maxEnergy;
}

int HUD::getEdelsteine()
{
	return edelsteine;
}

sf::Sprite &HUD::getSelectedWeaponSprite()
{
	return selectedWeaponSprite;
}

HUD::~HUD(void)
{
}
