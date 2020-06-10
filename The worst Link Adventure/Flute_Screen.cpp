#include "Screens.h"

#include "Defines.h"
#include "Graphic_Manager.h"
#include "Sound_Manager.h"
#include "Map_Manager.h"


Flute_Screen::Flute_Screen(void) : Screen(Screen::Screen_Type::Flute)
{
	initScreen();
}

void Flute_Screen::updateScreen()
{
	if(lineMoveClock.getElapsedTime().asMilliseconds() > 10)
	{
		lineMoveClock.restart();

		//Update noteline
		if(updateLineEnd > -1 && updateLines.size() > 0 && updateLineEnd < noteLines.getSize().x)
		{
			//move forward
			updateLineEnd++;
			updateLines.begin()->setSize(updateLines.begin()->getSize() + sf::Vector2f(1,0));

			//check if the played note is right
			if(updateLineEnd % 50 == 0)
			{
				short currentNote = updateLineEnd/50 - 1;

				short i_note = 0;
				for(int note = 0; note < 9; note++)
				{
					if(i_note == currentNote)
					{
						if(currentSelectedLine == currentMelody.notes[note].posLine)
						{
							currentMelody.notes[note].status = 1;
						}
						else
						{
							currentMelody.notes[note].status = 2;
						}
						break;
					}
					else
					{
						i_note++;
					}
				}

				switch(currentSelectedLine)
				{
				case 0:
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Note_D2);
					break;
				case 1:
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Note_A);
					break;
				case 2:
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Note_F);
					break;
				case 3:
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Note_D);
					break;
				}
			}
		}
		else if(updateLineEnd == noteLines.getSize().x)
		{
			//check if correct
			bool correct = true;

			for(int note = 0; note < 9; note++)
			{
				if(currentMelody.notes[note].status != 1)
				{
					correct = false;
				}
			}

			if(correct)
			{
				std::list<sf::RectangleShape>::iterator lineCursor;

				for(lineCursor = updateLines.begin(); lineCursor != updateLines.end(); lineCursor++)
				{
					lineCursor->setFillColor(sf::Color::Green);
				}

				Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);

				int currentMapID = Map_Manager::getMe().getCurrentMap().getID();

				if(currentMapID != 998 && currentMapID != 997 && currentMapID != 995)
				{
					switch(currentMelody.ID)
					{
					case 0:
						Map_Manager::getMe().changeCurrentMap("Pagotas",0);
						break;
					case 1:
						Map_Manager::getMe().changeCurrentMap("Verbotener Hain",4);
						break;
					case 2:
						Map_Manager::getMe().changeCurrentMap("Sebara Dungeon",4);
						break;
					case 4:
						Map_Manager::getMe().changeCurrentMap("Schloss Kerker",16);
						break;
					case 5:
						Map_Manager::getMe().changeCurrentMap("Schattenwelt Boss",1);
						break;
					}
				}
			}
			else
			{
				std::list<sf::RectangleShape>::iterator lineCursor;

				for(lineCursor = updateLines.begin(); lineCursor != updateLines.end(); lineCursor++)
				{
					lineCursor->setFillColor(sf::Color::Red);
				}
			}
		}
	}
}

void Flute_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);

	mainWindow.draw(frame);
	mainWindow.draw(flute);
	mainWindow.draw(spr_clef);
	mainWindow.draw(melodyName);
	mainWindow.draw(keyInfo);

	for(int i = 0; i < 160;i += 40)
	{
		noteLines.setPosition(WINDOW_SIZE_X/4 - 35,WINDOW_SIZE_Y/2 - 60 + i);
		mainWindow.draw(noteLines);
	}

	for(auto cursor : updateLines)
	{
		mainWindow.draw(cursor);
	}

	if(currentMelody.found)
	{
		int posX = 50;
		for(int note = 0; note < 9; note++)
		{
			spr_note.setPosition(WINDOW_SIZE_X/4 - 35 + posX,WINDOW_SIZE_Y/2 - 60 + (currentMelody.notes[note].posLine * 40));

			switch(currentMelody.notes[note].status)
			{
			case 0: spr_note.setColor(sf::Color(50,50,50)); break;
			case 1: spr_note.setColor(sf::Color::Green); break;
			case 2: spr_note.setColor(sf::Color::Red); break;
			}

			mainWindow.draw(spr_note);
			posX += 50;
		}
	}

	mainWindow.draw(lineTriangle);
}

void Flute_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Return) && currentMelody.found)
		{
			updateLines.clear();
			updateLineEnd = 0;

			for(int note = 0; note < 9; note++)
			{
				currentMelody.notes[note].status = 0;
			}

			createNewLine();
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
		{
			if(currentSelectedLine > 0)
			{
				changeCurrentLine(currentSelectedLine - 1);

				if(updateLines.size() > 0 && updateLineEnd < noteLines.getSize().x)
				{
					createNewLine();
				}
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
		{
			if(currentSelectedLine < 3)
			{ 
				changeCurrentLine(currentSelectedLine + 1);

				if(updateLines.size() > 0 && updateLineEnd < noteLines.getSize().x)
				{
					createNewLine();
				}
			}
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A))
		{
			changeMelody(currentMelody.ID - 1,LEFT);	
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D))
		{
			changeMelody(currentMelody.ID + 1,RIGHT);	
		}
	}
}

void Flute_Screen::loadScreen()
{
	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Die Flöte kann nur bei geladenem Spiel geöffnet werden!");

		if(screenBeforThis != nullptr)
			Screen_Manager::getMe().changeScreen(screenBeforThis->getScreenType(),true);
		else
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Main_Menue,true);

		return;
	}

	resetLines();
	changeMelody(0);
	changeCurrentLine(3);

	setLoaded(true);
}

void Flute_Screen::restartScreen()
{
	closeScreen();
	loadScreen();
}

void Flute_Screen::closeScreen()
{
	setLoaded(false);
}

void Flute_Screen::initScreen()
{
	//Frame
	frame.setSize(sf::Vector2f(WINDOW_SIZE_X+2,WINDOW_SIZE_Y/2));
	frame.setOrigin(0,frame.getGlobalBounds().height/2);
	frame.setPosition(-1,WINDOW_SIZE_Y/2);
	frame.setFillColor(sf::Color(75,75,75,220));
	frame.setOutlineThickness(3);
	frame.setOutlineColor(sf::Color(50,50,50));

	keyInfo = sf::Text("Pfeiltasten Hoch|Runter  -  Tonhöhe wechseln\nPfeiltasten Links|Rechts -   Melody wechseln\nEnter                    -          Beginnen\nEscape                   -            Zurück",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),16);
	keyInfo.setOrigin(keyInfo.getGlobalBounds().width/2,keyInfo.getGlobalBounds().height/2);
	keyInfo.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4*3.3);
	keyInfo.setOutlineColor(sf::Color::Black);
	keyInfo.setOutlineThickness(2);

	//Flute
	flute.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	flute.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,361));
	flute.setScale(8,8);
	flute.setOrigin(8,8);
	flute.setPosition(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/4);

	//Notenschlüssel
	sf::Image img_clef;

	img_clef.loadFromFile("Img/ect/clef.png");
	img_clef.createMaskFromColor(sf::Color(51,51,51));

	tex_clef.loadFromImage(img_clef);

	spr_clef.setTexture(tex_clef);
	spr_clef.setOrigin(0,spr_clef.getGlobalBounds().height/2);
	spr_clef.setPosition(20,WINDOW_SIZE_Y/2);

	//Note
	sf::Image img_note;

	img_note.loadFromFile("Img/ect/note.png");
	img_note.createMaskFromColor(sf::Color(51,51,51));

	tex_note.loadFromImage(img_note);

	spr_note.setTexture(tex_note);
	spr_note.setScale(0.25,0.25);
	spr_note.setOrigin(tex_note.getSize().x/2,tex_note.getSize().y/2);
	spr_note.setColor(sf::Color(50,50,50));

	//Noten Linien
	noteLines.setSize(sf::Vector2f(WINDOW_SIZE_X/4*3,6));

	updateLineEnd = 0;
	currentSelectedLine = 3;

	//Melody Text
	melodyName = sf::Text("???",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);
	melodyName.setOrigin(melodyName.getGlobalBounds().width / 2,melodyName.getGlobalBounds().height / 2);
	melodyName.setPosition(sf::Vector2f(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/3*2+20));
	melodyName.setFillColor(sf::Color(255,215,0));
	melodyName.setOutlineColor(sf::Color::Black);
	melodyName.setOutlineThickness(2);

	//Load Melodies
	std::string str_melodies[MAX_MELODIES];
	str_melodies[0] = "3,2,1,2,0,2,3,3,3,Melodie der Frieheit";
	str_melodies[1] = "3,2,3,1,2,3,2,0,2,Flüstern der Wälder";
	str_melodies[2] = "0,0,1,1,0,0,1,2,3,Ruhe des Labyrinthes";
	str_melodies[3] = "1,3,2,0,3,1,1,2,2,Prüfung der Götter";
	str_melodies[4] = "2,3,2,1,1,2,1,0,2,Gandalfs Ruf";
	str_melodies[5] = "0,2,3,3,2,0,1,1,3,Die vergessene Melodie";
	str_melodies[6] = "1,0,3,3,3,2,0,3,0,Echo des Lichtes";

	for(int melo = 0; melo < MAX_MELODIES; melo++)
	{
		for(int note = 0; note < 9; note++)
		{
			notePosition newNotePos;

			newNotePos.status = 0;

			int pos = str_melodies[melo].find(",");
			newNotePos.posLine = atoi(str_melodies[melo].substr(0,pos).c_str());
			str_melodies[melo] = str_melodies[melo].substr(pos+1);

			melodies[melo].notes[note] = newNotePos;
		}

		melodies[melo].name = str_melodies[melo];
		melodies[melo].found = false;
		melodies[melo].ID = melo;
	}

	changeMelody(0);

	//Triangle
	lineTriangle = sf::VertexArray(sf::PrimitiveType::Triangles,3);
	lineTriangle[0].position = sf::Vector2f(WINDOW_SIZE_X/4 - 40,WINDOW_SIZE_Y/2 - 57 + currentSelectedLine * 40);
	lineTriangle[1].position = sf::Vector2f(WINDOW_SIZE_X/4 - 50,WINDOW_SIZE_Y/2 - 47 + currentSelectedLine * 40);
	lineTriangle[2].position = sf::Vector2f(WINDOW_SIZE_X/4 - 50,WINDOW_SIZE_Y/2 - 67 + currentSelectedLine * 40);
}

void Flute_Screen::createNewLine()
{
	sf::RectangleShape newLine;

	newLine.setFillColor(sf::Color::Blue);
	newLine.setOutlineColor(sf::Color::Black);
	newLine.setOutlineThickness(2);
	newLine.setSize(sf::Vector2f(1,6));
	newLine.setPosition(WINDOW_SIZE_X/4 - 35 + updateLineEnd,WINDOW_SIZE_Y/2 - 60 + currentSelectedLine * 40);

	updateLines.push_front(newLine);
}

void Flute_Screen::changeCurrentLine(short line)
{
	currentSelectedLine = line;

	lineTriangle[0].position = sf::Vector2f(WINDOW_SIZE_X/4 - 40,WINDOW_SIZE_Y/2 - 57 + currentSelectedLine * 40);
	lineTriangle[1].position = sf::Vector2f(WINDOW_SIZE_X/4 - 50,WINDOW_SIZE_Y/2 - 47 + currentSelectedLine * 40);
	lineTriangle[2].position = sf::Vector2f(WINDOW_SIZE_X/4 - 50,WINDOW_SIZE_Y/2 - 67 + currentSelectedLine * 40);
}

void Flute_Screen::changeMelody(short melo, short dir)
{
	if(melo > MAX_MELODIES - 1) melo = 0;
	else if(melo < 0) melo = MAX_MELODIES - 1;

	currentMelody = melodies[melo];
	if(melodies[melo].found == true)
	{
		melodyName.setString(std::to_string(currentMelody.ID + 1) + " - " + melodies[melo].name);
	}
	else
	{
		melodyName.setString(std::to_string(currentMelody.ID + 1) + " - ????");
	}

	melodyName.setOrigin(melodyName.getGlobalBounds().width / 2,melodyName.getGlobalBounds().height / 2);
	melodyName.setPosition(sf::Vector2f(WINDOW_SIZE_X/2,WINDOW_SIZE_Y/3*2+20));

	resetLines();
}

void Flute_Screen::activateMelody(short melodyID)
{
	if(melodyID >= 0 && melodyID < MAX_MELODIES)
	{
		melodies[melodyID].found = true;
	}
}

void Flute_Screen::resetLines()
{
	updateLines.clear();
	updateLineEnd = 0;
}

bool Flute_Screen::loadMySaveData(std::string data)
{
	for(int i = 0; i < MAX_MELODIES; i++)
	{
		int cutPos = data.find(",");
		melodies[i].found = bool(atoi(data.substr(0,cutPos).c_str()));
		data = data.substr(cutPos+1);
	}
	
	return true;
}

std::string Flute_Screen::getMySaveData()
{
	std::string data = "";

	for(int i = 0; i < MAX_MELODIES; i++)
	{
		data += std::to_string(melodies[i].found) + ",";
	}

	return data;
}

Flute_Screen::~Flute_Screen(void)
{
}
