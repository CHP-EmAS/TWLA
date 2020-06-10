#include "Objects.h"

#include "Command_Manager.h"
#include "Message_Manager.h"
#include "Sound_Manager.h"
#include "Player_Manager.h"

CommandDoor::CommandDoor(int x, int y,short ID,std::list<std::string> requiredCommands,std::list<std::string> checkedCommands):InteractivObject(x,y,ID)
{
	objType = ObjectType::Command_Door;

	setCollisionMode(1);
	setInteract(true);

	cBox.height = ZOOMSIZE - 10;

	setObjectSprite(441,Graphic_Manager::Overworld);

	for(auto cmdCursor : requiredCommands)
	{
		std::map<std::string,bool>::iterator it;
		it = this->requiredCommands.find(cmdCursor);

		if(it != this->requiredCommands.end())
		{
			FOUT("Befehl: '" << cmdCursor << "' ist in Objekt(" << this->ID << ") doppelt definiert!");
		}

		this->requiredCommands[cmdCursor] = false;
	}

	for(auto checkCursor : checkedCommands)
	{
		std::map<std::string,bool>::iterator it;
		it = this->requiredCommands.find(checkCursor);

		if(it != this->requiredCommands.end())
		{
			this->requiredCommands[checkCursor] = true;

			bool allCeck = true;

			for(auto cursor:this->requiredCommands)
			{
				if(cursor.second == false) allCeck = false;
			}

			if(allCeck) setActiv(false);
		}
	}
}

void CommandDoor::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		if(interact_Type == Key_Interact)
		{
			Message_Manager::getMe().showMsg("Es sieht so aus als wäre dieser Stein mit einerart Machanismus verbunden!",sf::Color(0,255,255));
		}
		else if(interact_Type == Collision_Interact)
		{
			Message_Manager::getMe().showShortHintMsg("Q - Untersuchen");
		}
	}
}

void CommandDoor::executeObjectCommand(std::string command)
{
	int pos = command.find(":");
	
	if(pos != std::string::npos)
	{
		std::string subCommand = command.substr(0,pos);
		command = command.substr(pos+1);

		if(subCommand == "add")
		{
			std::map<std::string,bool>::iterator it;
			it = this->requiredCommands.find(command);

			if(it != this->requiredCommands.end())
			{
				this->requiredCommands[command] = true;

				bool allCeck = true;

				for(auto cursor:this->requiredCommands)
				{
					if(cursor.second == false) allCeck = false;
				}

				if(allCeck) open();
			}
		}
		else if(subCommand == "remove")
		{
			std::map<std::string,bool>::iterator it;
			it = this->requiredCommands.find(command);

			if(it != this->requiredCommands.end())
			{
				this->requiredCommands[command] = false;

				if(!isActive())
				{
					close();
				}
			}
		}
		else if(subCommand == "cmd")
		{
			if(command == "open") open();
			else if(command == "close") close();
		}
	}
	else
	{
		WOUT("Befehl: '" << command << "' ist für Objekt(" << this->ID << ") nicht verwendbar!");
	}
}

std::string CommandDoor::getMySaveData()
{
	std::string content = ""; 

	for(auto cursor:this->requiredCommands)
	{
		content += std::to_string(cursor.second) + ",";
	}

	content += InteractivObject::getMySaveData();

	return content;
}

bool CommandDoor::loadMySaveData(std::string data)
{
	for(auto cursor:this->requiredCommands)
	{
		int pos = data.find(",");
		this->requiredCommands[cursor.first] = bool(atoi(data.substr(0,pos).c_str()));
		data = data.substr(pos+1);
	}

	InteractivObject::loadMySaveData(data);

	return false;
}

void CommandDoor::open()
{
	setActiv(false);
	Sound_Manager::getMe().playEffectSound(Sound_Manager::Found_Secret);
	Player_Manager::getMe().updatePlayerHitBoxes();
}
	
void CommandDoor::close()
{
	setActiv(true);
	Player_Manager::getMe().updatePlayerHitBoxes();
}

CommandDoor::~CommandDoor(void)
{

}