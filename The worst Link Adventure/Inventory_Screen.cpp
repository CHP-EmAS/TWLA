#include "Screens.h"

#include "Player_Manager.h"
#include "Screen_Manager.h"
#include "Inventory_Manager.h"
#include "File_Manager.h"
#include <iomanip>  

#include <sstream>

Inventory_Screen::Inventory_Screen(void) : Screen(Screen::Screen_Type::Inventory)
{
	initScreen();
}

void Inventory_Screen::initScreen()
{
	//MainFrame
	mainFrame.setSize(sf::Vector2f(INVENTORY_SCREEN_SIZE_X,INVENTORY_SCREEN_SIZE_Y));
	mainFrame.setPosition(INVENTORY_SCREEN_POSITION_X,INVENTORY_SCREEN_POSITION_Y);
	mainFrame.setFillColor(sf::Color(139,90,43));
	mainFrame.setOutlineThickness(3);
	mainFrame.setOutlineColor(sf::Color::Black);

	//SlotFrame
	slotFrame.setSize(sf::Vector2f(ZOOMSIZE + 4,ZOOMSIZE + 4));
	slotFrame.setFillColor(sf::Color(200,200,200,100));
	slotFrame.setOutlineThickness(1);
	slotFrame.setOutlineColor(sf::Color(100,100,100));

	//Line
	line.setFillColor(sf::Color::Black);

	//Headline Text
	headlineText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),20);

	//Info Frame
	infoFrame.setFillColor(sf::Color(75,75,75,220));
	infoFrame.setOutlineThickness(1);
	infoFrame.setOutlineColor(sf::Color(20,20,20));

	infoText = sf::Text("",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),15);

	selectedSlot = Inventory_Manager::slotID::None;

	playerDummySprite.setScale(5,5);
	playerDummySprite.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 180,mainFrame.getPosition().y+35);

	movingStep = 0;

	statsText = sf::Text("HP  : 4\nMP  : 500\nRüst: 0%\nDmg : 0%\nMs  : 10\nSs  : 10",Graphic_Manager::getMe().getFont(Graphic_Manager::Pixel),12);
	statsText.setPosition(playerDummySprite.getPosition().x + 85,playerDummySprite.getPosition().y+3);

	//Icons
	settingsIcon.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	settingsIcon.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,281));
	settingsIcon.setScale(3,3);
	settingsIcon.setPosition(mainFrame.getPosition().x + 14,mainFrame.getPosition().y + 177);

	saveIcon.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	saveIcon.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,282));
	saveIcon.setScale(3,3);
	saveIcon.setPosition(mainFrame.getPosition().x + 14,mainFrame.getPosition().y + 232);

	xIcon.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	xIcon.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,283));
	xIcon.setScale(3,3);
	xIcon.setPosition(mainFrame.getPosition().x + 14,mainFrame.getPosition().y + 287);

	questIcon.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	questIcon.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,375));
	questIcon.setScale(3,3);
	questIcon.setPosition(mainFrame.getPosition().x + 14,mainFrame.getPosition().y + 15);

	archievementIcon.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	archievementIcon.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,377));
	archievementIcon.setScale(3,3);
	archievementIcon.setPosition(mainFrame.getPosition().x + 14,mainFrame.getPosition().y + 65);

	triForce.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,379));
	triForce.setScale(6,6);
	triForce.setOrigin(8,8);

	shiftTile = 0;
	shiftMode = 0;

	selectedSlot = Inventory_Manager::None;
}

void Inventory_Screen::updateScreen()
{
	//Triforce Ani
	if(triforceAniClock.getElapsedTime().asMilliseconds() > 100)
	{
		if(shiftMode == 0)
		{
			if(shiftTile < 13) shiftTile += 1;
			else shiftMode = 1;
		}
		else
		{
			if(shiftTile > 0) shiftTile -= 1;
			else shiftMode = 0;
		}

		triforceAniClock.restart();
	}

	//Player Ani
	if(playerMoveAniClock.getElapsedTime().asMilliseconds() > 333)
	{
		if(movingStep == 0)
		{
			if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Iron_Shield)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_1);
			else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Gold_Shield)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_2);
			else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Hero_Shield)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_3);
			else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::None)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN);

			movingStep = 1;
		}
		else
		{
			if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Iron_Shield)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_2_SHIELD_1);
			else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Gold_Shield)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_2_SHIELD_2);
			else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Hero_Shield)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_2_SHIELD_3);
			else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::None)
				playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_2);

			movingStep = 0;
		}

		playerMoveAniClock.restart();
	}

	HUD hud = Player_Manager::getMe().getGameHUD();
	Player_Manager::playerStats p_stats = Player_Manager::getMe().getPlayerStats();

	std::ostringstream s_damage;
    s_damage << std::setprecision(4) << p_stats.damage;

	std::string stats = "HP  : " + std::to_string(hud.getMaxLive()); 
	stats += "\nMP  : " + std::to_string(p_stats.maxEnergy);
	stats += "\nRüst: " + std::to_string(p_stats.armor) + "%\n";
	stats += "Dmg : " + s_damage.str() + "\n";
	stats += "Lg  : " + std::to_string(p_stats.moveSpeed) + "\n";
	stats += "Sg  : " + std::to_string(p_stats.swimmSpeed);

	statsText.setString(stats);
}

void Inventory_Screen::drawScreen(sf::RenderWindow &mainWindow)
{
	Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);
	
	//Icons & Mainframe
	mainWindow.draw(mainFrame);

	mainWindow.draw(playerDummySprite);
	mainWindow.draw(settingsIcon);
	mainWindow.draw(saveIcon);
	mainWindow.draw(xIcon);
	mainWindow.draw(questIcon);
	mainWindow.draw(archievementIcon);

	//Triforce
	//1
	if(Inventory_Manager::getMe().getTriforceFragments() > 0) triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::tileset::People,379));
	else triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::tileset::People,40));
	triForce.setPosition(mainFrame.getPosition().x + 227,mainFrame.getPosition().y + 111 - shiftTile);
	mainWindow.draw(triForce);
	//2
	if(Inventory_Manager::getMe().getTriforceFragments() > 1) triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::tileset::People,379));
	else triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::tileset::People,40));
	triForce.setPosition(mainFrame.getPosition().x + 269 + shiftTile,mainFrame.getPosition().y + 189 + shiftTile);
	mainWindow.draw(triForce);
	//3
	if(Inventory_Manager::getMe().getTriforceFragments() > 2) triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::tileset::People,379));
	else triForce.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::tileset::People,40));
	triForce.setPosition(mainFrame.getPosition().x + 185 - shiftTile,mainFrame.getPosition().y + 189 + shiftTile);
	mainWindow.draw(triForce);

	//Headlines
	headlineText.setString("Inventar");
	headlineText.setCharacterSize(30);
	headlineText.setPosition(mainFrame.getPosition().x + 130,mainFrame.getPosition().y);
	mainWindow.draw(headlineText);

	//Einteilungen mit Linien
	line.setSize(sf::Vector2f(3,mainFrame.getSize().y));
	line.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 190,mainFrame.getPosition().y);
	mainWindow.draw(line);

	line.setSize(sf::Vector2f(mainFrame.getSize().x - 190,3));
	line.setPosition(mainFrame.getPosition().x,mainFrame.getPosition().y+345);
	mainWindow.draw(line);

	line.setSize(sf::Vector2f(190,3));
	line.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 190,mainFrame.getPosition().y+135);
	mainWindow.draw(line);

	line.setSize(sf::Vector2f(3,345));
	line.setPosition(mainFrame.getPosition().x + 75,mainFrame.getPosition().y);
	mainWindow.draw(line);

	//Texte
	headlineText.setString("Items");
	headlineText.setCharacterSize(20);
	headlineText.setPosition(mainFrame.getPosition().x + 95,mainFrame.getPosition().y + 370);
	mainWindow.draw(headlineText);

	headlineText.setString("Tränke");
	headlineText.setPosition(mainFrame.getPosition().x + 262,mainFrame.getPosition().y + 370);
	mainWindow.draw(headlineText);

	headlineText.setString("Schwert");
	headlineText.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 147,mainFrame.getPosition().y + mainFrame.getSize().y - 351);
	mainWindow.draw(headlineText);

	headlineText.setString("Umhang");
	headlineText.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 140,mainFrame.getPosition().y + mainFrame.getSize().y - 286);
	mainWindow.draw(headlineText);

	headlineText.setString("Schild");
	headlineText.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 139,mainFrame.getPosition().y + mainFrame.getSize().y - 221);
	mainWindow.draw(headlineText);
	
	headlineText.setString("Ring");
	headlineText.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 125,mainFrame.getPosition().y + mainFrame.getSize().y - 156);
	mainWindow.draw(headlineText);

	headlineText.setString("Schuhe");
	headlineText.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 140,mainFrame.getPosition().y + mainFrame.getSize().y - 91);
	mainWindow.draw(headlineText);

	headlineText.setString(File_Manager::getMe().getPlayerName());
	headlineText.setPosition(mainFrame.getPosition().x + mainFrame.getSize().x - 95 - headlineText.getGlobalBounds().width / 2,mainFrame.getPosition().y + 5);
	mainWindow.draw(headlineText);
	
	mainWindow.draw(statsText);

	//Slots
	for(int i = 0; i < 34; i++)
	{
		Inventory_Manager::slot *drawingSlot = &Inventory_Manager::getMe().getSlot(static_cast<Inventory_Manager::slotID>(i));

		slotFrame.setPosition(drawingSlot->itemSprite.getPosition().x - 2,drawingSlot->itemSprite.getPosition().y - 2);

		if(selectedSlot == i)
		{
			slotFrame.setOutlineColor(sf::Color::White);
			slotFrame.setOutlineThickness(2);
		}
		else if(i == Inventory_Manager::getMe().getSelectedWeapon() || i == Inventory_Manager::getMe().getSelectedSword() || i == Inventory_Manager::getMe().getSelectedArmor() || i == Inventory_Manager::getMe().getSelectedShield() || i == Inventory_Manager::getMe().getSelectedRing() || i == Inventory_Manager::getMe().getSelectedShoes())
		{
			slotFrame.setOutlineColor(sf::Color::Black);
			slotFrame.setOutlineThickness(2);
		}
		else 
		{
			slotFrame.setOutlineColor(sf::Color(100,100,100));
			slotFrame.setOutlineThickness(1);
		}

		mainWindow.draw(slotFrame);

		if(drawingSlot->unlock)
			mainWindow.draw(drawingSlot->itemSprite);
	}

	//Info Label
	if(selectedSlot != Inventory_Manager::slotID::None)
	{
		sf::Sprite *slotSprite;

		if(selectedSlot < Inventory_Manager::slotID::Quests)
		{
			slotSprite = &Inventory_Manager::getMe().getSlot(selectedSlot).itemSprite;
		}
		else
		{
			switch(selectedSlot)
			{
			case Inventory_Manager::slotID::Quests:
				slotSprite = &questIcon;
				break;
			case Inventory_Manager::slotID::Settings:
				slotSprite = &settingsIcon;
				break;
			case Inventory_Manager::slotID::Archievements:
				slotSprite = &archievementIcon;
				break;
			case Inventory_Manager::slotID::Save:
				slotSprite = &saveIcon;
				break;
			case Inventory_Manager::slotID::X:
				slotSprite = &xIcon;
				break;
			default:
				slotSprite = &xIcon;
				break;
			}
		}

		int spritePosX = slotSprite->getPosition().x;
		int spritePosY = slotSprite->getPosition().y;
		int spriteWidth = slotSprite->getGlobalBounds().width;
		int spriteHeight = slotSprite->getGlobalBounds().height;

		int textWidth = infoText.getGlobalBounds().width;
		int textHeight = infoText.getGlobalBounds().height;

		infoText.setPosition(spritePosX + spriteWidth/2 - textWidth/2,spritePosY - textHeight - 10);

		if(infoText.getPosition().x < 0)
			infoText.setPosition(10,infoText.getPosition().y);
		else if(infoText.getPosition().x + textWidth > WINDOW_SIZE_X)
			infoText.setPosition(WINDOW_SIZE_X - textWidth - 10,infoText.getPosition().y);

		int textPosX = infoText.getPosition().x;
		int textPosY = infoText.getPosition().y;

		infoFrame.setPosition(textPosX-5,textPosY);
		
		mainWindow.draw(infoFrame);
		mainWindow.draw(infoText);
	}
}

void Inventory_Screen::checkEvents(sf::Event newEvent)
{
	switch(newEvent.type)
	{
	case sf::Event::KeyPressed:
	{
		if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //W oder Up
		{
			moveSelectedSlot(UP);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //W oder Up
		{
			moveSelectedSlot(LEFT);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //W oder Up
		{
			moveSelectedSlot(DOWN);
		}
		else if(sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //W oder Up
		{
			moveSelectedSlot(RIGHT);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::E) || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Game);
		}

		if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
		{
			Inventory_Manager::getMe().useSlot(selectedSlot);
		}
	}
	}
}

void Inventory_Screen::loadScreen()
{
	DOUT("")
	IOUT("Inventory-Screen wird gestartet")

	if(!Screen_Manager::getMe().getScreen(Screen::Screen_Type::Game)->isScreenLoaded())
	{
		FOUT("Das Inventar kann nur bei geladenem Spiel geöffnet werden!");

		if(screenBeforThis != nullptr)
			Screen_Manager::getMe().changeScreen(screenBeforThis->getScreenType(),true);
		else
			Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Main_Menue,true);

		return;
	}

	//Player Dummy
	playerDummySprite.setTexture(*Player_Manager::getMe().getPlayerSprite().getTexture());

	if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Iron_Shield)
		playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_1);
	else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Gold_Shield)
		playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_2);
	else if(Inventory_Manager::getMe().getSelectedShield() == Inventory_Manager::Herosword)
		playerDummySprite.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_3);

	setLoaded(true);
}

void Inventory_Screen::restartScreen()
{
	selectedSlot = Inventory_Manager::None;

	playerDummySprite.setColor(sf::Color(255,255,255));
	settingsIcon.setColor(sf::Color(255,255,255));
	saveIcon.setColor(sf::Color(255,255,255));
	xIcon.setColor(sf::Color(255,255,255));
	questIcon.setColor(sf::Color(255,255,255));
	archievementIcon.setColor(sf::Color(255,255,255));
}

void Inventory_Screen::closeScreen()
{
	restartScreen();
	setLoaded(false);
}

void Inventory_Screen::moveSelectedSlot(int dir)
{
	if(selectedSlot == Inventory_Manager::None)
	{
		selectedSlot = Inventory_Manager::Quests;
	}
	else
	{
		int i = selectedSlot;

		switch(dir)
		{
		case UP:
			if(i >= 4 && i <= 15) i = i - 3;
			else if(i == 16) i = 38;
			else if(i == 17 || i == 18) i = 30;
			else if(i == 19 || i == 20) i = 31;
			else if(i >= 21 && i <= 25) i = i - 5;
			else if(i == 26 || i == 27) i = 33;
			else if(i == 28 || i == 29) i = i - 2;
			else if(i >= 35 && i <= 38) i = i - 1;
			break;
		case DOWN:
			if(i == 0) i = 4;
			else if(i == 1 || i == 2) i = 5;
			else if(i == 3) i = 6;
			else if(i >= 4 && i <= 12) i = i + 3;
			else if(i >= 16 && i <= 20) i = i + 5;
			else if(i == 26 || i == 27) i = i + 2;
			else if(i == 30) i = 18;
			else if(i == 31) i = 19;
			else if(i == 32) i = 20;
			else if(i == 33) i = 26;
			else if(i >= 34 && i <= 37) i = i + 1;
			else if(i == 38) i = 16;
			break;
		case LEFT:
			switch(i)
			{
			case 0:
				i = 36;
				break;
			case 4:
			case 7:
				i = 33;
				break;
			case 10:
				i = 27;
				break;
			case 13:
				i = 29;
				break;
			case 26:
				i = 20;
				break;
			case 28:
				i = 25;
				break;
			case 30:
				i = 38;
				break;
			case 1:case 2:case 3:case 5:case 6:case 8:case 9:
			case 11:case 12:case 14:case 15:case 17:case 18:
			case 19:case 20:case 22:case 23:case 24:case 25:
			case 27:case 29:case 31:case 32:case 33:
				i = i - 1;
			break;
			}
			break;
		case RIGHT:
			switch(i)
			{
			case 20:
				i = 26;
				break;
			case 25:
				i = 28;
				break;
			case 27:
				i = 10;
				break;
			case 29:
				i = 13;
				break;
			case 33:
				i = 7;
				break;
			case 34:
				i = 0;
				break;
			case 0:case 1:case 2:case 4:case 5:case 7:case 8:
			case 10:case 11:case 13:case 14:case 16:case 17:
			case 18:case 19:case 21:case 22:case 23:case 24:
			case 26:case 28:case 30:case 31:case 32:
				i = i + 1;
				break;
			case 35:
				i = 34;
			break;
			case 36:
				i = 0;
			break;
			case 37:
			case 38:
				i = 30;
			break;
			}
			break;
		}

		selectedSlot = static_cast<Inventory_Manager::slotID>(i);
	}

	settingsIcon.setColor(sf::Color(255,255,255));
	saveIcon.setColor(sf::Color(255,255,255));
	xIcon.setColor(sf::Color(255,255,255));
	questIcon.setColor(sf::Color(255,255,255));
	archievementIcon.setColor(sf::Color(255,255,255));

	if(selectedSlot == Inventory_Manager::slotID::Settings)
	{
		settingsIcon.setColor(sf::Color(210,210,210));
		infoText.setString("Einstellungen");
	}
	else if(selectedSlot == Inventory_Manager::slotID::Save)
	{
		saveIcon.setColor(sf::Color(210,210,210));
		infoText.setString("Speichern");
	}
	else if(selectedSlot == Inventory_Manager::slotID::X)
	{
		xIcon.setColor(sf::Color(210,210,210));
		infoText.setString("Zurück ins Hauptmenü");
	}
	else if(selectedSlot == Inventory_Manager::slotID::Quests)
	{
		questIcon.setColor(sf::Color(210,210,210));
		infoText.setString("Quests");
	}
	else if(selectedSlot == Inventory_Manager::slotID::Archievements)
	{
		archievementIcon.setColor(sf::Color(210,210,210));
		infoText.setString("Statistiken");
	}
	else if(selectedSlot >= 0 && selectedSlot <= 33)
	{
		Inventory_Manager::slot* slot = &Inventory_Manager::getMe().getSlot(selectedSlot);

		if(slot->unlock)
		{
			infoText.setString(Item_List::getMe().getItemName(slot->itemNumber));

			if(selectedSlot >= 0 && selectedSlot <= 25)
			{
				std::string amountInfo = "";
				if(slot->amount > 1 || selectedSlot == Inventory_Manager::Bow)
				{
					std::string zusatz = "";

					switch(selectedSlot)
					{
					case Inventory_Manager::Bow:
						zusatz = " Pfeile";
						break;
					case Inventory_Manager::Bomb:
					case Inventory_Manager::Waterbomb:
						zusatz = " Stück";
						break;
					}

					amountInfo = " (" + std::to_string(slot->amount) + zusatz + ")";
				}

				if(selectedSlot != Inventory_Manager::getMe().getSelectedSword() && 
					selectedSlot != Inventory_Manager::getMe().getSelectedArmor() && 
					selectedSlot != Inventory_Manager::getMe().getSelectedShield() && 
					selectedSlot != Inventory_Manager::getMe().getSelectedRing() && 
					selectedSlot != Inventory_Manager::getMe().getSelectedShoes() && 
					selectedSlot != Inventory_Manager::getMe().getSelectedWeapon())
				{
					infoText.setString(infoText.getString() + " auswählen" + amountInfo);
				}
				else
				{
					infoText.setString(infoText.getString() + " ablegen" + amountInfo);
				}
			}
			else if(selectedSlot >= 26 && selectedSlot <= 29)
			{
				if(slot->itemNumber == 29 || slot->itemNumber == 30 || slot->itemNumber == 53)
				{
					infoText.setString(infoText.getString() + " trinken");
				}
			}

		}
		else
		{
			infoText.setString("leer");
		}
	}
	
	infoFrame.setSize(sf::Vector2f(infoText.getGlobalBounds().width + 10,infoText.getGlobalBounds().height + 7));
}

Inventory_Screen::~Inventory_Screen(void)
{

}
