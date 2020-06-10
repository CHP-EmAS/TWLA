#pragma once

#include "SFML/Graphics.hpp"

#include "Defines.h"

class Inventory_Manager : public Singleton<Inventory_Manager>
{
friend class Singleton<Inventory_Manager>;
public:

	enum slotID
	{
		Woodsword,
		Ironsword,
		Goldsword,
		Herosword,
		Standart_Armor,
		Blue_Armor,
		Red_Armor,
		Iron_Shield,
		Gold_Shield,
		Hero_Shield,
		Blue_Ring,
		Red_Ring,
		Hero_Ring,
		Standart_Shoe,
		Hiking_Shoe,
		Swimm_Shoe,
		Bow,
		Boomerang,
		Bomb,
		Waterbomb,
		Hammer,
		Grapplinghock,
		Flute,
		Bombbow,
		Magic_Wand, 
		Amulet, 
		Potion_1, 
		Potion_2, 
		Potion_3, 
		Potion_4, 
		Moneybag, 
		Arrowbag, 
		Arrow,
		Candle,
		
		Quests,
		Archievements,
		Settings,
		Save,
		X,
		
		None = -1
	};

	struct slot
	{
		int itemNumber;
		int amount;
		
		sf::Sprite itemSprite;
		
		bool unlock;

		int minimumAmount;
		int maximumAmount;
	};

	std::string getInventorySaveData();
	bool loadInventorySaveData(std::string data);

	void unlockSlot(slotID slotID);

	void setSlotAmount(slotID slotID, int amount);
	void setSlotItem(slotID slotID,int itemNumber);

	slot &getSlot(slotID slotID);

	slotID getSelectedShield();
	slotID getSelectedSword();
	slotID getSelectedWeapon();
	slotID getSelectedArmor();
	slotID getSelectedRing();
	slotID getSelectedShoes();

	int getSlotItem(slotID slotID);
	int getSlotAmount(slotID slotID);
	bool isSlotUnlocked(slotID slotID);
	
	void setArrowUpgradeType(unsigned short upgradeStatus);
	unsigned short getArrowType();

	void addTriforceFragment();
	void useTriforceFragment();
	int getTriforceFragments();
	int getUsedTriforceFragments();

	void useSlot(slotID slotID);

	~Inventory_Manager(void);
private:
	Inventory_Manager(void);
	
	void setSlot(slot &slot, int itemNumber, int amount, bool unlock, sf::Vector2i pos,int minimumAmount = 1,int maximumAmount = 1);

	//Slots
	slot slots[34];

	//Selection
	slotID selectedSword;
	slotID selectedShield;
	slotID selectedShoes;
	slotID selectedRing;
	slotID selectedArmor;
	slotID selectedWeapon;

	unsigned short arrowUpgradeType;

	//Triforce
	int activForceFragments;
	int usedForceFragments;
};

