#include "Screens.h"

#include "Map_Manager.h"
#include "Graphic_Manager.h"
#include "Message_Manager.h"
#include "Command_Manager.h"
#include "Weapon_Manager.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "Sound_Manager.h"
#include "NPC_Manager.h"
#include "Screen_Manager.h"
#include "Quest_Manager.h"
#include "Player_Manager.h"
#include "Dialog_Manager.h"
#include "File_Manager.h"

Game_Screen::Game_Screen(void) : Screen(Screen::Screen_Type::Game)
{
	initScreen();
}

void Game_Screen::updateScreen()
{
	Player_Manager::getMe().update();
	
	NPC_Manager::getMe().update_Enemys_NPCs();
	
	Particle_Manager::getMe().updateParticle();
	
	Weapon_Manager::getMe().updateActivWeapons();
	
	Animation_Manager::getMe().updateAnimations();

	Sound_Manager::getMe().updateManager();

	//calculate fps
	if(fpsClock.getElapsedTime().asMicroseconds() > 1000000)
	{
		fpsClock.restart();

		int cCounter = Collision_System::getMe().getCollisionCounter();

		if(fps <= 0) fpsText.setFillColor(sf::Color(255,255,255));
		else if(fps < 21) fpsText.setFillColor(sf::Color(255,0,0));
		else if(fps < 30) fpsText.setFillColor(sf::Color(255,255,0));
		else if(fps < 100) fpsText.setFillColor(sf::Color(0,255,0));
		else fpsText.setFillColor(sf::Color(0,0,255, 150));

		if(cCounter <= 0) collisionText.setFillColor(sf::Color(255,255,255));
		else if(cCounter > 30000) collisionText.setFillColor(sf::Color(255,0,0));
		else if(cCounter > 20000) collisionText.setFillColor(sf::Color(255,255,0));
		else if(cCounter > 10000) collisionText.setFillColor(sf::Color(0,255,0));
		else collisionText.setFillColor(sf::Color(0,0,255, 150));

		fpsText.setString("FPS:"+std::to_string(fps));
		collisionText.setString("cc:"+std::to_string(cCounter));

		fps = 0;
		Collision_System::getMe().resetCollisionCounter();
	}
}

void Game_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	fps++;

	//draw map
	Map_Manager::getMe().drawMap(mainWindow);

	//draw Weapons
	Weapon_Manager::getMe().drawActivWeapons(mainWindow);

	//NPCs
	NPC_Manager::getMe().draw_NPCs(mainWindow);

	//draw link
	Player_Manager::getMe().drawPlayer(mainWindow);

	//NPCs
	NPC_Manager::getMe().draw_Enemys(mainWindow);

	//draw Particle
	Particle_Manager::getMe().drawParticle(mainWindow);

	//draw shadow
	Map_Manager::getMe().drawShadowOverlay(mainWindow);

	//draw Messages
	Message_Manager::getMe().drawMessages(mainWindow);

	//draw hud
	Player_Manager::getMe().getGameHUD().draw(mainWindow);

	//draw fps
	if(showFPS)
	{
		mainWindow.draw(fpsText);
		mainWindow.draw(collisionText);
	}
}

void Game_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:
	{
		//STRG
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::LControl) || sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::F)) //F
			{
				showFPS = !showFPS;
				fpsText.setString("0");
				fpsText.setFillColor(sf::Color(255, 255, 255, 255));
				fps = 0;
				fpsClock.restart();
			}

			if(Screen_Manager::getMe().debugMode())
			{
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::K)) //K
				{
					Map_Manager::getMe().showFrames(!(Map_Manager::getMe().isDrawingFrames()));
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::Tab)) //Tab
				{
					Command_Manager::getMe().enterCommand();
				}
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E)) //E
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Inventory,false);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::P)) //P
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Pause,false);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::F2))
		{
			File_Manager::getMe().saveCurrentMapChanges();
			File_Manager::getMe().save_Savegame();
		}
		break;
	}
	case sf::Event::LostFocus:
	{
		Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Pause,false);
		break;
	}
	}
}

void Game_Screen::loadScreen()
{
	initScreen();

	DOUT("");
	IOUT("||||||************ Spiel wird gestartet! ************||||||");
	IOUT("Version: " << GAME_VERSION)
	
	DOUT("");
	IOUT("Items werden geladen")

	//Items laden
	Item_List::getMe().loadAllItems();

	DOUT("");
	IOUT("Nowendige Spiel-Manager werden geladen")

	//Manager erstellen
	Map_Manager::getMe();
	Message_Manager::getMe();
	Dialog_Manager::getMe();
	Weapon_Manager::getMe();
	NPC_Manager::getMe();
	Collision_System::getMe();
	Quest_Manager::getMe();
	Inventory_Manager::getMe();
	Player_Manager::getMe();
	
	setLoaded(true);
}

void Game_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Game_Screen::closeScreen()
{
	File_Manager::getMe().forceReset();

	Message_Manager::deleteMe();
	Dialog_Manager::deleteMe();
	Map_Manager::deleteMe();
	Item_List::deleteMe();
	Weapon_Manager::deleteMe();
	NPC_Manager::deleteMe();
	Collision_System::deleteMe();
	Quest_Manager::deleteMe();
	Inventory_Manager::deleteMe();
	Player_Manager::deleteMe();

	Sound_Manager::getMe().clearAllEffectSounds();
	Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Stopped);
	Sound_Manager::getMe().setMusicLockStatus(false);
	
	Screen_Manager::getMe().stopEffect();

	setLoaded(false);
}

void Game_Screen::initScreen()
{
	//fps
	fpsText = sf::Text("FPS:00000",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),17);
	fpsText.setPosition(3,0);
	fpsText.setFillColor(sf::Color(255, 255, 255));
	showFPS = Screen_Manager::getMe().debugMode();
	fps = 0;

	//collision counter
	collisionText = sf::Text("cc:0",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),17);
	collisionText.setPosition(fpsText.getPosition().x + fpsText.getGlobalBounds().width, 0);
	collisionText.setFillColor(sf::Color(255, 255, 255));
}

Game_Screen::~Game_Screen(void)
{
	closeScreen();
}
