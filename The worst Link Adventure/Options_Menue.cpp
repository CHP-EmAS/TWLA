#include "Screens.h"
#include "Screen_Manager.h"

#include "Graphic_Manager.h"
#include "Sound_Manager.h"
#include "File_Manager.h"

Options_Menue::Options_Menue(void) : Screen(Screen::Screen_Type::Options)
{
	initScreen();
}

void Options_Menue::updateScreen()
{

}

void Options_Menue::drawScreen(sf::RenderWindow &mainWindow)
{
	mainWindow.draw(mainFrame);
	
	headlineText.setFillColor(sf::Color::White);
	headlineText.setString("Einstellungen");
	headlineText.setCharacterSize(30);
	headlineText.setPosition(WINDOW_SIZE_X/2 - headlineText.getGlobalBounds().width/2,17);
	mainWindow.draw(headlineText);

	//***Sound Abschnitt***//
	headlineText.setString("Sound"); //Überschrift
	headlineText.setCharacterSize(25);
	headlineText.setPosition(WINDOW_SIZE_X/2 - headlineText.getGlobalBounds().width/2,90);
	mainWindow.draw(headlineText);

	line.setFillColor(sf::Color::Green); // Trennlinien
	line.setSize(sf::Vector2f((mainFrame.getSize().x-40)/2.5,3));
	line.setPosition(mainFrame.getPosition().x+20,105);
	mainWindow.draw(line);

	line.setPosition(headlineText.getPosition().x+90,105);
	mainWindow.draw(line);

	//Gesamte Lautstärke
	if(selectedOption == 0) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Gesamt-Lautstärke");
	headlineText.setCharacterSize(20);
	headlineText.setPosition(mainFrame.getPosition().x + 20,125);
	mainWindow.draw(headlineText);

	volumeFrame.setPosition(WINDOW_SIZE_X - 45 - volumeFrame.getSize().x,headlineText.getPosition().y+8);
	volumeBar.setPosition(volumeFrame.getPosition());
	volumeBar.setSize(sf::Vector2f(Sound_Manager::getMe().getMasterVolume()*2,10));
	mainWindow.draw(volumeFrame);
	mainWindow.draw(volumeBar);

	soundText.setString(std::to_string(Sound_Manager::getMe().getMasterVolume()) + "%");
	soundText.setPosition(volumeFrame.getPosition().x - soundText.getGlobalBounds().width - 5,volumeFrame.getPosition().y - 5);
	mainWindow.draw(soundText);

	//Musik Lautstärke
	if(selectedOption == 1) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Musik-Lautstärke");
	headlineText.setPosition(mainFrame.getPosition().x + 20,150);
	mainWindow.draw(headlineText);

	volumeFrame.setPosition(WINDOW_SIZE_X - 45 - volumeFrame.getSize().x,headlineText.getPosition().y+8);
	volumeBar.setPosition(volumeFrame.getPosition());
	volumeBar.setSize(sf::Vector2f(Sound_Manager::getMe().getMusikVolume()*2,10));
	mainWindow.draw(volumeFrame);
	mainWindow.draw(volumeBar);

	soundText.setString(std::to_string(Sound_Manager::getMe().getMusikVolume()) + "%");
	soundText.setPosition(volumeFrame.getPosition().x - soundText.getGlobalBounds().width - 5,volumeFrame.getPosition().y - 5);
	mainWindow.draw(soundText);

	//Effekt Lautstärke
	if(selectedOption == 2) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Effekt-Lautstärke");
	headlineText.setPosition(mainFrame.getPosition().x + 20,175);
	mainWindow.draw(headlineText);

	volumeFrame.setPosition(WINDOW_SIZE_X - 45 - volumeFrame.getSize().x,headlineText.getPosition().y+8);
	volumeBar.setPosition(volumeFrame.getPosition());
	volumeBar.setSize(sf::Vector2f(Sound_Manager::getMe().getEffektVolume()*2,10));
	mainWindow.draw(volumeFrame);
	mainWindow.draw(volumeBar);

	soundText.setString(std::to_string(Sound_Manager::getMe().getEffektVolume()) + "%");
	soundText.setPosition(volumeFrame.getPosition().x - soundText.getGlobalBounds().width - 5,volumeFrame.getPosition().y - 5);
	mainWindow.draw(soundText);

	//Effekt Lautstärke
	if(selectedOption == 3) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Dialogtext-Ton");
	headlineText.setPosition(mainFrame.getPosition().x + 20,200);
	mainWindow.draw(headlineText);

	if(Sound_Manager::getMe().getDoDialogSound()) videoText.setString("An");
	else videoText.setString("Aus");

	videoText.setPosition(WINDOW_SIZE_X - 45 - videoText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(videoText);

	//***Video Abschnitt***//
	headlineText.setFillColor(sf::Color::White);
	headlineText.setString("Video");//Überschrift
	headlineText.setCharacterSize(25);
	headlineText.setPosition(WINDOW_SIZE_X/2 - headlineText.getGlobalBounds().width/2,235);
	mainWindow.draw(headlineText);

	line.setFillColor(sf::Color::Red); //Trennlinien
	line.setSize(sf::Vector2f((mainFrame.getSize().x-40)/2.5,3));
	line.setPosition(mainFrame.getPosition().x+20,250);
	mainWindow.draw(line);

	line.setPosition(headlineText.getPosition().x+85,250);
	mainWindow.draw(line);

	//Vollbild
	if(selectedOption == 4) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Fenster-Modus");
	headlineText.setCharacterSize(20);
	headlineText.setPosition(mainFrame.getPosition().x + 20,275);
	mainWindow.draw(headlineText);

	switch(videoMode)
	{
	case 0:
		videoText.setString("Normal");
		break;
	case 1:
		videoText.setString("Randlos");
		break;
	case 2:
		videoText.setString("Vollbild");
		break;
	default:
		videoText.setString("Fehler");
		break;
	}

	videoText.setPosition(WINDOW_SIZE_X - 45 - videoText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(videoText);

	//Fenstergröße
	if(selectedOption == 5) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Fenstergröße");
	headlineText.setPosition(mainFrame.getPosition().x + 20,300);
	mainWindow.draw(headlineText);

	if(videoMode == 2)
		videoText.setFillColor(sf::Color(175,175,175));
		
	sf::Vector2u windowsSize = Screen_Manager::getMe().getGameWindow().getSize();
	std::string str_windowSize = std::to_string(windowsSize.x) + "x" + std::to_string(windowsSize.y);

	videoText.setString(str_windowSize);
	videoText.setPosition(WINDOW_SIZE_X - 45 - videoText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(videoText);

	//FPS-Limit
	if(selectedOption == 6) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	videoText.setFillColor(sf::Color::White);

	headlineText.setString("FPS-Limit");
	headlineText.setPosition(mainFrame.getPosition().x + 20,325);
	mainWindow.draw(headlineText);

	if(Screen_Manager::getMe().getGameFPSLimit() == 1000000) videoText.setString("Unbegrenzt!");
	else videoText.setString(std::to_string(Screen_Manager::getMe().getGameFPSLimit()));

	videoText.setPosition(WINDOW_SIZE_X - 45 - videoText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(videoText);

	//Show Particle
	if(selectedOption == 7) headlineText.setFillColor(sf::Color::Yellow);
	else headlineText.setFillColor(sf::Color::White);

	videoText.setFillColor(sf::Color::White);

	headlineText.setString("Partikel anzeigen");
	headlineText.setPosition(mainFrame.getPosition().x + 20,350);
	mainWindow.draw(headlineText);

	if(Screen_Manager::getMe().getShowParticle()) videoText.setString("Ja");
	else videoText.setString("Nein");

	videoText.setPosition(WINDOW_SIZE_X - 45 - videoText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(videoText);

	//***Steuerungs Abschnitt***//
	headlineText.setFillColor(sf::Color::White);
	headlineText.setString("Steuerung");//Überschrift
	headlineText.setCharacterSize(25);
	headlineText.setPosition(WINDOW_SIZE_X/2 - headlineText.getGlobalBounds().width/2,385);
	mainWindow.draw(headlineText);

	line.setFillColor(sf::Color(255,140,0));//Trennlinien
	line.setSize(sf::Vector2f((mainFrame.getSize().x-100)/2.5,3));
	line.setPosition(mainFrame.getPosition().x+20,400);
	mainWindow.draw(line);

	line.setPosition(headlineText.getPosition().x+135,400);
	mainWindow.draw(line);

	//Bewegen
	headlineText.setFillColor(sf::Color::White);

	headlineText.setString("Bewegen");
	headlineText.setCharacterSize(20);
	headlineText.setPosition(mainFrame.getPosition().x + 20,425);
	mainWindow.draw(headlineText);

	keyText.setString("W,A,S,D");
	keyText.setPosition(WINDOW_SIZE_X - 45 - keyText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(keyText);

	//Schwerthieb
	headlineText.setString("Schwerthieb");
	headlineText.setPosition(mainFrame.getPosition().x + 20,450);
	mainWindow.draw(headlineText);

	keyText.setString("Leertaste");
	keyText.setPosition(WINDOW_SIZE_X - 45 - keyText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(keyText);

	//Waffe benutzen
	headlineText.setString("Waffe benutzen");
	headlineText.setPosition(mainFrame.getPosition().x + 20,475);
	mainWindow.draw(headlineText);

	keyText.setString("R");
	keyText.setPosition(WINDOW_SIZE_X - 45 - keyText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(keyText);

	//Inventar öffnen
	headlineText.setString("Inventar öffnen");
	headlineText.setPosition(mainFrame.getPosition().x + 20,500);
	mainWindow.draw(headlineText);

	keyText.setString("E");
	keyText.setPosition(WINDOW_SIZE_X - 45 - keyText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(keyText);

	//Objekt Interaktion
	headlineText.setString("Objekt Interaktion");
	headlineText.setPosition(mainFrame.getPosition().x + 20,525);
	mainWindow.draw(headlineText);

	keyText.setString("Q");
	keyText.setPosition(WINDOW_SIZE_X - 45 - keyText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(keyText);

	//Sprinten
	headlineText.setString("Sprinten");
	headlineText.setPosition(mainFrame.getPosition().x + 20,550);
	mainWindow.draw(headlineText);

	keyText.setString("Shift");
	keyText.setPosition(WINDOW_SIZE_X - 45 - keyText.getGlobalBounds().width,headlineText.getPosition().y);
	mainWindow.draw(keyText);
}

void Options_Menue::checkEvents(sf::Event newEvent)
{

	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			if(selectedOption > 0) selectedOption--;
			else selectedOption = 7;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if(selectedOption < 7) selectedOption++;
			else selectedOption = 0;
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			switch(selectedOption)
			{
			case 0:
				if(Sound_Manager::getMe().getMasterVolume() > 0) 
					Sound_Manager::getMe().setMasterVolume(Sound_Manager::getMe().getMasterVolume()-1);
				break;
			case 1:
				if(Sound_Manager::getMe().getMusikVolume() > 0) 
					Sound_Manager::getMe().setMusikVolume(Sound_Manager::getMe().getMusikVolume()-1);
				break;
			case 2:
				if(Sound_Manager::getMe().getEffektVolume() > 0) 
					Sound_Manager::getMe().setEffektVolume(Sound_Manager::getMe().getEffektVolume()-1);
				break;
			case 3:
				Sound_Manager::getMe().setDoDialogSound(!Sound_Manager::getMe().getDoDialogSound());
				break;
			case 4:
				if(videoMode>0)videoMode--;
				else videoMode = 2;
				
				setVideoSettings(windowSizeX,windowSizeY,videoMode);

				break;
			case 5:
				
				if(Screen_Manager::getMe().getGameWindow().getSize().x -32 >= 160 && videoMode != 2)
				{
					if(videoMode != 2)
					{
						setVideoSettings(windowSizeX-32,windowSizeY-32,videoMode);
					}
				}
		
				break;
			case 6:
				if(Screen_Manager::getMe().getGameFPSLimit() == 1000000)
					Screen_Manager::getMe().setGameFPSLimit(500);
				else if(Screen_Manager::getMe().getGameFPSLimit() > 30)
					Screen_Manager::getMe().setGameFPSLimit(Screen_Manager::getMe().getGameFPSLimit()-1);
				break;
			case 7:
				Screen_Manager::getMe().setShowParticle(!Screen_Manager::getMe().getShowParticle());
				break;
			}
		}
		
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)  || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			switch(selectedOption)
			{
			case 0:
				if(Sound_Manager::getMe().getMasterVolume() < 100) 
					Sound_Manager::getMe().setMasterVolume(Sound_Manager::getMe().getMasterVolume()+1);
				break;
			case 1:
				if(Sound_Manager::getMe().getMusikVolume() < 100) 
					Sound_Manager::getMe().setMusikVolume(Sound_Manager::getMe().getMusikVolume()+1);
				break;
			case 2:
				if(Sound_Manager::getMe().getEffektVolume() < 100) 
					Sound_Manager::getMe().setEffektVolume(Sound_Manager::getMe().getEffektVolume()+1);
				break;
			case 3:
				Sound_Manager::getMe().setDoDialogSound(!Sound_Manager::getMe().getDoDialogSound());
				break;
			case 4:

				if(videoMode<2)videoMode++;
				else videoMode = 0;
				
				setVideoSettings(windowSizeX,windowSizeY,videoMode);
				break;
			case 5:
		
				if(videoMode != 2)
				{
					setVideoSettings(windowSizeX+32,windowSizeY+32,videoMode);
				}

				break;
		
			case 6:
				if(Screen_Manager::getMe().getGameFPSLimit() < 501)
					Screen_Manager::getMe().setGameFPSLimit(Screen_Manager::getMe().getGameFPSLimit()+1);
				break;
			case 7:
				Screen_Manager::getMe().setShowParticle(!Screen_Manager::getMe().getShowParticle());
				break;
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			File_Manager::getMe().saveSettings();

			if(screenBeforThis != nullptr)
				Screen_Manager::getMe().changeScreen(screenBeforThis->getScreenType());
			else
				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Main_Menue);

			Screen_Manager::getMe().fadeIn();
		}
	}
}

void Options_Menue::loadScreen()
{
	DOUT("")
	IOUT("Options-Screen wird gestartet")

	selectedOption = 0;

	setLoaded(true);
}

void Options_Menue::restartScreen()
{

}

void Options_Menue::closeScreen()
{
	setLoaded(false);
}

void Options_Menue::initScreen()
{
	//MainFrame
	mainFrame.setSize(sf::Vector2f(WINDOW_SIZE_X - 50,WINDOW_SIZE_Y-90));
	mainFrame.setPosition(25,75);
	mainFrame.setFillColor(sf::Color(50,50,50,200));
	mainFrame.setOutlineThickness(3);
	mainFrame.setOutlineColor(sf::Color::Blue);

	//Headline Text
	headlineText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),20);

	//Text
	soundText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),15);;
	videoText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),20);;
	keyText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),20);;

	//Volume
	volumeBar.setSize(sf::Vector2f(200,10));
	volumeBar.setFillColor(sf::Color::Green);
	
	volumeFrame.setSize(volumeBar.getSize());
	volumeFrame.setFillColor(sf::Color::Transparent);
	volumeFrame.setOutlineColor(sf::Color::White);
	volumeFrame.setOutlineThickness(1);

	selectedOption = 0;
	videoMode = 0;
	windowSizeX = WINDOW_SIZE_X;
	windowSizeY = WINDOW_SIZE_Y;
}

int Options_Menue::getVideoModeSetting()
{
	return videoMode;
}

void Options_Menue::setVideoSettings(int windowSizeX,int windowSizeY,short s_videomode)
{
	if(windowSizeX % 32 != 0)
	{
		windowSizeX = windowSizeX - (windowSizeX % 32);
	}

	if(windowSizeY % 32 != 0)
	{
		windowSizeY = windowSizeY - (windowSizeY % 32);
	}

	this->windowSizeX = windowSizeX;
	this->windowSizeY = windowSizeY;

	if(s_videomode >= 0 && s_videomode <= 2)
	{
		videoMode = s_videomode;
	}
	else
	{
		WOUT("Der Videomode <" << s_videomode << "> wird nicht unterstützt!")
		return;
	}

	switch(videoMode)
	{
	case 0:
		Screen_Manager::getMe().setWindowParameter(windowSizeX+=32,windowSizeY+=32,true,false);
		break;
	case 1:
		Screen_Manager::getMe().setWindowParameter(windowSizeX+=32,windowSizeY+=32,false,false);
		break;
	case 2:
		Screen_Manager::getMe().setWindowParameter(windowSizeX+=32,windowSizeY+=32,false,true);
		break;
	}
}

int Options_Menue::getFrameWindowSizeX()
{
	return windowSizeX;
}

int Options_Menue::getFrameWindowSizeY()
{
	return windowSizeY;
}

Options_Menue::~Options_Menue(void)
{

}
