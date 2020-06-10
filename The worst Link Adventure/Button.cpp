#include "Objects.h"

#include "Graphic_Manager.h"
#include "Command_Manager.h"
#include "Sound_Manager.h"
#include "Message_Manager.h"

Button::Button(int x, int y,short ID, std::string pressCommand, std::string releaseCommand, bool pressed, int type):InteractivObject(x,y,ID)
{
	objType = ObjectType::Button;

	interactable = true;

	tileset = Graphic_Manager::Overworld;

	this->pressed = pressed;

	if(type == 0)
	{
		if(pressed) setObjectSprite(440,tileset);
		else setObjectSprite(439,tileset);

		collisionMode = 0;
	}
	else if(type == 1)
	{
		if(pressed) setObjectSprite(432,tileset);
		else setObjectSprite(433,tileset);

		cBox.height = ZOOMSIZE - 10;
		cBox.width = ZOOMSIZE - 14;
		cBoxPosX = 7;
		collisionMode = 1;
	}

	this->releaseCommand = releaseCommand;
	this->pressCommand = pressCommand;
	this->type = type;
}

void Button::interact(interactType interact_Type)
{
	if(interactable && activ)
	{
		bool interact = false;

		if(interact_Type == Key_Interact)
		{
			if(type == 0) interact = true;
		}
		else if(interact_Type == Arrow_Interact || interact_Type == Hit_Interact)
		{
			if(type == 1) interact = true;
		}
		else if(interact_Type == Collision_Interact)
		{
			if(type == 0)
			{
				if(pressed) Message_Manager::getMe().showShortHintMsg("Q - Ausschalten");
				else Message_Manager::getMe().showShortHintMsg("Q - Anschalten");
			}
		}

		if(interact)
		{
			if(idleClock.getElapsedTime().asMilliseconds() >= 200)
			{
				idleClock.restart();

				if(pressed)
				{
					Command_Manager::getMe().doCommand(releaseCommand);

					if(type == 0) setObjectSprite(439,tileset);
					else if(type == 1) setObjectSprite(433,tileset);
				}
				else
				{
					Command_Manager::getMe().doCommand(pressCommand);

					if(type == 0) setObjectSprite(440,tileset);
					else if(type == 1) setObjectSprite(432,tileset);
				}

				pressed = !pressed;
			}
		}
	}
}

std::string Button::getMySaveData()
{
	std::string content = std::to_string(pressed) + ",";

	content += InteractivObject::getMySaveData();

	return content;
}

bool Button::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_pressed = data.substr(0,pos);
	data = data.substr(pos+1);

	InteractivObject::loadMySaveData(data);

	this->pressed = bool(atoi(str_pressed.c_str()));

	if(type == 0)
	{
		if(pressed) setObjectSprite(440,tileset);
		else setObjectSprite(439,tileset);
	}
	else if(type == 1)
	{
		if(pressed) setObjectSprite(432,tileset);
		else setObjectSprite(433,tileset);
	}

	return false;
}

Button::~Button(void)
{

}