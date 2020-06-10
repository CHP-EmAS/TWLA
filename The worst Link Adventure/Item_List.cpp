#include "Item_List.h"

#include <fstream>

#include "Graphic_Manager.h"

Item_List::Item_List(void)
{
	NEWOUT("Neue Item_List wurde erstellt! (" << this << ")")
}

const sf::Sprite &Item_List::getItemSprite(int itemNumber)
{
	std::list<item>::iterator it;
	
	for(it = itemList.begin(); it != itemList.end(); it++)
	{
		if(it->itemNumber == itemNumber)
		{
			return (it->itemSprite);
		}
	}
	
	FOUT("Item <" << itemNumber << "> konnte nicht gefunden werden! (Out of range!)")
	return getItemSprite(-1);
}

Graphic_Manager::tileset Item_List::getItemTileset(int itemNumber)
{
	std::list<item>::iterator it;
	
	for(it = itemList.begin(); it != itemList.end(); it++)
	{
		if(it->itemNumber == itemNumber)
		{
			return (it->tileset);
		}
	}
	
	FOUT("Item <" << itemNumber << "> konnte nicht gefunden werden! (Out of range!)")
	return Graphic_Manager::Overworld;
}

int Item_List::getItemTile(int itemNumber)
{
	std::list<item>::iterator it;

	for(it = itemList.begin(); it != itemList.end(); it++)
	{
		if(it->itemNumber == itemNumber)
		{
			return (it->tile);
		}
	}
	
	FOUT("Item <" << itemNumber << "> konnte nicht gefunden werden! (Out of range!)")
	return 0;
}

std::string Item_List::getItemName(int itemNumber)
{
	std::list<item>::iterator it;

	for(it = itemList.begin(); it != itemList.end(); it++)
	{
		if(it->itemNumber == itemNumber)
		{
			return (it->itemName);
		}
	}
	
	FOUT("Item <" << itemNumber << "> konnte nicht gefunden werden! (Out of range!)")
	return "Nichts";
}

std::string Item_List::getItemArticle(int itemNumber)
{
	std::list<item>::iterator it;

	for(it = itemList.begin(); it != itemList.end(); it++)
	{
		if(it->itemNumber == itemNumber)
		{
			return (it->article);
		}
	}
	
	FOUT("Item <" << itemNumber << "> konnte nicht gefunden werden! (Out of range!)")
	return "ein";
}

const Item_List::item &Item_List::getItem(int itemNumber)
{
	std::list<item>::iterator it;

	for(it = itemList.begin(); it != itemList.end(); it++)
	{
		if(it->itemNumber == itemNumber)
		{
			return *it;
		}
	}
	
	FOUT("Item <" << itemNumber << "> konnte nicht gefunden werden! (Out of range!)")
	return getItem(0);
}

void Item_List::loadAllItems()
{
	std::ifstream ifs;

	std::string listPath = "lists/Item_list";
	listPath += + ZELDA_DATA_ENDING;

	ifs.open(listPath, std::ios::in);

	if (ifs.good())
	{
		//dummy
		char c_dummy[51];

		while(1)
		{
			ifs.getline(c_dummy,10,',');
			std::string number = std::string(c_dummy);

			if(number!="End")
			{
				item newItem;

				//Number
				newItem.itemNumber = atoi(number.c_str());

				//Tileset
				ifs.getline(c_dummy,10,',');
				std::string tileset = std::string(c_dummy);
				newItem.tileset = Graphic_Manager::tileset(atoi(tileset.c_str()));

				//Tile
				ifs.getline(c_dummy,10,',');
				std::string tile = std::string(c_dummy);
				newItem.tile = atoi(tile.c_str());

				//Artikel
				ifs.getline(c_dummy,10,',');
				newItem.article = std::string(c_dummy);

				//Name
				ifs.getline(c_dummy,50,',');
				newItem.itemName = std::string(c_dummy);
				
				//stackable
				ifs.getline(c_dummy,10,'\n');
				std::string str_stackable = std::string(c_dummy);

				//Sprite
				newItem.itemSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(newItem.tileset));
				newItem.itemSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(newItem.tileset,newItem.tile));

				itemList.push_back(newItem);
			}
			else
			{
				break;
			}
		}

		ifs.close();
	}
}

Item_List::~Item_List(void)
{
	DELOUT("Item_List wurde geloescht!\t(" << this << ")")
}
