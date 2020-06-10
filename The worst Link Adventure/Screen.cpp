#include "Screen.h"

Screen::Screen(Screen::Screen_Type type)
{
	this->type = type;
	isLoaded = false;
	Screen* screenBeforThis = nullptr;
}

void Screen::setLastScreen(Screen* lastScreen)
{
	screenBeforThis = lastScreen;
}

Screen* Screen::getLastScreen()
{
	return screenBeforThis;
}

void Screen::setLoaded(bool load)
{
	isLoaded = load;
}

bool Screen::isScreenLoaded()
{
	return isLoaded;
}

Screen::Screen_Type Screen::getScreenType()
{
	return type;
}

Screen::~Screen(void)
{

}
