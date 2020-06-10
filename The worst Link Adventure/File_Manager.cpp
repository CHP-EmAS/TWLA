#include "File_Manager.h"

#include "Defines.h"
#include <fstream>
#include "Map_Manager.h"
#include "Player_Manager.h"
#include "NPC_Manager.h"
#include "Sound_Manager.h"
#include "Screen_Manager.h"
#include "Weapon_Manager.h"
#include "Particle_Manager.h"
#include "Quest_Manager.h"
#include "Message_Manager.h"

#include "Screens.h"


std::string File_Manager::create_SaveGame(std::string name, int difficulty)
{
	time_t t = time(0);
	struct tm* now = localtime(&t);

	//Ordnername
	std::string dirName = "Savegames/save#";

	dirName += std::to_string(now->tm_mday);
	dirName += std::to_string(now->tm_mon+1);
	dirName += std::to_string(now->tm_year + 1900);
	dirName += std::to_string(now->tm_hour);
	dirName += std::to_string(now->tm_min);
	dirName += std::to_string(now->tm_sec);

	std::wstring stemp = std::wstring(dirName.begin(), dirName.end());
	LPCWSTR sw = stemp.c_str();
	CreateDirectory(sw, NULL);

	currentSaveGame.path = dirName;

	//Mapordner
	stemp += std::wstring(TEXT("/maps"));
	sw = stemp.c_str();
	CreateDirectory(sw, NULL);

	//Haupt Daten
	std::ofstream ofs;
	std::string path = dirName + "/main" + ZELDA_DATA_ENDING;
	
	ofs.open(path, std::ios::out);

	if(ofs.good())
	{ 
		std::string content = "{name}" + name + "\n";
		content += "{date}" + std::to_string(now->tm_mday) + "." + std::to_string(now->tm_mon+1) + "." + std::to_string(now->tm_year + 1900) + " ";
		content += std::to_string(now->tm_hour) + ":" + std::to_string(now->tm_min) + "\n";
		content += "{diff}" + std::to_string(difficulty) + "\n";
		content += "{player}4,1488,2864,2,\n";
		content += "{hud}4,4,500,500,0,\n";

		if(name == CHEAT_NAME)
			content += "{inventory}1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,20,1,20,20,1,1,1,1,1,1,1,1,1,1,1,1,0,1,3,16,6,9,12,14,0,0,0,\n";
		else
			content += "{inventory}" + Inventory_Manager::getMe().getInventorySaveData() + "\n";

		content += "{map}Wald von Emasin,1488,2864\n";
		content += "{stats}0,0,0,0,0,0,0,\n";

		ofs.write(content.c_str(),content.size());
	}
	else
	{
		FOUT("Fehler beim Erstellen der Haupt-Daten in <" << path << ">");
	}

	ofs.close();

	//Quest Daten
	std::ofstream ofs_q;

	path = dirName + "/questData" + ZELDA_DATA_ENDING;

	ofs_q.open(path, std::ios::out);

	if(ofs_q.good())
	{
		std::string content = Quest_Manager::getMe().getQuest_ManagerSaveData();
		ofs_q.write(content.c_str(),content.size());
	}
	else
	{
		FOUT("Fehler beim Erstellen der Quest-Daten in <" << path << ">")
	}

	ofs_q.close();

	return dirName;
}

bool File_Manager::load_Savegame(std::string saveGamePath)
{
	//Quests Laden
	Quest_Manager::getMe().loadQuest_ManagerSaveData(saveGamePath + "/questData" + ZELDA_DATA_ENDING);

	//Main Laden
	std::list<fullCommand> commandList = convertFileToCommandlist(saveGamePath + "/main" + ZELDA_DATA_ENDING);

	currentSaveGame.path = saveGamePath;

	for(auto cursor : commandList)
	{
		if(cursor.command == "name")
		{
			currentSaveGame.name = cursor.content;
		}
		else if(cursor.command == "date")
		{
			currentSaveGame.date = cursor.content;
		}
		else if(cursor.command == "diff")
		{
			currentSaveGame.difficulty = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "player")
		{
			Player_Manager::getMe().loadPlayerSaveData(cursor.content);
		}
		else if(cursor.command == "hud")
		{
			Player_Manager::getMe().getGameHUD().loadHUDSaveData(cursor.content); 
		}
		else if(cursor.command == "inventory")
		{
			Inventory_Manager::getMe().loadInventorySaveData(cursor.content);
		}
		else if(cursor.command == "map")
		{
			Map_Manager::getMe().loadMySaveData(cursor.content);
		}
		else if(cursor.command == "flute")
		{
			Flute_Screen *flute = dynamic_cast<Flute_Screen*>(Screen_Manager::getMe().getScreen(Screen::Flute));
			flute->loadMySaveData(cursor.content);
		}
		else if(cursor.command == "shop")
		{
			Shop_Screen *shop = dynamic_cast<Shop_Screen*>(Screen_Manager::getMe().getScreen(Screen::Shop));
			shop->loadMySaveData(cursor.content);
		}
		else if(cursor.command == "stats")
		{
			Player_Manager::getMe().getGameStats().loadSaveData(cursor.content);
		}
	}

	Screen_Manager::getMe().updateActivScreen();
	Screen_Manager::getMe().drawActivScreen();

	return false;
}

bool File_Manager::save_Savegame()
{
	//Prüfe ob Specherstand-Pfad vorhanden
	if(currentSaveGame.path == "")
	{
		FOUT("Spiel konnte nicht gespeichert werden, da kein Spiel geladen ist!");
		return false;
	}
	else
	{
		IOUT("Spiel wird gespeichert!")
	}

	//Haupt Daten abspeichern -> main.zData
	std::ofstream ofs;
	std::string path = currentSaveGame.path + "/main" + ZELDA_DATA_ENDING;
	
	ofs.open(path, std::ios::out);

	if(ofs.good())
	{ 
		Flute_Screen *flute = dynamic_cast<Flute_Screen*>(Screen_Manager::getMe().getScreen(Screen::Flute));
		Shop_Screen *shop = dynamic_cast<Shop_Screen*>(Screen_Manager::getMe().getScreen(Screen::Shop));

		std::string content = "{name}" + currentSaveGame.name + "\n";
		content += "{date}" + currentSaveGame.date + "\n";
		content += "{diff}" + std::to_string(currentSaveGame.difficulty) + "\n";
		content += "{player}" + Player_Manager::getMe().getPlayerSaveData() + "\n";
		content += "{hud}" + Player_Manager::getMe().getGameHUD().getHUDSaveData() + "\n";
		content += "{inventory}" + Inventory_Manager::getMe().getInventorySaveData() + "\n";
		content += "{map}" + Map_Manager::getMe().getMySaveData() + "\n";
		content += "{flute}" + flute->getMySaveData() + "\n";
		content += "{shop}" + shop->getMySaveData() + "\n";
		content += "{stats}" + Player_Manager::getMe().getGameStats().getSaveData() + "\n";
		
		ofs.write(content.c_str(),content.size());
	}
	else
	{
		FOUT("Fehler beim Öffnen der Haupt-Daten in <" << path << ">");
		return false;
	}

	ofs.close();

	//Screenshot vom Spieler abspeichern
	sf::Image screenshot = doScreenShotFromPlayer();
	screenshot.saveToFile(currentSaveGame.path + "/ss.png");

	//Quest Daten abspeichern
	std::ofstream ofs_q;

	std::string path_q = currentSaveGame.path + "/questData" + ZELDA_DATA_ENDING;
	ofs_q.open(path_q, std::ios::out);

	if(ofs_q.good())
	{
		std::string content = Quest_Manager::getMe().getQuest_ManagerSaveData();
		ofs_q.write(content.c_str(),content.size());
	}
	else
	{
		FOUT("Fehler beim Erstellen der Quest-Daten in <" << path_q << ">");
		return false;
	}

	ofs_q.close();

	//Speicher Icon anzeigen
	Message_Manager::getMe().showSaveIcon();

	return true;
}

sf::Image File_Manager::doScreenShotFromPlayer()
{
	if(!Screen_Manager::getMe().getScreen(Screen::Game)->isScreenLoaded())
	{
		WOUT("Ein Screenshot vom Spieler kann nur bei geladenem Spiel erfolgen!")

		sf::Image errorImg;
		errorImg.create(9 * ZOOMSIZE,9 * ZOOMSIZE,sf::Color::Transparent);

		return errorImg;
	}

	Screen_Manager::getMe().getGameWindow().clear();

	//draw
	Map_Manager::getMe().drawMap(Screen_Manager::getMe().getGameWindow());
	Weapon_Manager::getMe().drawActivWeapons(Screen_Manager::getMe().getGameWindow());
	NPC_Manager::getMe().draw_NPCs(Screen_Manager::getMe().getGameWindow());
	Player_Manager::getMe().drawPlayer(Screen_Manager::getMe().getGameWindow());
	NPC_Manager::getMe().draw_Enemys(Screen_Manager::getMe().getGameWindow());
	Particle_Manager::getMe().drawParticle(Screen_Manager::getMe().getGameWindow());

	int posX = Player_Manager::getMe().getPosX();
	int posY = Player_Manager::getMe().getPosY();

	sf::Texture ss;
	ss.create(Screen_Manager::getMe().getGameWindow().getSize().x,Screen_Manager::getMe().getGameWindow().getSize().y);
	ss.update(Screen_Manager::getMe().getGameWindow(),0,0);

	sf::Image img_ss = ss.copyToImage();

	sf::Image player_ss;
	player_ss.create(9 * ZOOMSIZE,9 * ZOOMSIZE);

	for(int x = 0; x < (9 * ZOOMSIZE);x++)
	{
		for(int y = 0; y < (9 * ZOOMSIZE);y++)
		{
			sf::Vector2i newCoords = Screen_Manager::getMe().getGameWindow().mapCoordsToPixel(sf::Vector2f(posX - (4 * ZOOMSIZE) + x,posY - (4 * ZOOMSIZE) + y));
			player_ss.setPixel(x,y,img_ss.getPixel(newCoords.x,newCoords.y));
		}
	}

	Screen_Manager::getMe().drawActivScreen();

	return player_ss;
}

bool File_Manager::loadCurrentMapChanges()
{
	if(currentSaveGame.path == "") return false;

	std::string fileName = currentSaveGame.path;

	fileName += "/maps/" + std::to_string(Map_Manager::getMe().getCurrentMap().getID()) + ZELDA_DATA_ENDING;

	std::list<fullCommand> saves = convertFileToCommandlist(fileName);

	for(auto cursor : saves)
	{
		if(cursor.command == "npc")
		{
			NPC_Manager::getMe().loadNPCSaveData(cursor.content);
		}
		else if(cursor.command == "object")
		{
			Map_Manager::getMe().load_Object_Savedata_from_current_Map(cursor.content);
		}
		else if(cursor.command == "enemy")
		{
			NPC_Manager::getMe().loadEnemySaveData(cursor.content);
		}
	}

	Screen_Manager::getMe().updateActivScreen();
	Screen_Manager::getMe().drawActivScreen();

	return false;
}

bool File_Manager::saveCurrentMapChanges()
{
	if(currentSaveGame.path == "") return false;

	std::string fileName = currentSaveGame.path;

	fileName += "/maps/" + std::to_string(Map_Manager::getMe().getCurrentMap().getID()) + ZELDA_DATA_ENDING;

	std::ofstream ofs;
	
	ofs.open(fileName, std::ios::out);

	if(ofs.good())
	{ 
		std::string npcData = NPC_Manager::getMe().getNPCSaveData();
		ofs.write(npcData.c_str(),npcData.size());

		std::string objData = Map_Manager::getMe().get_Objects_Savedata_from_current_Map();
		ofs.write(objData.c_str(),objData.size());
	}

	ofs.close();

	return false;
}

bool File_Manager::saveSettings()
{
	std::ofstream ofs;
	
	ofs.open("settings.zData", std::ios::out);

	std::string fulldata;

	if(ofs.good())
	{ 
		fulldata += "{mastervolume}" + std::to_string(Sound_Manager::getMe().getMasterVolume()) + "\n";
		fulldata += "{musicvolume}" + std::to_string(Sound_Manager::getMe().getMusikVolume()) + "\n";
		fulldata += "{effectrvolume}" + std::to_string(Sound_Manager::getMe().getEffektVolume()) + "\n";
		fulldata += "{textSound}" + std::to_string(Sound_Manager::getMe().getDoDialogSound()) + "\n";
		
		Options_Menue *menue = dynamic_cast<Options_Menue*>(Screen_Manager::getMe().getScreen(Screen::Screen_Type::Options));

		fulldata += "{videomode}" +  std::to_string(menue->getVideoModeSetting()) + "\n";
		fulldata += "{windowwidth}" +  std::to_string(menue->getFrameWindowSizeX()) + "\n";
		fulldata += "{windowhight}" +  std::to_string(menue->getFrameWindowSizeY()) + "\n";
		fulldata += "{fpslimit}" +  std::to_string(Screen_Manager::getMe().getGameFPSLimit()) + "\n";
		fulldata += "{showParticle}" + std::to_string(Screen_Manager::getMe().getShowParticle()) + "\n";
		
		ofs.write(fulldata.c_str(),fulldata.size());
	}
	else
	{
		ofs.close();
		return false;
	}

	ofs.close();
	return true;
}

bool File_Manager::loadSettings()
{
	std::list<fullCommand> settings = convertFileToCommandlist("settings.zData");
	
	int videoMode = -1;
	int sizeX = -1;
	int sizeY = -1;

	for(auto cursor : settings)
	{
		if(cursor.command == "mastervolume")
		{
			Sound_Manager::getMe().setMasterVolume(atoi(cursor.content.c_str()));
		}
		else if(cursor.command == "musicvolume")
		{
			Sound_Manager::getMe().setMusikVolume(atoi(cursor.content.c_str()));
		}
		else if(cursor.command == "effectrvolume")
		{
			Sound_Manager::getMe().setEffektVolume(atoi(cursor.content.c_str()));
		}
		else if(cursor.command == "videomode")
		{
			videoMode = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "windowwidth")
		{
			sizeX = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "windowhight")
		{
			sizeY = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "fpslimit")
		{
			Screen_Manager::getMe().setGameFPSLimit(atoi(cursor.content.c_str()));
		}
		else if(cursor.command == "textSound")
		{
			Sound_Manager::getMe().setDoDialogSound(atoi(cursor.content.c_str()));
		}
		else if(cursor.command == "showParticle")
		{
			Screen_Manager::getMe().setShowParticle(atoi(cursor.content.c_str()));
		}
	}

	if(videoMode != -1 && sizeX != -1 && sizeY != -1)
	{
		Options_Menue *menue = dynamic_cast<Options_Menue*>(Screen_Manager::getMe().getScreen(Screen::Screen_Type::Options));
		menue->setVideoSettings(sizeX,sizeY,videoMode);
	}
	else
	{
		WOUT("Video Einstellungen konnten nicht geladen werden!");
		return false;
	}

	return true;
}

std::list<fullCommand> File_Manager::convertFileToCommandlist(std::string filePath)
{
	std::ifstream ifs;

	std::list<fullCommand> list; //Liste der Befehle			
	fullCommand dummy; //Dummy Befehl

	//Datei öffnen
	ifs.open(filePath, std::ios::in);

	if(ifs.good())
	{
		std::string content = "";

		//Gesamte Datei in einen String speichern
		while(!ifs.eof())
		{
			char dummy[1000];
			ifs.getline(dummy,1000);

			if(std::strcmp(dummy,"") != 0)
			{
				content += dummy;
				content += "\n";
			}
		}

		//Wiederholen solange sich noch was in content befindet
		while(content.size() > 0)
		{
			int pos = content.find("{"); //Suche {
			content = content.substr(pos+1); //Lösche alles bis {

			pos = content.find("}"); //Suche }
			
			if(pos == std::string::npos)
			{
				//Befehlsklammern wurden nicht geschlossen
				dummy.command = content;
				content = "";
				FOUT("Unerwartetes Datei-Ende! <" << filePath << ">");
			}
			else
			{
				//Befehl hinzufügen
				dummy.command = content.substr(0,pos);
				content = content.substr(pos+1);
			}

			//Suche Zeilenumbruch
			pos = content.find("\n");

			if(pos == std::string::npos)
			{
				//Letzter Befehl der Datei gefunden
				dummy.content = content;
				content = "";
			}
			else
			{
				//Speicher alles bis zum Zeilenumbruch als Inhalt ab
				dummy.content = content.substr(0,pos);
				content = content.substr(pos+1);
			}

			//Hinzufügen das kompletten Befehles in die Liste
			list.push_back(dummy);
		}
	}
	else
	{
		WOUT("Datei <" << filePath << "> konnte nicht geöffnet werden!");
	}

	ifs.close();

	return list; //Liste zurückgeben
}

std::string File_Manager::getPlayerName()
{
	return currentSaveGame.name;
}

int File_Manager::getGameDifficulty()
{
	return currentSaveGame.difficulty;
}

void File_Manager::forceReset()
{
	currentSaveGame.date = "NONE";
	currentSaveGame.name = "";
	currentSaveGame.path = "";
	currentSaveGame.difficulty = 2;
}

std::ofstream& File_Manager::getLogStream()
{
	return logStream;
}

File_Manager::File_Manager(void)
{
	Singleton::instanz = this;

	NEWOUT("Neuer File_Manager wurde erstellt! (" << this << ")");

	//Log
	time_t t = time(0);
	struct tm* now = localtime(&t);

	std::string dirName = "logs/log#";

	dirName += std::to_string(now->tm_mday);
	dirName += std::to_string(now->tm_mon+1);
	dirName += std::to_string(now->tm_year + 1900);
	dirName += std::to_string(now->tm_hour);
	dirName += std::to_string(now->tm_min);
	dirName += std::to_string(now->tm_sec);

	dirName += ".txt";

	logStream.open(dirName.c_str(),std::ios::out);
}

File_Manager::~File_Manager(void)
{
	logStream.close();
}
