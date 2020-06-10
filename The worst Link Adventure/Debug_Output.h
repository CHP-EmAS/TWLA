#pragma once

#include <iostream>
#include <Windows.h>
#include "Screen_Manager.h"
#include "File_Manager.h"

#define DIN(x)    if(Screen_Manager::getMe().debugMode()){std::cout << "Eingabe: "; std::cin >> x;};

#define DOUT(x)   if(Screen_Manager::getMe().debugMode()){std::cout << x << "\n";}File_Manager::getMe().getLogStream() << x << "\n";
#define SOUT(x)   if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 10); std::cout << '+'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[CHECK]- " << x << "\n";
#define NEWOUT(x) if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 14); std::cout << '*'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[NEW]- " << x << "\n";
#define DELOUT(x) if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);  std::cout << '/'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[DELETE]- " << x << "\n";
#define FOUT(x)   if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12); std::cout << 'X'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[FAIL]- " << x << "\n";
#define IOUT(x)   if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 11); std::cout << 'i'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[INFO]- " << x << "\n";
#define QOUT(x)   if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 5);  std::cout << '?'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[QUESTION]- " << x << "\n";
#define WOUT(x)   if(Screen_Manager::getMe().debugMode()){std::cout << "[ "; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);  std::cout << '!'; SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15); std::cout << " ] - " << x << std::endl;}File_Manager::getMe().getLogStream() << "[WARNING]- " << x << "\n";

#define CRITOUT(x) MessageBox(NULL,TEXT(x),TEXT("Kritischer Fehler!"),MB_ICONERROR | MB_OK);File_Manager::getMe().getLogStream() << "[CRITICAL]- " << x << "\n";
