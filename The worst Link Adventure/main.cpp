#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"

#include "Defines.h"
#include <thread>
#include <iostream>
#include <list>
#include "Graphic_Manager.h"
#include "Command_Manager.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "Sound_Manager.h"
#include "Screen_Manager.h"
#include "File_Manager.h"

#include <tchar.h>

std::list<sf::Event> events;
sf::Mutex mute;

void checkEvents()
{
	MSG msg;
	HWND *mainWindow = nullptr;

	//Fenster immer Updaten --> No responding Fehler vermeiden
	while(true)
	{
		mute.lock();

		mainWindow = &Screen_Manager::getMe().getGameWindowHandle();

		PeekMessage(&msg, *mainWindow, 0, 0,PM_NOREMOVE);
		
		Sleep(100);

		mute.unlock();
	}
}

//https://github.com/CHP-EmAS/TWLA

int main(int argc, char * argv[])
{
	bool debug = false;

	if(argc > 1)
	{
		if(std::string(argv[1]) == "-d") 
		{
			debug = true;
		}
	}

	setlocale(LC_CTYPE, "");
	std::ofstream cout("log/log.txt");

	sf::Thread updateThread(&checkEvents);

	fflush(stdin);
	std::cin.clear();

	srand(time(NULL));

	EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	if(!debug) //Konsolen-Fenster verstecken falls Debugmode aus
	{
		ShowWindow( GetConsoleWindow(), SW_HIDE);
	}

	//Alle unabh�nigen Manager laden
	if(debug){IOUT("Debug = ON\n")}
	else{IOUT("Debug = OFF\n")}

	IOUT("Nowendige Manager werden geladen\n");

	Command_Manager::getMe();

	//Grafiken und Fonts laden
	Graphic_Manager::getMe().loadAllGraphics();
	Graphic_Manager::getMe().loadAllFonts();
	Graphic_Manager::getMe().initAnimations();

	//Screens laden
	Screen_Manager::getMe().initScreens(debug);

	Animation_Manager::getMe();
	Particle_Manager::getMe();

	if(!Sound_Manager::getMe().loadAllSounds())
	{
		FOUT("Nicht alle Sounds wurden korrekt geladen!")
	}

	//File_Manager auf Standart setzen
	File_Manager::getMe().forceReset();
	File_Manager::getMe().loadSettings();

	//Aktuellen Screen auf den Logoscreen setzen
	Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Logos,false);

	sf::Clock drawClock;
	sf::Clock updateClock;
	
	updateThread.launch();
	mute.lock();
	mute.unlock();

	float updateWaitDelay;
	float drawWaitDelay;
	float updateWaitTime;
	float drawWaitTime;

	//Haupt-Loop
	while(Screen_Manager::getMe().getGameWindow().isOpen())
	{
		updateWaitDelay = (1000000.f / Screen_Manager::getMe().getActivScreenUpdateLimit());

		//Updaten des Fensters
		if(updateClock.getElapsedTime().asMicroseconds() >= updateWaitDelay)
		{
			updateClock.restart();

			Screen_Manager::getMe().checkWindowEvents();
			Screen_Manager::getMe().updateActivScreen();
			Graphic_Manager::getMe().updateAnimations();
		}
		
		drawWaitDelay = (1000000.f / Screen_Manager::getMe().getActivScreenFPSLimit());

		//Zeichnen des Fensters
		if(drawClock.getElapsedTime().asMicroseconds() >= drawWaitDelay)
		{
			drawClock.restart();
			Screen_Manager::getMe().drawActivScreen();
		}

		//Wartezeiten berechnen
		updateWaitTime = updateWaitDelay - updateClock.getElapsedTime().asMicroseconds();
		drawWaitTime = drawWaitDelay - drawClock.getElapsedTime().asMicroseconds();

		//Damit nicht st�ndig wiederholt wird, gegebenenfalls warten
		if(updateWaitTime<=drawWaitTime)
		{
			if(updateWaitTime-500 > 0)
			std::this_thread::sleep_for(std::chrono::microseconds(long(updateWaitTime-500)));
		}
		else if(drawWaitTime-500 > 0)
		{
			std::this_thread::sleep_for(std::chrono::microseconds(long(drawWaitTime-500)));
		}
	}
	
	//Spiel beenden
	DOUT("")
	IOUT("Beenden!\n")

	updateThread.terminate();

	Screen_Manager::getMe().deleteScreens();

	//Alle Manager l�schen
	Graphic_Manager::deleteMe();
	Command_Manager::deleteMe(); 
	Animation_Manager::deleteMe();
	Particle_Manager::deleteMe();
	Sound_Manager::deleteMe();

	//Muss als letztes gel�scht werden
	Screen_Manager::deleteMe();
	File_Manager::deleteMe();

	std::cout << "\n";

	EnableMenuItem(GetSystemMenu(GetConsoleWindow(), FALSE), SC_CLOSE, MF_BYCOMMAND | MF_ENABLED);

	//Falls im Debug-Modus Konsole vor Beenden pauseieren
	if(debug)
	{
		system("pause");
	}
	
	return EXIT_SUCCESS;
}

