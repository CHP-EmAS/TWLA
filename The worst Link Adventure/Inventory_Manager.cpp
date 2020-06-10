#include "Inventory_Manager.h"

#include "Screen_Manager.h"
#include "Map_Manager.h"
#include "Graphic_Manager.h"
#include "Item_List.h"
#include "Player_Manager.h"
#include "File_Manager.h"
#include "Sound_Manager.h"

Inventory_Manager::Inventory_Manager(void)
{
	selectedSword = slotID::None;
	selectedArmor = slotID::Standart_Armor;
	selectedShield = slotID::None;
	selectedRing = slotID::None;
	selectedShoes = slotID::Standart_Shoe;
	selectedWeapon = slotID::None;

	//Slots
	//Swords
	setSlot(slots[slotID::Woodsword],6,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 169, INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 323));
	setSlot(slots[slotID::Ironsword],7,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 129, INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 323));
	setSlot(slots[slotID::Goldsword],8,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 89, INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 323));
	setSlot(slots[slotID::Herosword],9,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 49, INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 323));
	//Armor
	setSlot(slots[slotID::Standart_Armor],40,1,true,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 153,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 258));
	setSlot(slots[slotID::Blue_Armor],41,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 108,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 258));
	setSlot(slots[slotID::Red_Armor],42,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 63,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 258));
	//Shield
	setSlot(slots[slotID::Iron_Shield],10,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 153,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 193));
	setSlot(slots[slotID::Gold_Shield],11,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 108,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 193));
	setSlot(slots[slotID::Hero_Shield],12,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 63,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 193));
	//Rings
	setSlot(slots[slotID::Blue_Ring],13,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 153,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 128));
	setSlot(slots[slotID::Red_Ring],14,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 108,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 128));
	setSlot(slots[slotID::Hero_Ring],15,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 63,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 128));
	//Shoes
	setSlot(slots[slotID::Standart_Shoe],16,1,true,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 153,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 63));
	setSlot(slots[slotID::Hiking_Shoe],17,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 108,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 63));
	setSlot(slots[slotID::Swimm_Shoe],18,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + INVENTORY_SCREEN_SIZE_X - 63,INVENTORY_SCREEN_POSITION_Y + INVENTORY_SCREEN_SIZE_Y - 63));
	//Items
	setSlot(slots[slotID::Bow],19,0,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 27,INVENTORY_SCREEN_POSITION_Y + 400),0,40);
	setSlot(slots[slotID::Boomerang],20,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 72,INVENTORY_SCREEN_POSITION_Y + 400));
	setSlot(slots[slotID::Bomb],21,0,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 117,INVENTORY_SCREEN_POSITION_Y + 400),0,20);
	setSlot(slots[slotID::Waterbomb],22,0,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 162,INVENTORY_SCREEN_POSITION_Y + 400),0,20);
	setSlot(slots[slotID::Hammer],23,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 207,INVENTORY_SCREEN_POSITION_Y + 400));
	setSlot(slots[slotID::Grapplinghock],24,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 27,INVENTORY_SCREEN_POSITION_Y + 445));
	setSlot(slots[slotID::Flute],25,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 72,INVENTORY_SCREEN_POSITION_Y + 445));
	setSlot(slots[slotID::Bombbow],26,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 117,INVENTORY_SCREEN_POSITION_Y + 445));
	setSlot(slots[slotID::Magic_Wand],27,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 162,INVENTORY_SCREEN_POSITION_Y + 445));
	setSlot(slots[slotID::Amulet],28,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 207,INVENTORY_SCREEN_POSITION_Y + 445));
	//Potions
	setSlot(slots[slotID::Potion_1],31,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 272,INVENTORY_SCREEN_POSITION_Y + 400));
	setSlot(slots[slotID::Potion_2],31,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 317,INVENTORY_SCREEN_POSITION_Y + 400));
	setSlot(slots[slotID::Potion_3],31,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 272,INVENTORY_SCREEN_POSITION_Y + 445));
	setSlot(slots[slotID::Potion_4],31,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 317,INVENTORY_SCREEN_POSITION_Y + 445));
	//Update Item
	setSlot(slots[slotID::Moneybag],32,1,true,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 121,INVENTORY_SCREEN_POSITION_Y + 302));
	setSlot(slots[slotID::Arrowbag],39,20,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 181,INVENTORY_SCREEN_POSITION_Y + 302),40,40);
	setSlot(slots[slotID::Arrow],34,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 241,INVENTORY_SCREEN_POSITION_Y + 302));
	setSlot(slots[slotID::Candle],37,1,false,sf::Vector2i(INVENTORY_SCREEN_POSITION_X + 301,INVENTORY_SCREEN_POSITION_Y + 302));

	setArrowUpgradeType(0);
	activForceFragments = 0;
	usedForceFragments = 0;

	NEWOUT("Neuer Inventory_Manager wurde erstellt! (" << this << ")")
}

void Inventory_Manager::setSlot(slot &slot, int itemNumber, int amount, bool unlock, sf::Vector2i pos,int minimumAmount,int maximumAmount)
{
	slot.itemNumber = itemNumber;

	slot.itemSprite = Item_List::getMe().getItemSprite(itemNumber);
	slot.itemSprite.setScale(SCALE,SCALE);
	slot.itemSprite.setPosition(sf::Vector2f(pos));

	slot.unlock = unlock;
	slot.amount = amount;

	slot.maximumAmount = maximumAmount;
	slot.minimumAmount = minimumAmount;
}

Inventory_Manager::slot &Inventory_Manager::getSlot(slotID slotID)
{
	if(slotID >= 0 && slotID < 34)
	{
		return slots[slotID];
	}

	FOUT("Slot <" << slotID << "> ist kein Item-Slot!");

	return slots[0];
}

Inventory_Manager::slotID Inventory_Manager::getSelectedShield()
{
	return selectedShield;
}

Inventory_Manager::slotID Inventory_Manager::getSelectedSword()
{
	return selectedSword;
}

Inventory_Manager::slotID Inventory_Manager::getSelectedWeapon()
{
	return selectedWeapon;
}

Inventory_Manager::slotID Inventory_Manager::getSelectedArmor()
{
	return selectedArmor;
}

Inventory_Manager::slotID Inventory_Manager::getSelectedRing()
{
	return selectedRing;
}

Inventory_Manager::slotID Inventory_Manager::getSelectedShoes()
{
	return selectedShoes;
}

int Inventory_Manager::getSlotItem(slotID slotID)
{
	if(slotID >= 0 && slotID < 34)
	{
		return slots[slotID].itemNumber;
	}
	
	FOUT("Slot <" << slotID << "> ist kein Item-Slot!");

	return slots[0].itemNumber;
}

int Inventory_Manager::getSlotAmount(slotID slotID)
{
	if(slotID >= 0 && slotID < 34)
	{
		return slots[slotID].amount;
	}
	
	FOUT("Slot <" << slotID << "> ist kein Item-Slot!");

	return slots[0].amount;
}

unsigned short Inventory_Manager::getArrowType()
{
	return arrowUpgradeType;
}

bool Inventory_Manager::isSlotUnlocked(slotID slotID)
{
	return slots[slotID].unlock;
}

void Inventory_Manager::unlockSlot(slotID slotID)
{
	slots[slotID].unlock = true;
}

void Inventory_Manager::useSlot(slotID slotID)
{
	if(slotID >= 0 && slotID < 34)
		if(!slots[slotID].unlock) return;

	if(slotID >= slotID::Woodsword && slotID <= slotID::Herosword)
	{
		if(selectedSword == slotID)
		{
			selectedSword = slotID::None;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Deselect);
		}
		else
		{
			selectedSword = slotID;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}

		Player_Manager::getMe().updatePlayerStats();
	}
	else if(slotID >= slotID::Standart_Armor && slotID <= slotID::Red_Armor)
	{
		if(selectedArmor == slotID)
		{
			selectedArmor = slotID::Standart_Armor;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Deselect);
		}
		else
		{
			selectedArmor = slotID;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}

		Player_Manager::getMe().updatePlayerStats();

		switch(selectedArmor)
		{
		case slotID::Standart_Armor:
			Player_Manager::getMe().setPlayerColor(GREEN_ARMOR);
			break;
		case slotID::Red_Armor:
			Player_Manager::getMe().setPlayerColor(RED_ARMOR);
			break;
		case slotID::Blue_Armor:
			Player_Manager::getMe().setPlayerColor(BLUE_ARMOR);
			break;
		}
	}
	else if(slotID >= slotID::Iron_Shield && slotID <= slotID::Hero_Shield)
	{
		if(selectedShield == slotID)
		{
			selectedShield = slotID::None;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Deselect);
		}
		else
		{
			selectedShield = slotID;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}

		Player_Manager::getMe().updatePlayerStats();
	}
	else if(slotID >= slotID::Blue_Ring && slotID <= slotID::Hero_Ring)
	{
		if(selectedRing == slotID)
		{
			selectedRing = slotID::None;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Deselect);
		}
		else
		{
			selectedRing = slotID;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}

		Player_Manager::getMe().updatePlayerStats();
	}
	else if(slotID >= slotID::Standart_Shoe && slotID <= slotID::Swimm_Shoe)
	{
		if(selectedShoes == slotID)
		{
			selectedShoes = slotID::Standart_Shoe;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Deselect);
		}
		else
		{
			selectedShoes = slotID;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}

		Player_Manager::getMe().updatePlayerStats();
	}
	else if(slotID >= slotID::Bow && slotID <= slotID::Amulet)
	{
		if(selectedWeapon == slotID)
		{
			selectedWeapon = slotID::None;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Deselect);
		}
		else
		{
			selectedWeapon = slotID;
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}

		Player_Manager::getMe().updatePlayerStats();
	}
	else if(slotID >= slotID::Potion_1 && slotID <= slotID::Potion_4)
	{
		if(slots[slotID].itemNumber == 30)
		{
			setSlotItem(slotID,31);
			Player_Manager::getMe().heal(4);
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		}
		else if(slots[slotID].itemNumber == 29)
		{
			setSlotItem(slotID,31);
			Player_Manager::getMe().heal(Player_Manager::getMe().getGameHUD().getMaxLive());
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		}
		else if(slots[slotID].itemNumber == 53)
		{
			setSlotItem(slotID,31);
			Player_Manager::getMe().getGameHUD().setEnergy((Player_Manager::getMe().getGameHUD().getMaxEnergy()));
		}
	}
	else if(slotID == slotID::Quests)
	{
		Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Quests,false);
	}
	else if(slotID == slotID::Archievements)
	{
		Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game_Stats,false);;
	}
	else if(slotID == slotID::Settings )
	{
		Screen_Manager::getMe().fadeOut();
		Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Options,false);
		Screen_Manager::getMe().fadeIn();
	}
	else if(slotID == slotID::X)
	{
		File_Manager::getMe().saveCurrentMapChanges();
		File_Manager::getMe().save_Savegame();
		Screen_Manager::getMe().fadeOut();
		Screen_Manager::getMe().getScreen(Screen::Game)->closeScreen();
		Screen_Manager::getMe().changeScreen(Screen::Main_Menue);
		Screen_Manager::getMe().fadeIn();
	}
	else if(slotID == slotID::Save)
	{
		File_Manager::getMe().saveCurrentMapChanges();
		File_Manager::getMe().save_Savegame();
	}

}

void Inventory_Manager::setSlotAmount(slotID slotID,int amount)
{
	if(slots[slotID].unlock == false)
	{
		WOUT("Die Slot-Anzahl von Slot <" << slotID << "> kann nicht erhöt werden, da er noch nicht aufgedeckt ist!")
		return;
	}

	int lastAmount = slots[slotID].amount;

	if(amount <= slots[slotID].minimumAmount)
		slots[slotID].amount = slots[slotID].minimumAmount;
	else if(amount >= slots[slotID].maximumAmount)
		slots[slotID].amount = slots[slotID].maximumAmount;
	else
		slots[slotID].amount = amount;

	if(slotID == slotID::Bomb)
	{
		if(lastAmount > 0 && amount <= 0)
			setSlotItem(slotID::Bomb,43);
		else if(lastAmount <= 0 && amount > 0)
			setSlotItem(slotID::Bomb,21);
	}
	else if(slotID == slotID::Waterbomb)
	{
		if(lastAmount > 0 && amount <= 0)
			setSlotItem(slotID::Waterbomb,44);
		else if(lastAmount <= 0 && amount > 0)
			setSlotItem(slotID::Waterbomb,22);
	}
}

void Inventory_Manager::setSlotItem(slotID slotID,int itemNumber)
{
	sf::Vector2f lastPos = slots[slotID].itemSprite.getPosition();

	slots[slotID].itemNumber = itemNumber;

	slots[slotID].itemSprite = Item_List::getMe().getItemSprite(itemNumber);
	slots[slotID].itemSprite.setScale(SCALE,SCALE);
	slots[slotID].itemSprite.setPosition(sf::Vector2f(lastPos));
}

void Inventory_Manager::setArrowUpgradeType(unsigned short upgradeStatus)
{
	if(upgradeStatus >= 0 && upgradeStatus <= 2)
	{
		arrowUpgradeType = upgradeStatus;
	}
	else
	{
		FOUT("Arrow-type out of range - " << upgradeStatus);
	}

	switch(arrowUpgradeType)
	{
	case 0:
		setSlotItem(slotID::Arrow,34);
		break;
	case 1:
		setSlotItem(slotID::Arrow,35);
		break;
	case 2:
		setSlotItem(slotID::Arrow,36);
		break;
	}
}

std::string Inventory_Manager::getInventorySaveData()
{
	std::string fullstring;

	for(int slot = 0; slot < 34; slot++)
	{
		switch(slot)
		{
		case Bow:
		case Bomb:
		case Waterbomb:
			if(slots[slot].unlock)
			{
				fullstring += std::to_string(slots[slot].amount);
			}
			else
			{
				fullstring += "-1";
			}

			fullstring += ",";
			break;
		default:
			fullstring += std::to_string(slots[slot].unlock);
			fullstring += ",";
			break;
		}
	}

	fullstring += std::to_string(selectedSword) + ",";
	fullstring += std::to_string(selectedWeapon) + ",";
	fullstring += std::to_string(selectedArmor) + ",";
	fullstring += std::to_string(selectedShield) + ",";
	fullstring += std::to_string(selectedRing) + ",";
	fullstring += std::to_string(selectedShoes) + ",";

	fullstring += std::to_string(arrowUpgradeType) + ",";

	fullstring += std::to_string(activForceFragments) + ",";
	fullstring += std::to_string(usedForceFragments) + ",";

	return fullstring;
}

bool Inventory_Manager::loadInventorySaveData(std::string data)
{
	int cutPos;

	for(int slot = 0; slot < 43; slot++)
	{
		cutPos = data.find(",");
		std::string slotData = data.substr(0,cutPos);
		data = data.substr(cutPos+1);

		switch(slot)
		{
		case Bow:
		case Bomb:
		case Waterbomb:

			if(slotData == "-1")
			{
				slots[slot].unlock = false;
				slots[slot].amount = 0;
			}
			else
			{
				slots[slot].unlock = true;
				slots[slot].amount = atoi(slotData.c_str());
			}

			break;
		case 34:
		case 35:
		case 36:
		case 37:
		case 38:
		case 39:
			useSlot(slotID(atoi(slotData.c_str())));
			break;
		case 40:
			setArrowUpgradeType(atoi(slotData.c_str()));
			break;
		case 41:
			activForceFragments = atoi(slotData.c_str());
			break;
		case 42:
			usedForceFragments = atoi(slotData.c_str());
			break;
		default:
			slots[slot].unlock = bool(atoi(slotData.c_str()));
			break;
		}
	}

	return false;
}

void Inventory_Manager::addTriforceFragment()
{
	if(activForceFragments < 3) activForceFragments++;
}

void Inventory_Manager::useTriforceFragment()
{
	if(usedForceFragments < activForceFragments) usedForceFragments++;
}

int Inventory_Manager::getTriforceFragments()
{
	return activForceFragments;
}

int Inventory_Manager::getUsedTriforceFragments()
{
	return usedForceFragments;
}

Inventory_Manager::~Inventory_Manager(void)
{
	DELOUT("Inventory_Manager wurde geloescht!\t(" << this << ")")
}
