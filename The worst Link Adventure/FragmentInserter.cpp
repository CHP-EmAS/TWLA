#include "Objects.h"

#include "Message_Manager.h"
#include "Map_Manager.h"
#include "Player_Manager.h"
#include "Dialog_Manager.h"

FragmentInserter::FragmentInserter(int x, int y,short ID) : InteractivObject(x,y,ID)
{
	objType = ObjectType::Fragment_Inserter;

	setObjectSprite(0,Graphic_Manager::Overworld);
	spr_object.setColor(sf::Color::Transparent);

	collisionMode = 0;
	interactable = true;
	inserted = false;
}

void FragmentInserter::interact(interactType interact_Type)
{
	if(interactable && interact_Type == Key_Interact && activ)
	{
		if(Inventory_Manager::getMe().getTriforceFragments() > Inventory_Manager::getMe().getUsedTriforceFragments())
		{
			Inventory_Manager::getMe().useTriforceFragment();

			Map_Manager::getMe().getCurrentMap().getTile(posX,posY)->tileNumber = 450;
			Map_Manager::getMe().getCurrentMap().getTile(posX+1,posY)->tileNumber = 451;
			Map_Manager::getMe().getCurrentMap().getTile(posX-1,posY)->tileNumber = 449;
			Map_Manager::getMe().getCurrentMap().getTile(posX,posY+1)->tileNumber = 453;
			Map_Manager::getMe().getCurrentMap().getTile(posX,posY-1)->tileNumber = 452;

			Map_Manager::getMe().updateViewMap();

			inserted = true;
			activ = false;
			interactable = false;

			if(Inventory_Manager::getMe().getUsedTriforceFragments() == 3)
			{
				Dialog_Manager::getMe().doDialog(2);
			}
		}
		else
		{
			Message_Manager::getMe().showMsg("Ich hab keinen weiteres Fragment zur Verfügung.",sf::Color(0,255,255));
		}
	}
	else if(interactable && interact_Type == Collision_Interact)
	{
		Message_Manager::getMe().showShortHintMsg("Q - Fragment einsetzen");
	}
}

std::string FragmentInserter::getMySaveData()
{
	std::string content = std::to_string(inserted) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool FragmentInserter::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_broken = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->inserted = bool(atoi(str_broken.c_str()));

	if(inserted)
	{
		activ = false;
		interactable = false;

		Map_Manager::getMe().getCurrentMap().getTile(posX,posY)->tileNumber = 450;
		Map_Manager::getMe().getCurrentMap().getTile(posX+1,posY)->tileNumber = 451;
		Map_Manager::getMe().getCurrentMap().getTile(posX-1,posY)->tileNumber = 449;
		Map_Manager::getMe().getCurrentMap().getTile(posX,posY+1)->tileNumber = 453;
		Map_Manager::getMe().getCurrentMap().getTile(posX,posY-1)->tileNumber = 452;

		Map_Manager::getMe().updateViewMap();
	}

	return false;
}

FragmentInserter::~FragmentInserter(void)
{

}