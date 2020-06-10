#include "Command_Manager.h"

#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Message_Manager.h"
#include "Animation_Manager.h"
#include "Weapon_Manager.h"
#include "Screen_Manager.h"
#include "Player_Manager.h"
#include "File_Manager.h"
#include "Dialog_Manager.h"
#include "Sound_Manager.h"
#include "NPC_Manager.h"
#include "Screens.h"

//Konstruktor
Command_Manager::Command_Manager(void)
{
	NEWOUT("Neuer Command_Manager wurde erstellt! (" << this << ")")
}

//Methoden
void Command_Manager::doCommand(std::string c)
{
	//Main- & Subcommand
	std::string mainCommand;
	std::string *subCommands = new std::string[std::count(c.begin(), c.end(), ' ')];
	int commandNumber = 0;
	
	//Split main/subcommands in an array
	size_t pos = 0;
	int i = 0;

	c += " ";

	while(true)
	{
		if(c.size() > 0) while(c[0] == ' ') c.erase(0, 1);

		if((pos = c.find(' ')) == std::string::npos) break;

		if(i == 0)
		{
			mainCommand = c.substr(0, pos); //setMaincommand
		}
		else
		{
			subCommands[i-1] = c.substr(0, pos); //set Subcommands
		}

		i++;
		commandNumber++;

		c.erase(0, pos + 1);

		if(c[0] == '<')
		{
			c.erase(0, 1);

			if((pos = c.find('>')) != std::string::npos)
			{
				subCommands[i-1] = c.substr(0, pos);

				i++;
				commandNumber++;

				c.erase(0, pos + 1);
			}
		}
	}

	//Command List
	if(mainCommand == "tp") //Syntax: tp <x : int> <y : int>
	{
		Player_Manager::getMe().tp(atoi(subCommands[0].c_str()),atoi(subCommands[1].c_str()));
	}
	else if(mainCommand == "give")
	{
		if(commandNumber >= 3)
		{
			Player_Manager::getMe().give(atoi(subCommands[0].c_str()),atoi(subCommands[1].c_str()));
		}
		else if(commandNumber >= 2)
		{
			if(subCommands[0] == "all")
			{
				for(int i = 0;i<38;i++)
				{
					Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(i));
				}

				Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Arrowbag,20);
				Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Bomb,20);
				Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Waterbomb,20);
			}
		}
		else
		{
			FOUT("Zu wenig Argumente! give <itemnumber> <amount>")
		}
	}
	else if(mainCommand == "heal")
	{
		if(commandNumber >= 2)
		{
			Player_Manager::getMe().heal(atoi(subCommands[0].c_str()));
		}
	}
	else if(mainCommand == "changePlayerColor") //Syntax: changePlayerColor <r : int> <g : int> <b : int> 
	{
		if(commandNumber == 2 || commandNumber >= 5)
		{
			if(subCommands[0] == "original" || subCommands[0] == "reset")
				Player_Manager::getMe().setPlayerColor(sf::Color(0,192,0));
			else
				Player_Manager::getMe().setPlayerColor(sf::Color(atoi(subCommands[0].c_str()),atoi(subCommands[1].c_str()),atoi(subCommands[2].c_str()),atoi(subCommands[3].c_str())));
		}
		else
		{
			FOUT("Zu wenig Argumente! changePlayerColor <r> <g> <b> <a> ODER changePlayerColor original")
		}
	}
	else if(mainCommand == "explode") //Syntax: explode 
	{
		Animation_Manager::getMe().doSpriteExplosion(Player_Manager::getMe().getPlayerSprite(),8);
	}
	else if(mainCommand == "kill") //Syntax: playerExplode 
	{
		Player_Manager::getMe().hurt(Player_Manager::getMe().getGameHUD().getMaxLive());
	}
	else if(mainCommand == "zoom")
	{
		if(commandNumber >= 2)
		{
			if(subCommands[0] == "original")
				Player_Manager::getMe().getPlayerSprite().setScale(SCALE,SCALE);
			else
				Animation_Manager::getMe().doZoomAnimation(Player_Manager::getMe().getPlayerSprite(),atoi(subCommands[0].c_str()));
		}
		else
		{
			FOUT("Zu wenig Argumente! zoom <zoom in percent>")
		}
	}
	else if(mainCommand == "debugMsg") //Syntax: debug_Msg <msg : string>
	{
		if(commandNumber >= 2)
		{
			DOUT(subCommands[0]);
		}
		else
			FOUT("Zu wenig Argumente! debug_Msg <msg>")
	}
	else if(mainCommand == "say")
	{
		if(commandNumber >= 5)
		{
			Message_Manager::getMe().showMsg(subCommands[0],sf::Color(atoi(subCommands[1].c_str()),atoi(subCommands[2].c_str()),atoi(subCommands[3].c_str())));
		}
		else
			FOUT("Zu wenig Argumente! say <msg> <r> <g> <b>")
	}
	else if(mainCommand == "toggleGlobalWall")
	{
		std::list<InteractivObject*> objList = Map_Manager::getMe().getCurrentMap().getObjectList();
		
		for(auto cursor:objList)
		{
			if(cursor->getObjectType() == InteractivObject::ObjectType::Global_Wall)
				cursor->executeObjectCommand("toggle");
		}
	}
	else if(mainCommand == "toggleGodMode") 
	{
		Player_Manager::getMe().toggleGodmode();
	}
	else if(mainCommand == "toggleDmgDebug") 
	{
		Weapon_Manager::getMe().toggleDamageDebug();
	}
	else if(mainCommand == "setObjectActivity")
	{
		if(commandNumber >= 3)
		{
			Map_Manager::getMe().getCurrentMap().getObject(atoi(subCommands[0].c_str()))->setActiv(atoi(subCommands[1].c_str()));
		}
	}
	else if(mainCommand == "doObjectCmd") 
	{
		if(commandNumber >= 3)
		{
			Map_Manager::getMe().getCurrentMap().getObject(atoi(subCommands[1].c_str()))->executeObjectCommand(subCommands[0]);
		}
	}
	else if(mainCommand == "setNPCActivity") 
	{
		if(commandNumber >= 3)
		{
			NPC_Manager::getMe().getNPC(atoi(subCommands[0].c_str()))->setActiv(atoi(subCommands[1].c_str()));
		}
	}
	else if(mainCommand == "setEnemyActivity") 
	{
		if(commandNumber >= 3)
		{
			NPC_Manager::getMe().getEnemy(atoi(subCommands[0].c_str()))->setActiv(atoi(subCommands[1].c_str()));
		}
	}
	else if(mainCommand == "changeMap") 
	{
		if(commandNumber >= 3)
		{
			Map_Manager::getMe().changeCurrentMap(subCommands[0],atoi(subCommands[1].c_str()));
		}
		else
		{
			FOUT("Zu wenig Argumente! changeMap <mapName> <spawnPoint_number>");
		}
	}
	else if(mainCommand == "spawnWeapon")
	{
		if(commandNumber >= 5)
		{
			Weapon_Manager::getMe().useWeapon(Weapon_Manager::weapon(atoi(subCommands[0].c_str())),atoi(subCommands[1].c_str()),atoi(subCommands[2].c_str()),atoi(subCommands[3].c_str()));
			IOUT("Waffe (" << subCommands[0] << ") wurde bei " << subCommands[1] << ":" << subCommands[2] << " gespawnt! (" << subCommands[3] << ")");
		}
		else
		{
			FOUT("Zu wenig Argumente! spawn_Weapon <weaponID> <x> <y> <dir>");
		}
	}
	else if(mainCommand == "enableMelody")
	{
		if(commandNumber >= 2)
		{
			Flute_Screen *flute = dynamic_cast<Flute_Screen*>(Screen_Manager::getMe().getScreen(Screen::Flute));
			flute->activateMelody(atoi(subCommands[0].c_str()));
		}
	}
	else if(mainCommand == "reloadGraphics") 
	{
		Graphic_Manager::getMe().loadAllGraphics();
	}
	else if(mainCommand == "end" || mainCommand == "exit") 
	{
  		Screen_Manager::getMe().getGameWindow().close();
	}
	else if(mainCommand == "doDialog") 
	{
		if(commandNumber >= 2)
		{
			Dialog_Manager::getMe().doDialog(atoi(subCommands[0].c_str()));
		}
	}
	else if(mainCommand == "openShop") 
	{
		Screen_Manager::getMe().changeScreen(Screen::Shop,false);
	}
	else if(mainCommand == "openBombShop") 
	{
		Screen_Manager::getMe().changeScreen(Screen::Bomb_Shop,false);
	}
	else if(mainCommand == "openPotionShop") 
	{
		Screen_Manager::getMe().changeScreen(Screen::Potion_Shop,false);
	}
	else if(mainCommand == "doEffect") 
	{
		if(commandNumber >= 2)
		{
			Screen_Manager::getMe().doEffect(Screen_Manager::Effect(atoi(subCommands[0].c_str())));
		}
	}
	else if(mainCommand == "cancelEffect") 
	{
		Screen_Manager::getMe().stopEffect();
	}
	else if(mainCommand == "changeMusic")
	{
		if(commandNumber >= 2)
		{
			Sound_Manager::getMe().changeBackgroundMusicSmoothly(subCommands[0]);
		}
	}
	else if(mainCommand == "setTilenumber")
	{
		if(commandNumber >= 4)
		{
			Map_Manager::getMe().getCurrentMap().getTile(atoi(subCommands[0].c_str()),atoi(subCommands[1].c_str()))->tileNumber = atoi(subCommands[2].c_str());
		}
	}
	else if(mainCommand == "lockMusic")
	{
		if(commandNumber >= 2)
		{
			Sound_Manager::getMe().setMusicLockStatus(atoi(subCommands[0].c_str()));
		}
	}
	else if(mainCommand == "fadeOut")
	{
		Screen_Manager::getMe().fadeOut();
	}
	else if(mainCommand == "fadeIn")
	{
		Screen_Manager::getMe().fadeIn();
	}
	else if(mainCommand == "update")
	{
		Screen_Manager::getMe().updateActivScreen();
	}
	else if(mainCommand == "forceEnding")
	{
		Player_Manager::getMe().doEndAnimationMovie();
	}
	else if(mainCommand == "help")
	{
		DOUT("");
		IOUT("HELP-Menü");
		DOUT("");
		IOUT("Teleportieren          - Syntax: tp <x> <y>")
		IOUT("Item geben             - Syntax: give <item_nr> <amount>")
		IOUT("Heilen                 - Syntax: heal <int>")
		IOUT("Shop öffnen            - Syntax: openShop")
		IOUT("Bomb Shop öffnen       - Syntax: openBombShop")
		IOUT("Dialog starten         - Syntax: doDialog <dialogID>")
		IOUT("Player Farbe ändern    - Syntax: changePlayerColor <r> <g> <b> <a>")
		IOUT("Player explodieren     - Syntax: explode")
		IOUT("Player töten           - Syntax: kill")
		IOUT("Player zoomen          - Syntax: zoom <percent>")
		IOUT("Gott Modus             - Syntax: toggleGodMode")
		IOUT("Schaden anzeigen       - Syntax: toggleDmgDebug")
		IOUT("Effekt starten         - Syntax: doEffect <EffectID>")
		IOUT("Effekt stoppen         - Syntax: cancelEffect")
		IOUT("Konsolen Nachricht     - Syntax: debugMsg <msg>")
		IOUT("Ingame Nachricht       - Syntax: say <msg> <r> <g> <b>")
		IOUT("Objekt Befehl          - Syntax: doObjectCmd <cmd> <ID>")
		IOUT("Objekt De/-Aktivieren  - Syntax: setObjectActivity <ID> <bool>")
		IOUT("NPC De/-Aktivieren     - Syntax: setNPCActivity <ID> <bool>")
		IOUT("Map ändern             - Syntax: changeMap <Map Name> <Spawnpoint ID>")
		IOUT("Map Tile ändern        - Syntax: setTilenumber <x> <y> <tilenumber>")
		IOUT("Waffe erstellen        - Syntax: spawnWeapon <Weapon ID> <Amount>")
		IOUT("Grafiken erneut ladnen - Syntax: reloadGraphics")
		IOUT("Bildschirm abdunkeln   - Syntax: fadeOut")
		IOUT("Bildschirm erhellen    - Syntax: fadeIn")
		IOUT("Bildschirm updaten     - Syntax: update")
		IOUT("Melody erlernen        - Syntax: enableMelody <ID>")
		IOUT("Musik ändern           - Syntax: changeMusic <Path>")
		IOUT("Musik Konstant setzen  - Syntax: lockMusic")
		IOUT("Spiel beenden          - Syntax: end")
	}
	else
	{
		FOUT("Der Befehl '" << mainCommand << "' konnte nicht gefunden werden!")
	}

	delete [] subCommands;
}

std::string Command_Manager::replace$Strings(std::string replaceString)
{
	bool $StringFound = false;

	int foundPos = replaceString.find("$playername");

	if(foundPos != std::string::npos)
	{
		replaceString.replace(replaceString.find("$playername"),11,File_Manager::getMe().getPlayerName());
		$StringFound = true;
	}

	foundPos = replaceString.find("$enter");

	if(foundPos != std::string::npos)
	{
		replaceString.replace(replaceString.find("$enter"),6,"\n");
		$StringFound = true;
	}

	foundPos = replaceString.find("$up");

	if(foundPos != std::string::npos)
	{
		replaceString.replace(replaceString.find("$up"),3,"^");
		$StringFound = true;
	}

	foundPos = replaceString.find("$down");

	if(foundPos != std::string::npos)
	{
		replaceString.replace(replaceString.find("$down"),5,"³");
		$StringFound = true;
	}

	foundPos = replaceString.find("$left");

	if(foundPos != std::string::npos)
	{
		replaceString.replace(replaceString.find("$left"),5,"§");
		$StringFound = true;
	}

	foundPos = replaceString.find("$right");

	if(foundPos != std::string::npos)
	{
		replaceString.replace(replaceString.find("$right"),6,"²");
		$StringFound = true;
	}

	if($StringFound) replaceString = replace$Strings(replaceString);

	return replaceString;
}

void Command_Manager::enterCommand()
{
	if(Screen_Manager::getMe().debugMode())
	{
		DOUT("Bitte geben sie ein Befehl ein!")

		SetForegroundWindow(GetConsoleWindow());

		std::string eingabe;
		std::getline(std::cin,eingabe,'\n');
		
		Screen_Manager::getMe().getGameWindow().requestFocus();

		doCommand(eingabe);

		SetForegroundWindow(Screen_Manager::getMe().getGameWindowHandle());
	}
}

void Command_Manager::wait(sf::Time waitTime)
{
	clock_t time_end;
    time_end = clock() + waitTime.asMicroseconds() * CLOCKS_PER_SEC/1000000;
    while (clock() < time_end){}
}

//Destruktor
Command_Manager::~Command_Manager(void)
{
	DELOUT("Command_Manager wurde geloescht!\t(" << this << ")")
}