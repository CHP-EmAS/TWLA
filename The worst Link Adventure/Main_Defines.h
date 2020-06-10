#ifndef MAIN_DEFINES
#define MAIN_DEFINES

#include <windows.h>
#include <iostream>
#include <string>

#define GAME_VERSION "1.2 Stable" 
#define CHEAT_NAME "PikachuLink" //max 11 Zeichen

//Data-Handling
#define ZELDA_DATA_ENDING  ".zData"
#define ZELDA_QUEST_ENDING ".zQuest"
#define ZELDA_DIALOG_ENDING ".zDialog"

//Difficulty
#define IMMORTAL 0
#define EASY 1
#define NORMAL 2
#define DIFFICULT 3
#define HARDCORE 4


//Singelton Template
template <typename  C>
class Singleton
{
public:
	static C& getMe();
	static void deleteMe();
protected:
	static C* instanz;

	Singleton(){}
	~Singleton(){deleteMe();}
};

template <typename  C>
typename C* Singleton<C>::instanz = nullptr;

template <class C>
C& Singleton<C>::getMe()
{
	if(instanz == nullptr) Singleton<C>::instanz = new C();
	return *instanz;
}

template <typename  C>
void Singleton<C>::deleteMe()
{
	if(instanz != nullptr) 
	{
		C *temp = Singleton<C>::instanz;
		Singleton<C>::instanz = nullptr;
		delete temp;
	}
}

#endif //Main_Defines