#include "Objects.h"

#include "Graphic_Manager.h"
#include "Message_Manager.h"


Sign::Sign(int x, int y,short ID, std::string signMessage,sf::Color textColor,short type):InteractivObject(x,y,ID)
{
	objType = ObjectType::Sign;

	this->signMessage = signMessage;
	this->textColor = textColor;

	collisionMode = 1;
	interactable = true;

	tileset = Graphic_Manager::Overworld;

	switch(type)
	{
	case 0:
		setObjectSprite(72,tileset);
		break;
	case 1:
		setObjectSprite(103,tileset);
		break;
	}

	cBox.height = ZOOMSIZE - 10;
}

void Sign::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Key_Interact)
			Message_Manager::getMe().showMsg(signMessage,textColor);
		else if(interact_Type == Collision_Interact)
			Message_Manager::getMe().showShortHintMsg("Q - Lesen");
	}
}


Sign::~Sign(void)
{

}
