#include "Objects.h"

#include "Command_Manager.h"
#include "Message_Manager.h"

Trigger::Trigger(int x, int y,short ID, std::list<std::string> commands, bool multiTrigger, interactType trigger_Type, std::string hintMsg):InteractivObject(x,y,ID)
{
	objType = ObjectType::Trigger;

	if(trigger_Type == interactType::Hit_Interact)
	{
		setCollisionMode(true);
	}
	else
	{
		setCollisionMode(false);
	}
	
	setInteract(true);
	setActiv(true);

	this->multiTrigger = multiTrigger;
	this->commands = commands;
	this->triggerType = trigger_Type;
	this->hintMsg = hintMsg;

	cBox.type = collisionboxType::Interact_Only;

	setObjectSprite(0,Graphic_Manager::Overworld);
	spr_object.setColor(sf::Color::Transparent);
}

void Trigger::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == triggerType)
		{
			if(!multiTrigger) setInteract(false);

			std::list<std::string> tempCommands = this->commands;

			for(auto cursor : tempCommands)
			{
				Command_Manager::getMe().doCommand(cursor);
			}
		}
		else if(interact_Type == Collision_Interact && hintMsg != "")
		{
			Message_Manager::getMe().showShortHintMsg(hintMsg);
		}
	}
}

Trigger::~Trigger(void)
{

}
