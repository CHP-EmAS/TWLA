#include "Objects.h"

#include "Message_Manager.h"

Grabable_Tile::Grabable_Tile(int posX, int posY,short ID, Graphic_Manager::tileset tileset, int tile):InteractivObject(posX,posY,ID)
{
	objType = ObjectType::Grabable_Tile;

	this->tileset = tileset;

	setObjectSprite(tile,tileset);

	collisionMode = 1;
	interactable = true;

	cBox.type = Grabable_Collisionbox;
	cBox.height = ZOOMSIZE - 14;

	cBoxPosY = 2;
}

void Grabable_Tile::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Key_Interact)
		{
			Message_Manager::getMe().showMsg("Dieses Objekt sieht merkwürdig aus...",sf::Color(0,200,200));
		}
		else if(interact_Type == Collision_Interact)
		{
			Message_Manager::getMe().showShortHintMsg("Q - Untersuchen");
		}
	}
}

Grabable_Tile::~Grabable_Tile(void)
{
}
