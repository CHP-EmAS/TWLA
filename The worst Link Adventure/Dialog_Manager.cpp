#include "Dialog_Manager.h"

#include <fstream>

#include "Message_Manager.h"
#include "Quest_Manager.h"
#include "Command_Manager.h"
#include "File_Manager.h"

Dialog_Manager::Dialog_Manager(void)
{

	std::ifstream ifs;

	ifs.open("lists/Dialogs/Dialog_list.zData", std::ios::in);

	//Vorhandene Dialoge in Liste hinzufügen
	if (ifs.good())
	{
		char c_dummy[500];

		while(true)
		{
			ifs.getline(c_dummy,200,' ');
			int dialogID = atoi(c_dummy);

			if(std::strcmp(c_dummy,"") == 0)break;

			ifs.getline(c_dummy,200,'\n');
			std::string dialogPath = c_dummy;

			std::map<int,std::string>::iterator it;
			it = dialog_Paths.find(dialogID);

			if(it != dialog_Paths.end())
			{
				FOUT("Dialog-ID: " << dialogID << " ist doppelt definiert!");
				WOUT("Dialog " << dialogID << " wird überschrieben!");
			}

			dialog_Paths[dialogID] = dialogPath;
		}

	}
	else
	{
		CRITOUT("Dialogliste konnte nicht geöffnet werden!");
	}

	IOUT("Es wurden " << dialog_Paths.size() << " Dialoge gefunden!");

	currentDialog = nullptr;

	NEWOUT("Neuer Dialog_Manager wurde erstellt! (" << this << ")")
}

void Dialog_Manager::doDialog(unsigned short dialogID)//Dialog ausführen
{
	std::map<int,std::string>::iterator it;
	it = dialog_Paths.find(dialogID);

	if(it == dialog_Paths.end())
	{
		FOUT("Dialog: " << dialogID << " ist nicht in der Dialog-Liste vorhanden!");
		return;
	}

	if(currentDialog == nullptr)//Dialog laden
	{
		if(!loadDialog(dialogID))
		{
			FOUT("Dialog: " << dialogID << " konnte nicht geladen werden!");
			return;
		}
		else
		{
			SOUT("Dialog: " << dialogID << " wird ausgeführt!");
		}
	}
	else if(currentDialog->ID != dialogID)//Dialog laden
	{
		if(!loadDialog(dialogID))
		{
			FOUT("Dialog: " << dialogID << " konnte nicht geladen werden!");
			return;
		}
		else
		{
			SOUT("Dialog: " << dialogID << " wird ausgeführt!");
		}
	}

	doDialogSection(getRightDialogSection());//Richtigen Abschnitt ausführen
}

bool Dialog_Manager::loadDialog(unsigned short dialogID)
{
	//Suche Dialog ID
	std::map<int,std::string>::iterator it;
	it = dialog_Paths.find(dialogID);

	if(it == dialog_Paths.end())
	{
		FOUT("Dialog: " << dialogID << " ist nicht in der Dialog-Liste vorhanden!");
		return false;
	}

	//Öffne Dialog Datei
	std::string path = "lists/Dialogs/";
	path += it->second;

	//Alle Befehle in Liste konvertieren
	std::list<fullCommand> fullCommandsList = File_Manager::getMe().convertFileToCommandlist(path);
	
	dialogSection *newSection = nullptr;
	dialogQuestion *newQuestion = nullptr;
	std::string currentAnswer = "";

	//Vorherigen Dialog löschen
	if(currentDialog != nullptr) 
		deleteCurrentDialog();

	currentDialog = new dialog;

	for(auto cursor : fullCommandsList)
	{
		if(cursor.command == "ID") //ID setzen
		{
			currentDialog->ID = atoi(cursor.content.c_str());
		}
		else if(cursor.command == "next section") //Neuer Abschnitt hinzufügen
		{
			if(newQuestion != nullptr)
			{
				FOUT("Skript-Error! Es kann kein neuer Abschnitt bei bestehender Frage erstellt werden! Datei: " << path)
				return false;
			}

			if(newSection != nullptr)
				currentDialog->sections.push_back(newSection); //Vorherigen Abschnitt in die Abschnittliste verschieben

			newSection = new dialogSection; //Neuen Abschnitt erstellen
		}
		else if(cursor.command == "end sections")
		{
			if(newSection != nullptr)
				currentDialog->sections.push_back(newSection); //Abschnitte schließen
			else
			{
				WOUT("Skript-Warnung! Abschnitte werden beendet obwohl kein Abschnit definiert ist! Datei: " << path)
			}

			newSection = nullptr;
		}
		else if(cursor.command == "requirement") //Benötigter Quest-Befehl
		{
			if(newSection != nullptr && newQuestion != nullptr)
			{
				FOUT("Skript-Error! Abschnitt und Frage sind gleichzeitig definiert! Datei: " << path)
				return false;
			}

			if(newSection != nullptr)
				newSection->requirements.push_back(cursor.content); //Befehl dem offenem Abschnitt zuordnen
			else
			{
				FOUT("Skript-Error! Anforderung konnte keinem Abschnitt zugeordnet werden! Datei: " << path)
				return false;
			}
		}
		else if(cursor.command == "text" || cursor.command == "do question" || cursor.command == "command" || cursor.command == "quest command" || cursor.command == "jumper" || cursor.command == "jump to" || cursor.command == "speaker name" || cursor.command == "text color")
		{ //Sonstige Befehle dem Abschnitt zuordnen
			if(newSection != nullptr && newQuestion != nullptr)
			{
				FOUT("Skript-Error! Abschnitt und Frage sind gleichzeitig definiert! Datei: " << path)
				return false;
			}

			if(newSection != nullptr)
				newSection->commands.push_back(cursor); //Befehl dem offenem Abschnitt zuordnen
			else if(newQuestion != nullptr)
			{
				if(currentAnswer != "")
					newQuestion->answerCommands.find(currentAnswer)->second.push_back(cursor); //Befehl der offenen Antwort in der aktuellen Frage zuordnen
				else
				{
					FOUT("Skript-Error! Befehl <" << cursor.command << "> konnte keiner Antwort zugeordnet werden! Datei: " << path)
					return false;
				}
			}
			else
			{
				FOUT("Skript-Error! Befehl konnte keiner Instanz zugeordnet werden! Datei: " << path)
				return false;
			}
		}
		else if(cursor.command == "question") //Neue Frage hinzufügen
		{
			if(newSection != nullptr)
			{
				FOUT("Skript-Error! Es kann keine neue Frage bei bestehendem Abschnitt erstellt werden! Datei: " << path)
				return false;
			}

			if(newQuestion != nullptr)
				currentDialog->sections.push_back(newSection); //Vorherige Frage in die Fragenliste verschieben 

			newQuestion = new dialogQuestion;

			newQuestion->questionName = cursor.content;
		}
		else if(cursor.command == "question text") //Fragentext der Frage zuordnen
		{
			if(newSection != nullptr && newQuestion != nullptr)
			{
				FOUT("Skript-Error! Abschnitt und Frage sind gleichzeitig definiert! Datei: " << path)
				return false;
			}

			if(newQuestion != nullptr)
				newQuestion->questionText = cursor.content; //Fragentext der Frage hinzufügen
			else
			{
				FOUT("Skript-Error! Fragen-Text konnte keiner Frage zugeordnet werden! Datei: " << path)
				return false;
			}
		}
		else if(cursor.command == "answer")
		{
			if(newSection != nullptr && newQuestion != nullptr) //Antwort der Frage zuordnen
			{
				FOUT("Skript-Error! Abschnitt und Frage sind gleichzeitig definiert! Datei: " << path)
				return false;
			}

			if(newQuestion != nullptr)
			{
				newQuestion->answerCommands[cursor.content]; //Antwort der Frage hinzufügen

				currentAnswer = cursor.content; //Aktuelle Antwort festlegen
			}
			else
			{
				FOUT("Skript-Error! Antwort konnte keiner Frage zugeordnet werden! Datei: " << path);
				return false;
			}
		}
		else if(cursor.command == "end question") //Frage abschließen
		{
			if(newQuestion != nullptr)
				currentDialog->questions.push_back(newQuestion); //Frage der Fragen-Liste hinzufügen
			else
			{
				WOUT("Skript-Warnung! Frage wird beendet obwohl keine Frage definiert ist! Datei: " << path)
			}

			newQuestion = nullptr;
			currentAnswer = "";
		}
		else
		{
			WOUT("Skript-Warnung! Befehl <" << cursor.command << "> ist unbekannt! Datei: " << path);
		}
	}

	return true;
}
	
void Dialog_Manager::deleteCurrentDialog()
{
	if(currentDialog == nullptr) return;

	for(std::list<dialogQuestion*>::iterator cursor = currentDialog->questions.begin(); cursor != currentDialog->questions.end(); cursor++)
	{
		delete (*cursor);
	}

	for(std::list<dialogSection*>::iterator cursor = currentDialog->sections.begin(); cursor != currentDialog->sections.end(); cursor++)
	{
		delete (*cursor);
	}

	currentDialog->questions.clear();
	currentDialog->sections.clear();

	delete currentDialog;
	currentDialog = nullptr;
}

short Dialog_Manager::getRightDialogSection() //Korrekten Dialog-Abschnitt herrausfinden
{
	if(currentDialog->sections.size() <= 0) return -1;

	int sectionNumber = currentDialog->sections.size();

	std::list<dialogSection*>::iterator dialogCursor;
	for(dialogCursor = currentDialog->sections.end(); dialogCursor != currentDialog->sections.begin(); NULL) //Dialog-Abschnitte von unten nach oben durchgehen
	{
		dialogCursor--;
		sectionNumber--;

		bool isChecked = true;

		for(auto requirement : (*dialogCursor)->requirements)
		{
			if(!Quest_Manager::getMe().isQuestCommandDone(requirement)) //Überprüfen der Quest-Befehle
			{
				isChecked = false;
				break;
			}
		}

		if(isChecked) return sectionNumber; //Wenn alle Quest-Befehle erledigt, Dialog-Abchnittsnummer zurückgeben
	}

	return -1; //Wenn kein Abschnitt gefunden -1 zurückgeben
}

void Dialog_Manager::doDialogSection(short dialogSectionNumber) //Dialogs-Abschnitt ausführen
{
	if(dialogSectionNumber >= 0 && dialogSectionNumber < currentDialog->sections.size())
	{
		short cursorNumber = 0;

		for(auto section : currentDialog->sections) //Abschnitt durchgehen
		{
			if(cursorNumber == dialogSectionNumber)//Richtigen Abschnitt finden
			{
				doDialogCommands(section->commands);//Befehle in diesem Abschnitt ausführen
				return;
			}

			cursorNumber++;
		}

		return;
	}
	else
	{
		return;
	}
}

void Dialog_Manager::doDialogCommands(std::list<fullCommand> commandList) //Befehle ausführen
{
	std::string speakerName = "";
	sf::Color textColor = sf::Color::White;

	for(auto commandCursor : commandList)
	{
		if(commandCursor.command == "text") //Text ausgeben
		{
			Message_Manager::getMe().showMsg(commandCursor.content, textColor,speakerName);
		}
		else if(commandCursor.command == "quest command") //Quest-Befehl ausführen
		{
			Quest_Manager::getMe().doQuestCommand(commandCursor.content);
		}
		else if(commandCursor.command == "do question") //Frage ausführen
		{
			for(auto question : currentDialog->questions)
			{
				if(question->questionName == commandCursor.content)
				{
					std::string answer1 = "",answer2 = "",answer3 = "",answer4 = "";

					for(auto answer : question->answerCommands) //Antworten erstellen
					{
						if(answer1 == "")
							answer1 = answer.first;
						else if(answer2 == "")
							answer2 = answer.first;
						else if(answer3 == "")
							answer3 = answer.first;
						else if(answer4 == "")
							answer4 = answer.first;
						else
						{
							WOUT("Zu viele Antworten in Frage <" << question->questionName << ">! max. 4");
						}
					}

					if(answer1 == "" || answer2 == "") 
					{
						FOUT("Zu wenig Antworten in Frage <" << question->questionName << ">! min. 2");
						break;
					}
					
					//Frage anzeigen
					std::string userAnswer = Message_Manager::getMe().showQuestion(question->questionText,textColor,answer1,answer2,answer3,answer4,speakerName);
					
					//Befehle der ausgewählten Antwort ausführen
					doDialogCommands(question->answerCommands[userAnswer]);
				}
			}
		}
		else if(commandCursor.command == "command")//Befehl ausführen
		{
			Command_Manager::getMe().doCommand(commandCursor.content);
		}
		else if(commandCursor.command == "speaker name") //Gesprächsname setzen
		{
			speakerName = commandCursor.content;
		}
		else if(commandCursor.command == "text color") //Textfarbe setzen
		{
			int r,g,b,a;
		
			int pos = commandCursor.content.find(",");
			r = atoi(commandCursor.content.substr(0,pos).c_str());
			commandCursor.content = commandCursor.content.substr(pos+1);

			pos = commandCursor.content.find(",");
			g = atoi(commandCursor.content.substr(0,pos).c_str());
			commandCursor.content = commandCursor.content.substr(pos+1);

			pos = commandCursor.content.find(",");
			b = atoi(commandCursor.content.substr(0,pos).c_str());
			commandCursor.content = commandCursor.content.substr(pos+1);
			
			a = atoi(commandCursor.content.c_str());

			textColor = sf::Color(r,g,b,a);
		}
	}
}

Dialog_Manager::~Dialog_Manager(void)
{
	deleteCurrentDialog();

	DELOUT("Dialog_Manager wurde geloescht!\t(" << this << ")")
}
