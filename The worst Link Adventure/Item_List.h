#pragma once

#include "SFML/Graphics.hpp"
#include <list>

#include "Defines.h"

#include "Graphic_Manager.h"

class Item_List : public Singleton<Item_List>
{

friend class Singleton<Item_List>;

public:

	struct item
	{
		sf::Sprite itemSprite;

		int itemNumber;

		Graphic_Manager::tileset tileset;
		int tile;

		std::string itemName;
		std::string article;
	};
		
	const sf::Sprite &getItemSprite(int itemNumber);
	Graphic_Manager::tileset getItemTileset(int itemNumber);
	int getItemTile(int itemNumber);
	std::string getItemName(int itemNumber);
	std::string getItemArticle(int itemNumber);
	const Item_List::item &getItem(int itemNumber);

	void loadAllItems();

	~Item_List(void);

private:
	Item_List(void);

	std::list<item> itemList;
};

