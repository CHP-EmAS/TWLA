#include "Player_Manager.h"

#include <stdlib.h> 
#include <time.h>       
#include <thread>
#include <conio.h>

#include "File_Manager.h"
#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Message_Manager.h"
#include "Screen_Manager.h"
#include "Item_List.h"
#include "Map.h"
#include "Weapon_Manager.h"
#include "Weapons.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "Sound_Manager.h"
#include "Collision_System.h"
#include "Dialog_Manager.h"
#include "InteractivObject.h"
#include "NPC.h"
#include "Enemy.h"
#include "Quest_Manager.h"
#include "Command_Manager.h"

Player_Manager::Player_Manager() : 
	moveAniTime(150),    //standart 150ms
	moveSpeedTime(5),    //standart 4ms
	swimmAniTime(750),   //standart 200ms
	noDmgTime(1500)
{
	NEWOUT("Neuer Player_Manager wurde erstellet! (" << this << ")");

	//for move Animation
	dir = 2;
	moving_Mode = 1; //0 = move ,1 = swimming,2 = carrying
	dirAnimation = 0; //0 = first step, 1 = secound step, 2 = fight step
	
	lastDir = dir;
	lastMovingMode = moving_Mode;
	lastDirAnimation = dirAnimation;
	lastShield = Inventory_Manager::None;

	//fight Animation
	swordKeyPressed = false;
	weaponKeyPressed = false;

	currentAnimation = -1;

	hasBoomerang = true;
	hasGrapplibghook = true;
	isSwordAniFinished = true;

	//Sleep
	sleepSize = 0;
	sleepText = sf::Text("z",Graphic_Manager::getMe().getFont(Graphic_Manager::Arial),12);
	sleepText.setFillColor(sf::Color(0, 0, 100,200));
	sleepText.setString("");
	sleepText.setStyle(sf::Text::Bold);
	sleepTimeSec = 120; //standart 120sec

	//damage
	dmgColor = sf::Color(255,50,50);

	barrier.setFillColor(sf::Color(0,255,255,50));
	barrier.setOutlineColor(sf::Color(0,255,255,255));
	barrier.setOutlineThickness(2);
	barrier.setRadius(30);
	barrier.setOrigin(barrier.getRadius(),barrier.getRadius());
	showBarrier = false;

	//Link start statements
	posX = 0; 
	posY = 0;
	lastTpPos.x = 0;
	lastTpPos.y = 0;
	
	lastUpdatePositionX = -1000;
	lastUpdatePositionY = -1000;

	movespeed = 1; //standart 1

	canMove = true;
	godmode = false;

	//loading Img for Link Tiles
	tex_Player = Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People);

	spr_Player.setTexture(tex_Player);
	
	if(lastShield == Inventory_Manager::Iron_Shield)
		spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_1);
	else if(lastShield == Inventory_Manager::Gold_Shield)
		spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_2);
	else if(lastShield == Inventory_Manager::Hero_Shield)
		spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_3);
	else if(lastShield == Inventory_Manager::None)
		spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN);

	spr_Player.setScale(SCALE,SCALE);
	spr_Player.setPosition(posX,posY);

	playerCollisionbox.height = ZOOMSIZE;
	playerCollisionbox.width = ZOOMSIZE;
	playerCollisionbox.mapPosX = getMapPosX();
	playerCollisionbox.mapPosY = getMapPosY();
	playerCollisionbox.type = Player_Collisionbox;
	playerCollisionbox.boundNPC = nullptr;
	playerCollisionbox.boundEnemy = nullptr;
	playerCollisionbox.boundObj = nullptr;

	game_HUD = new HUD;
	gameStats = new InGame_Stats;

	//energy
	ms_energyEmptyTime = 1;
	ms_energyFillTime = 5;

	//ending
	spr_heart.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	spr_heart.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,257));
	spr_heart.setScale(2,2);
	spr_heart.setOrigin(8,8);
	heartCounter = 0;
}

bool Player_Manager::loadPlayerSaveData(std::string data)
{
	int cutPos;

	int mapID = 0;
	int spawnX = 0;
	int spawnY = 0;

	for(int i = 0; i < 4;i++)
	{
		cutPos = data.find(",");
		std::string playerData = data.substr(0,cutPos);
		data = data.substr(cutPos+1);
		
		switch(i)
		{
		case 0:
			mapID = atoi(playerData.c_str());
			break;
		case 1:
			spawnX = atoi(playerData.c_str());
			break;
		case 2:
			spawnY = atoi(playerData.c_str());
			break;
		case 3:
			dir = atoi(playerData.c_str());
			break;
		}
	}

	Map_Manager::getMe().changeCurrentMap(mapID,0,false);
	tp(spawnX,spawnY);

	//File_Manager::getMe().save_Savegame();

	return true;
}

std::string Player_Manager::getPlayerSaveData()
{
	std::string fullData = "";

	fullData += std::to_string(Map_Manager::getMe().getCurrentMap().getID()) + ",";
	fullData += std::to_string(getMapPosX()) + "," + std::to_string(getMapPosY()) + ",";
	fullData += std::to_string(dir) + ",";

	return fullData;
}

//Updating Links Movement and Settings and Animations
void Player_Manager::update()
{
	if(canMove)
	{
		checkMoveKeys(); //WASD pressend? and set Link positions
		ckeckFightKeys(); //SPACE fight!
		checkPlayKeys();
	}
	
	if(Screen_Manager::getMe().debugMode())
	{
		checkDebugKeys(); //any other Keys M
	}

	checkPlayerMoveBox();

	if(currentAnimation != -1)
	{
		updateAnimations();
	}
	else
	{
		checkEnvironment(); //checking the Environment
		updatePlayerTile(); //Setting right Tile for Link
	}

	if(sleepClock.getElapsedTime().asSeconds() >= sleepTimeSec)
	{
		if(currentAnimation == -1)
		{
			doSleepAni();
		}
	}

	gameStats->update();

	
	spr_Player.setPosition(posX,posY);
	playerCollisionbox.mapPosX = getMapPosX();
	playerCollisionbox.mapPosY = getMapPosY();
}

void Player_Manager::updatePlayerStats()
{
	int stat_MaxEnergy = 500;
	int stat_Armor = 0;
	float stat_Damage = 0;
	int stat_MoveSpeed = 0;
	int stat_SwimmSpeed = 0;

	Inventory_Manager::slotID selc_armor = Inventory_Manager::getMe().getSelectedArmor();
	Inventory_Manager::slotID selc_ring = Inventory_Manager::getMe().getSelectedRing();
	Inventory_Manager::slotID selc_shield = Inventory_Manager::getMe().getSelectedShield();
	Inventory_Manager::slotID selc_shoes = Inventory_Manager::getMe().getSelectedShoes();
	Inventory_Manager::slotID selc_sword = Inventory_Manager::getMe().getSelectedSword();

	switch(selc_sword)
	{
	case Inventory_Manager::Woodsword:
		stat_Damage = 0.25;
		break;
	case Inventory_Manager::Ironsword:
		stat_Damage = 0.5;
		break;
	case Inventory_Manager::Goldsword:
		stat_Damage = 1;
		break;
	case Inventory_Manager::Herosword:
		stat_Damage = 1.5;
		break;
	}

	switch(selc_shoes)
	{
	case Inventory_Manager::Standart_Shoe:
		stat_MoveSpeed = 20;
		stat_SwimmSpeed = 20;
		break;
	case Inventory_Manager::Hiking_Shoe:
		stat_MoveSpeed = 30;
		stat_SwimmSpeed = 10;
		break;
	case Inventory_Manager::Swimm_Shoe:
		stat_MoveSpeed = 10;
		stat_SwimmSpeed = 40;
		break;
	}

	switch(selc_armor)
	{
	case Inventory_Manager::Standart_Armor:
		
		break;
	case Inventory_Manager::Blue_Armor:
		stat_Armor += 3;
		stat_MaxEnergy += 250;
		stat_SwimmSpeed += 20;
		break;
	case Inventory_Manager::Red_Armor:
		stat_Damage += 0.25;
		stat_Armor += 10;
		break;
	}

	switch(selc_ring)
	{
	case Inventory_Manager::Blue_Ring:
		stat_MoveSpeed += 20;
		stat_MaxEnergy += 400;
		break;
	case Inventory_Manager::Red_Ring:
		stat_Damage += 0.25;
		stat_Armor += 5;
		break;
	case Inventory_Manager::Hero_Ring:
		stat_Damage += 0.5;
		stat_MaxEnergy += 150;
		break;
	}

	switch(selc_shield)
	{
	case Inventory_Manager::Iron_Shield:
		stat_Armor += 10;
		break;
	case Inventory_Manager::Gold_Shield:
		stat_Armor += 20;
		break;
	case Inventory_Manager::Hero_Shield:
		stat_Armor += 30;
		stat_Damage += 0.25;
		break;
	}

	if(selc_sword == Inventory_Manager::None) stat_Damage = 0;

	if(selc_shoes == Inventory_Manager::None) 
	{
		stat_MoveSpeed = 10;
		stat_SwimmSpeed = 10;
	}

	stats.maxEnergy = stat_MaxEnergy;

	stats.damage = stat_Damage;
	stats.armor = stat_Armor;
	
	stats.moveSpeed = stat_MoveSpeed;
	stats.swimmSpeed = stat_SwimmSpeed;

	game_HUD->setMaxEnergy(stats.maxEnergy);
}

void Player_Manager::updatePlayerTile()
{
	//Prüfen ob ein Update nötig ist
	if(lastDir == dir && lastMovingMode == moving_Mode && lastDirAnimation == dirAnimation && lastShield == Inventory_Manager::getMe().getSelectedShield()) return;
	
	//Schlaftimer resetten
	sleepClock.restart();

	//auf neue moving-states setzen
	lastDir = dir;
	lastMovingMode = moving_Mode;
	lastDirAnimation = dirAnimation;
	lastShield = Inventory_Manager::getMe().getSelectedShield();

	switch(dir)
	{
	case 0: //Left
		if(moving_Mode == 0)
		{
			if(dirAnimation == 0)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT);
			}
			else if(dirAnimation == 1)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_2_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_2_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_2_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_LEFT_2);
			}
		}
		else if(moving_Mode == 1)
		{
			if(dirAnimation == 0) spr_Player.setTextureRect(LINK_SWIMM_ANI_LEFT);
			else if(dirAnimation == 1) spr_Player.setTextureRect(LINK_SWIMM_ANI_LEFT_2);
		}

		break;
	case 1: //Right
		if(moving_Mode == 0)
		{
			if(dirAnimation == 0)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT);
			}
			else if(dirAnimation == 1)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_2_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_2_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_2_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT_2);
			}
		}
		else if(moving_Mode == 1)
		{
			if(dirAnimation == 0) spr_Player.setTextureRect(LINK_SWIMM_ANI_RIGHT);
			else if(dirAnimation == 1) spr_Player.setTextureRect(LINK_SWIMM_ANI_RIGHT_2);
		}

		break;
	case 2://Down
		if(moving_Mode == 0)
		{
			if(dirAnimation == 0)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN);
			}
			else if(dirAnimation == 1)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_2_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_2_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_2_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_2);
			}
		}
		else if(moving_Mode == 1)
		{
			if(dirAnimation == 0) spr_Player.setTextureRect(LINK_SWIMM_ANI_DOWN);
			else if(dirAnimation == 1) spr_Player.setTextureRect(LINK_SWIMM_ANI_DOWN_2);
		}
		break;
	case 3://Up
		if(moving_Mode == 0)
		{
			if(dirAnimation == 0)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP);
			}
			else if(dirAnimation == 1)
			{
				if(lastShield == Inventory_Manager::Iron_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_2_SHIELD_1);
				else if(lastShield == Inventory_Manager::Gold_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_2_SHIELD_2);
				else if(lastShield == Inventory_Manager::Hero_Shield)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_2_SHIELD_3);
				else if(lastShield == Inventory_Manager::None)
					spr_Player.setTextureRect(LINK_MOVE_ANI_UP_2);
			}
		}
		else if(moving_Mode == 1)
		{
			if(dirAnimation == 0) spr_Player.setTextureRect(LINK_SWIMM_ANI_UP);
			else if(dirAnimation == 1) spr_Player.setTextureRect(LINK_SWIMM_ANI_UP_2);
		}
		break;
	}
}


void Player_Manager::updatePlayerHitBoxes()
{
	Collision_System::getMe().get_colliding_Collisionboxes(nearPlayerCollisionBoxes,posX+TILESIZE-(LINK_UPDATE_RADIUS + ZOOMSIZE),posY+TILESIZE-(LINK_UPDATE_RADIUS + ZOOMSIZE),(LINK_UPDATE_RADIUS + ZOOMSIZE)*2,(LINK_UPDATE_RADIUS + ZOOMSIZE)*2);
	lastUpdatePositionX = getMapPosX() + TILESIZE;
	lastUpdatePositionY = getMapPosY() + TILESIZE;
}

void Player_Manager::clearPlayerHitboxes()
{
	nearPlayerCollisionBoxes.clear();
}


void Player_Manager::updateAnimations()
{
	switch(currentAnimation)
	{
	case 0:
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;
		}
		break;
	case 1:
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;
			drawAniSprite = false;
		}
		break;
	case 2:
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			switch(aniMode)
			{
			case 0:
				switch(dir)
				{
				case LEFT:
					spr_Player.setTextureRect(LINK_HAMMER_LEFT_2);
					aniSprite.setTextureRect(sf::IntRect(382,224,-14,14));
					aniSprite.setPosition(posX-22,posY-13);
					break;
				case RIGHT:
					spr_Player.setTextureRect(LINK_HAMMER_RIGHT_2);
					aniSprite.setTextureRect(sf::IntRect(368,224,14,14));
					aniSprite.setPosition(posX+26,posY-13);
					break;
				case DOWN:
					spr_Player.setTextureRect(LINK_HAMMER_DOWN_2);
					aniSprite.setTextureRect(sf::IntRect(373,209,10,11));
					aniSprite.setPosition(posX+14,posY+6);
					break;
				case UP:
					spr_Player.setTextureRect(LINK_HAMMER_UP_2);
					aniSprite.setTextureRect(sf::IntRect(369,241,10,14));
					aniSprite.setPosition(posX-2,posY-22);
					break;
				}

				aniMode = 1;
				current_msAniTime = 30;
				aniClock.restart();
				break;
			case 1:
				switch(dir)
				{
				case LEFT:
					aniSprite.setTextureRect(sf::IntRect(350,84,-14,12));
					aniSprite.setPosition(posX-28,posY+10);
					break;
				case RIGHT:
					aniSprite.setTextureRect(sf::IntRect(336,84,14,12));
					aniSprite.setPosition(posX+32,posY+10);
					break;
				case DOWN:
					aniSprite.setTextureRect(sf::IntRect(374,80,10,12));
					aniSprite.setPosition(posX+14,posY+32);
					break;
				case UP:
					aniSprite.setTextureRect(sf::IntRect(352,69,11,11));
					aniSprite.setPosition(posX-2,posY-22);
					break;
				}

				aniMode = 2;
				current_msAniTime = 500;
				aniClock.restart();

				Sound_Manager::getMe().playEffectSound(Sound_Manager::Hammer);
				Weapon_Manager::getMe().doDamageOnPos(sf::Vector2i(aniSprite.getPosition()),sf::Vector2i(aniSprite.getGlobalBounds().width,aniSprite.getGlobalBounds().height),1,Hammer_Interact);
				break;
			case 2:
				canMove = true;
				currentAnimation = -1;
				spr_Player.setTextureRect(lastLinkAniRect);
				dirAnimation = 0;
				drawAniSprite = false;
				break;
			}
		}
		break;
	case 3:
		if(hasGrapplibghook)
		{
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;

			updatePlayerHitBoxes();
			tryToTeleportOutOfHitBox();
		}
		break;
	case 5:
		if(isSwordAniFinished) 
		{
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;
			updatePlayerHitBoxes();
		}
		break;
	case 6:
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;
			drawAniSprite = false;
		}
		break;
	case 7:
	{
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			if(sleepSize < 3)
			{
				std::string temp = sleepText.getString();
				temp += "z";
				sleepText.setString(temp);
				sleepSize++;
			}
			else
			{
				sleepText.setString("");
				sleepSize = 0;
			}

			aniClock.restart();
		}

		bool found = false;

		for(int i = 0; i < 100;i++)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key(i)))
			{
				found = true;
				break;
			}
		}

		if(found)
		{
			sleepClock.restart();
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;
			drawAniSprite = false;
		}

		break;
	}
	case 8:
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			switch(aniMode)
			{
			case 0:
				Screen_Manager::getMe().fadeIn();
				current_msAniTime = 500;
				aniMode = 1;
				aniClock.restart();
				break;
			case 1:
				Message_Manager::getMe().showMsg("Du hast es geschafft! Du hast unser Königreich gerettet.",sf::Color::White,"Königin Michelle");
				Message_Manager::getMe().showMsg("Ich danke dir! Du bist mein Held!",sf::Color::White,"Königin Michelle");
				aniMode = 2;
				aniClock.restart();
				break;
			case 2:
				spr_Player.setTextureRect(LINK_MOVE_ANI_DOWN_2);

				aniSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,427));
				aniSprite.setPosition(posX+26,posY);
				 
				Screen_Manager::getMe().doEffect(Screen_Manager::Effect::Heart);

				aniClock.restart();

				current_msAniTime = 300;
				aniMode = 3;
				break;
			case 3:
				if(heartCounter < 20)
				{
					aniClock.restart();
					spr_heart.setPosition(posX+32+rand()%10-9,posY+rand()%3-2);
					Particle_Manager::getMe().addParticle(spr_heart,500,20,0,rand()%2-2,true,false);
					heartCounter++;
				}
				else
				{
					heartCounter = 0;
					aniMode = 4;
				}
				break;
			case 4:
				Screen_Manager::getMe().fadeOut();
				
				Command_Manager::getMe().doCommand("setNPCActivity 19 1");
				Screen_Manager::getMe().stopEffect();

				File_Manager::getMe().saveCurrentMapChanges();
				File_Manager::getMe().save_Savegame();

				Screen_Manager::getMe().doSecureClose(Screen::Game);
				Screen_Manager::getMe().changeScreen(Screen::Credits,false);

				Screen_Manager::getMe().fadeIn();
				break;
			}
		}
		break;
	case 9:
		if(aniClock.getElapsedTime().asMilliseconds() >= current_msAniTime)
		{
			if(barrier.getRadius() == 30)
			{
				barrier.setRadius(28);
			}
			else
			{
				barrier.setRadius(30);
			}

			barrier.setOrigin(barrier.getRadius(),barrier.getRadius());
			aniClock.restart();
		}

		if(barrierClock.getElapsedTime().asMilliseconds() > 750)
		{
			showBarrier = false;
			canMove = true;
			currentAnimation = -1;
			spr_Player.setTextureRect(lastLinkAniRect);
			dirAnimation = 0;
			drawAniSprite = false;
		}
		break;
	case 10:
		if(aniClock.getElapsedTime().asMilliseconds() > current_msAniTime)
		{
			aniClock.restart();

			switch(aniMode%8)
			{
			case 0:
				aniSprite.setTextureRect(LINK_FALL_1);
				break;
			case 1:
				aniSprite.setTextureRect(LINK_FALL_2);
				break;
			case 2:
				aniSprite.setTextureRect(LINK_FALL_3);
				break;
			case 3:
				aniSprite.setTextureRect(LINK_FALL_4);
				break;
			case 4:
				aniSprite.setTextureRect(LINK_FALL_5);
				break;
			case 5:
				aniSprite.setTextureRect(LINK_FALL_6);
				break;
			case 6:
				aniSprite.setTextureRect(LINK_FALL_7);
				break;
			case 7:
				aniSprite.setTextureRect(LINK_FALL_8);
				break;
			}

			aniMode++;

			aniSprite.setScale(aniSprite.getScale().x-0.1,aniSprite.getScale().y-0.1);
			
			if(aniMode == 15)
			{
				showBarrier = false;
				canMove = true;
				currentAnimation = -1;
				spr_Player.setTextureRect(lastLinkAniRect);
				dirAnimation = 0;
				drawAniSprite = false;
				aniSprite.setScale(SCALE,SCALE);

				tile *inTile = Map_Manager::getMe().getCurrentMap().getTile(Map_Manager::getMe().getOnMapPosX(posX+TILESIZE), Map_Manager::getMe().getOnMapPosY(posY+TILESIZE+TILESIZE/2));

				if(inTile->settings == ABYSS)
				{
					int lastSP = Map_Manager::getMe().getLastSpawnpoint();

					if(lastSP != -1)
					{
						Player_Manager::getMe().tp(lastTpPos.x,lastTpPos.y);
					}
				}
			}
		}
		break;
	}
}


void Player_Manager::drawPlayer(sf::RenderWindow &mainWindow)
{
	if(drawAniSprite && !drawAniBeforPlayer) mainWindow.draw(aniSprite);

	if(currentAnimation != 10) mainWindow.draw(spr_Player);

	if(drawAniSprite && drawAniBeforPlayer) mainWindow.draw(aniSprite);

	if(currentAnimation == 7)
	{
		mainWindow.draw(sleepText);
	}

	if(foundItemAni)
	{
		mainWindow.draw(itemSprite);
	}

	if(showBarrier)
	{
		mainWindow.draw(barrier);
	}

	if(Map_Manager::getMe().isDrawingFrames())
	{
		sf::RectangleShape box;

		for(auto cursor : nearPlayerCollisionBoxes)
		{
			if(cursor->type == collisionboxType::Standart_Collisionbox) box.setFillColor(sf::Color(200,0,0,150));
			else if(cursor->type == collisionboxType::Interactiv_Collisionbox) box.setFillColor(sf::Color(0,200,0,150));
			else if(cursor->type == collisionboxType::Grabable_Collisionbox) box.setFillColor(sf::Color(0,0,200,150));
			else if(cursor->type == collisionboxType::NPC_Collisionbox) box.setFillColor(sf::Color(200,200,0,150));
			else box.setFillColor(sf::Color(0,0,0,150));

			box.setPosition(Map_Manager::getMe().getMapPosX() + cursor->mapPosX,Map_Manager::getMe().getMapPosY() + cursor->mapPosY);
			box.setSize(sf::Vector2f(cursor->width, cursor->height));

			mainWindow.draw(box);
		}

		box.setFillColor(sf::Color(255,255,255,150));
		box.setPosition(Map_Manager::getMe().getMapPosX() + playerCollisionbox.mapPosX,Map_Manager::getMe().getMapPosY() + playerCollisionbox.mapPosY);
		box.setSize(sf::Vector2f(playerCollisionbox.width,playerCollisionbox.height));

		mainWindow.draw(box);
	}
}


void Player_Manager::doDieAni(sf::RenderWindow &mainWindow)
{
	File_Manager::getMe().forceReset();

	Sound_Manager::getMe().setBackgroundMusicStatus(sf::SoundSource::Status::Stopped);
	Sound_Manager::getMe().clearAllEffectSounds();

	Animation_Manager::getMe().cancelAllAnimation();
	Particle_Manager::getMe().clearParticleList();

	Sound_Manager::sound *fallSound = Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Falldown);
	fallSound->mainSound.setPitch(0.5);

	spr_Player.setColor(sf::Color(255,255,255,255));
	spr_Player.setTextureRect(LINK_DIE);

	short mode = 0;

	sf::Clock aniClock;

	sf::RectangleShape fadeBox;
	fadeBox.setFillColor(sf::Color(0,0,0,0));
	fadeBox.setSize(sf::Vector2f(WINDOW_SIZE_X + 2,WINDOW_SIZE_Y + 2));
	fadeBox.setPosition(-1,-1);

	while(true)
	{
		std::this_thread::sleep_for(std::chrono::microseconds(500));

		if(aniClock.getElapsedTime().asMilliseconds() > 1)
		{
			if(mode == 0)
			{
				int alpha = fadeBox.getFillColor().a;

				if(alpha == 255)
				{
					Animation_Manager::getMe().doZoomAnimation(spr_Player,300,1);
					Animation_Manager::getMe().updateAnimations();
					mode = 1;
				}
				else
				{
					mainWindow.clear();
					Screen_Manager::getMe().getScreen(Screen::Game)->drawScreen(mainWindow);
					mainWindow.draw(fadeBox);
					mainWindow.draw(spr_Player);
					mainWindow.display();
					fadeBox.setFillColor(sf::Color(0,0,0,alpha+1));
				}
			}
			else if(mode == 1)
			{
				if(Animation_Manager::getMe().getRunningAnimationAmount() == 0)
				{
					Animation_Manager::getMe().doSpriteExplosion(spr_Player,9,Particle_Manager::Rectangle);
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Die_Explosion,false,false);
					mode = 2;
				}

				Animation_Manager::getMe().updateAnimations();

				mainWindow.clear();
				mainWindow.draw(spr_Player);
				mainWindow.display();
			}
			else if(mode == 2)
			{
				if(Particle_Manager::getMe().getActivParticleAmount() == 0)
				{
					break;
				}

				Animation_Manager::getMe().updateAnimations();
				Particle_Manager::getMe().updateParticle();

				mainWindow.clear();
				Particle_Manager::getMe().drawParticle(Screen_Manager::getMe().getGameWindow());
				mainWindow.display();
			}

			aniClock.restart();
		}
	}

	Screen_Manager::getMe().changeScreen(Screen::Screen_Type::Gameover,true);
}

void Player_Manager::doItemFoundAni(int itemNumber, bool oneHand)
{
	Item_List::item item = Item_List::getMe().getItem(itemNumber);

	itemSprite = item.itemSprite;
	itemSprite.setPosition(getPosX(),getPosY()-ZOOMSIZE + 1);
	itemSprite.setScale(SCALE,SCALE); 
	foundItemAni = true;

	sf::IntRect lastLinkRect = spr_Player.getTextureRect();

	if(spr_Player.getColor() == dmgColor)
			spr_Player.setColor(sf::Color(255,255,255));

	if(moving_Mode == 0)
	{
		if(oneHand) spr_Player.setTextureRect(LINK_FIND_ONE_HAND);
		else spr_Player.setTextureRect(LINK_FIND_TWO_HANDS);
	}
	else
	{
		if(oneHand) spr_Player.setTextureRect(LINK_FIND_ONE_HAND_SWIMM);
		else spr_Player.setTextureRect(LINK_FIND_TWO_HANDS_SWIMM);
	}

	std::string msg = "Du hast " + Item_List::getMe().getItemArticle(itemNumber) + " " + Item_List::getMe().getItemName(itemNumber) + " gefunden!";
	Message_Manager::getMe().showMsg(msg);

	spr_Player.setTextureRect(lastLinkRect);
	foundItemAni = false;
}

void Player_Manager::doWeapon(Inventory_Manager::slotID weaponSlot)
{
	switch(weaponSlot)
	{
	case Inventory_Manager::Woodsword:
		if(moving_Mode == 0)
		{
			Weapon *sword = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Woodsword,getMapPosX(),getMapPosY(),dir);

			isSwordAniFinished = false;

			((Weapons::Sword *)sword)->setIsFinishedVariable(&isSwordAniFinished);
			((Weapons::Sword *)sword)->setDamage(stats.damage);

			gameStats->addSwordHits();
		}
		break;
	case Inventory_Manager::Ironsword:
		if(moving_Mode == 0)
		{
			Weapon *sword = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Ironsword,getMapPosX(),getMapPosY(),dir);

			isSwordAniFinished = false;

			((Weapons::Sword *)sword)->setIsFinishedVariable(&isSwordAniFinished);
			((Weapons::Sword *)sword)->setDamage(stats.damage);

			gameStats->addSwordHits();
		}
		break;
	case Inventory_Manager::Goldsword:
		if(moving_Mode == 0)
		{
			Weapon *sword = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Goldsword,getMapPosX(),getMapPosY(),dir);

			isSwordAniFinished = false;

			((Weapons::Sword *)sword)->setIsFinishedVariable(&isSwordAniFinished);
			((Weapons::Sword *)sword)->setDamage(stats.damage);

			gameStats->addSwordHits();
		}
		break;
	case Inventory_Manager::Herosword:
		if(moving_Mode == 0)
		{
			Weapon *sword = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Herosword,getMapPosX(),getMapPosY(),dir);

			isSwordAniFinished = false;

			((Weapons::Sword *)sword)->setIsFinishedVariable(&isSwordAniFinished);
			((Weapons::Sword *)sword)->setDamage(stats.damage);

			gameStats->addSwordHits();
		}
		break;
	case Inventory_Manager::Bomb:
		Weapon_Manager::getMe().useWeapon(Weapon_Manager::Bomb,getMapPosX() + 16, getMapPosY() + 16,0);
		Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Bomb,Inventory_Manager::getMe().getSlotAmount(Inventory_Manager::Bomb) - 1);
		gameStats->addPlacedBomb();
		break;
	case Inventory_Manager::Waterbomb:
		Weapon_Manager::getMe().useWeapon(Weapon_Manager::Waterbomb,getMapPosX() + 16, getMapPosY() + 16,0);
		Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Waterbomb,Inventory_Manager::getMe().getSlotAmount(Inventory_Manager::Waterbomb) - 1);
		gameStats->addPlacedBomb();
		break;
	case Inventory_Manager::Bow:
	{
		if(currentAnimation == -1 && moving_Mode == 0) 
		{
			doBowAni();

			Weapon_Manager::weapon arrow;

			switch(Inventory_Manager::getMe().getArrowType())
			{
			case 0:
				arrow = Weapon_Manager::Wood_Arrow;
				break;
			case 1:
				arrow = Weapon_Manager::Feather_Arrow;
				break;
			case 2:
				arrow = Weapon_Manager::Iron_Arrow;
				break;
			default:
				arrow = Weapon_Manager::Wood_Arrow;
				break;
			}

			int currentMapPosX = Map_Manager::getMe().getMapPosX();
			int currentMapPosY = Map_Manager::getMe().getMapPosY();

			switch(dir)
			{
			case LEFT:
				Weapon_Manager::getMe().useWeapon(arrow,aniSprite.getPosition().x - currentMapPosX-10, aniSprite.getPosition().y - currentMapPosY,dir);
				break;
			case RIGHT:
				Weapon_Manager::getMe().useWeapon(arrow,aniSprite.getPosition().x - currentMapPosX+10, aniSprite.getPosition().y - currentMapPosY,dir);
				break;
			case DOWN:
				Weapon_Manager::getMe().useWeapon(arrow,aniSprite.getPosition().x - currentMapPosX, aniSprite.getPosition().y - currentMapPosY+10,dir);
				break;
			case UP:
				Weapon_Manager::getMe().useWeapon(arrow,aniSprite.getPosition().x - currentMapPosX, aniSprite.getPosition().y - currentMapPosY-10,dir);
				break;
			}

			Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Bow,Inventory_Manager::getMe().getSlotAmount(Inventory_Manager::Bow) - 1);
			gameStats->addShotedArrow();
		}

		break;
	}
	case Inventory_Manager::Boomerang:
		if(hasBoomerang)
		{
			if(currentAnimation == -1 && moving_Mode == 0) 
			{
				doKickAni(200);
		
				hasBoomerang = false;

				Weapon *boomerang = Weapon_Manager::getMe().useWeapon(Weapon_Manager::Boomerang,getMapPosX() + 16,getMapPosY() + 16,dir);
				((Weapons::Boomerang *)boomerang)->setIsArrivedVariable(&hasBoomerang);
				((Weapons::Boomerang *)boomerang)->setBoundObj(&playerCollisionbox);
			}
		}

		break;
	case Inventory_Manager::Hammer:
		if(currentAnimation == -1 && moving_Mode == 0) 
		{
			doHammerAni();
		}
		break;
	case Inventory_Manager::Grapplinghock:
		if(hasGrapplibghook)
		{
			if(currentAnimation == -1 && moving_Mode == 0) 
			{
				doHookAni();

				hasGrapplibghook = false;
				Weapon *hook;

				switch(dir)
				{
				case LEFT:
					hook =  Weapon_Manager::getMe().useWeapon(Weapon_Manager::Grapplinghock,getMapPosX(),getMapPosY()+24,dir);
					break;
				case RIGHT:
					hook =  Weapon_Manager::getMe().useWeapon(Weapon_Manager::Grapplinghock,getMapPosX()+32,getMapPosY()+24,dir);
					break;
				case DOWN:
					hook =  Weapon_Manager::getMe().useWeapon(Weapon_Manager::Grapplinghock,getMapPosX()+24,getMapPosY()+34,dir);
					break;
				case UP:
					hook =  Weapon_Manager::getMe().useWeapon(Weapon_Manager::Grapplinghock,getMapPosX()+6,getMapPosY()-4,dir);
					break;
				default:
					hook =  Weapon_Manager::getMe().useWeapon(Weapon_Manager::Grapplinghock,getMapPosX(),getMapPosY()+24,dir);
					break;
				}
				
				((Weapons::Grapplinghook *)hook)->setIsArrivedVariable(&hasGrapplibghook);
				((Weapons::Grapplinghook *)hook)->setBoundObjPositions(&posX,&posY);
			}
		}
		break;
	case Inventory_Manager::Bombbow:
		if(Inventory_Manager::getMe().getSlot(Inventory_Manager::Bomb).amount > 0)
		{
			Weapon_Manager::getMe().useWeapon(Weapon_Manager::Bombbow,getMapPosX() + 16, getMapPosY() + 16,dir);
			Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Bomb,Inventory_Manager::getMe().getSlotAmount(Inventory_Manager::Bomb) - 1);
			gameStats->addPlacedBomb();
		}
		break;
	case Inventory_Manager::Magic_Wand:
		if(currentAnimation == -1 && moving_Mode == 0 && game_HUD->getEnergy() == game_HUD->getMaxEnergy()) 
		{
			game_HUD->setEnergy(0);

			doMagicwandAni();

			int currentMapPosX = Map_Manager::getMe().getMapPosX();
			int currentMapPosY = Map_Manager::getMe().getMapPosY();

			switch(dir)
			{
			case LEFT:
				Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,aniSprite.getPosition().x - currentMapPosX - 8, aniSprite.getPosition().y - currentMapPosY,dir);
				break;
			case RIGHT:
				Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,aniSprite.getPosition().x - currentMapPosX + 8, aniSprite.getPosition().y - currentMapPosY,dir);
				break;
			case DOWN:
				Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,aniSprite.getPosition().x - currentMapPosX, aniSprite.getPosition().y - currentMapPosY + 8,dir);
				break;
			case UP:
				Weapon_Manager::getMe().useWeapon(Weapon_Manager::Laser,aniSprite.getPosition().x - currentMapPosX, aniSprite.getPosition().y - currentMapPosY - 8,dir);
				break;
			}
		}

		break;
	case Inventory_Manager::Amulet:
		
		if(currentAnimation == -1 && moving_Mode == 0 && game_HUD->getEnergy()>100) 
		{
			doBarrierAni();
			game_HUD->setEnergy(game_HUD->getEnergy()-100);
		}
		
		break;
	case Inventory_Manager::None:
		
		break;
	}
}


void Player_Manager::doKickAni(int ms_duration)
{
	if(currentAnimation == -1)
	{
		if(canMove)
		{
			currentAnimation = 0;
			canMove = false;
			current_msAniTime = ms_duration;
			lastLinkAniRect = spr_Player.getTextureRect();

			switch(dir)
			{
			case LEFT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_LEFT);
				break;
			case RIGHT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_RIGHT);
				break;
			case DOWN:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_DOWN);
				break;
			case UP:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_UP);
				break;
			}
		}

		aniClock.restart();
	}
}

void Player_Manager::doSwordAni()
{
	if(currentAnimation == -1)
	{
		if(canMove)
		{
			currentAnimation = 5;
			canMove = false;
			current_msAniTime = 0;
			lastLinkAniRect = spr_Player.getTextureRect();

			switch(dir)
			{
			case LEFT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_LEFT);
				break;
			case RIGHT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_RIGHT);
				break;
			case DOWN:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_DOWN);
				break;
			case UP:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_UP);
				break;
			}
		}

		aniClock.restart();
	}
}

void Player_Manager::doBowAni()
{
	if(currentAnimation == -1)
	{
		if(canMove)
		{
			aniSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
			aniSprite.setScale(SCALE - 0.3,SCALE - 0.3);
			aniSprite.setTextureRect(sf::IntRect(212,48,8,16));
			aniSprite.setOrigin(4,8);

			drawAniSprite = true;

			currentAnimation = 1;
			canMove = false;
			current_msAniTime = 333;
			lastLinkAniRect = spr_Player.getTextureRect();

			switch(dir)
			{
			case LEFT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_LEFT);
				aniSprite.setPosition(posX+4,posY+24);
				aniSprite.setRotation(180);
				drawAniBeforPlayer = false;
				break;
			case RIGHT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_RIGHT);
				aniSprite.setPosition(posX+28,posY+24);
				aniSprite.setRotation(0);
				drawAniBeforPlayer = false;
				break;
			case DOWN:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_DOWN);
				aniSprite.setPosition(posX+24,posY+30);
				aniSprite.setRotation(90);
				drawAniBeforPlayer = true;
				break;
			case UP:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_UP);
				aniSprite.setPosition(posX+6,posY+2);
				aniSprite.setRotation(270);
				drawAniBeforPlayer = false;
				break;
			}

			Sound_Manager::getMe().playEffectSound(Sound_Manager::Bow_Pull);

			weaponHitWaiter.restart();
		}

		aniClock.restart();
	}
}

void Player_Manager::doHammerAni()
{
	if(currentAnimation == -1)
	{
		if(canMove)
		{
			aniSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
			aniSprite.setScale(SCALE,SCALE);
			aniSprite.setRotation(0);
			aniSprite.setOrigin(0,0);

			drawAniSprite = true;

			currentAnimation = 2;
			canMove = false;
			current_msAniTime = 100;
			lastLinkAniRect = spr_Player.getTextureRect();
			aniMode = 0;

			switch(dir)
			{
			case LEFT:
				spr_Player.setTextureRect(LINK_HAMMER_LEFT);
				aniSprite.setTextureRect(sf::IntRect(364,224,-12,16));
				aniSprite.setPosition(posX+8,posY-24);
				drawAniBeforPlayer = true;
				break;
			case RIGHT:
				spr_Player.setTextureRect(LINK_HAMMER_RIGHT);
				aniSprite.setTextureRect(sf::IntRect(352,224,12,16));
				aniSprite.setPosition(posX,posY-24);
				drawAniBeforPlayer = true;
				break;
			case DOWN:
				spr_Player.setTextureRect(LINK_HAMMER_DOWN);
				aniSprite.setTextureRect(sf::IntRect(357,208,10,15));
				aniSprite.setPosition(posX+16,posY-24);
				drawAniBeforPlayer = true;
				break;
			case UP:
				spr_Player.setTextureRect(LINK_HAMMER_UP);
				aniSprite.setTextureRect(sf::IntRect(352,240,10,15));
				aniSprite.setPosition(posX,posY-26);
				drawAniBeforPlayer = true;
				break;
			}

			weaponHitWaiter.restart();
		}

		aniClock.restart();
	}
}

void Player_Manager::doHookAni()
{
	if(currentAnimation == -1)
	{
		if(canMove)
		{
			currentAnimation = 3;
			canMove = false;
			current_msAniTime = 0;
			lastLinkAniRect = spr_Player.getTextureRect();

			switch(dir)
			{
			case LEFT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_LEFT);
				break;
			case RIGHT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_RIGHT);
				break;
			case DOWN:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_DOWN);
				break;
			case UP:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_UP);
				break;
			}
		}

		aniClock.restart();
	}
}

void Player_Manager::doFallAni(int tileX,int tileY)
{
	if(currentAnimation == -1)
	{
		if(lastPlayerMove.y == 1) move(UP);
		else if(lastPlayerMove.y == -1) move(DOWN);

		if(lastPlayerMove.x == 1) move(LEFT);
		else if(lastPlayerMove.x == -1) move(RIGHT);

		currentAnimation = 10;
		canMove = false;
		current_msAniTime = 75;
		drawAniSprite = true;
		drawAniBeforPlayer = true;
		aniMode = 0;
		
		lastLinkAniRect = spr_Player.getTextureRect();
		
		aniSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
		aniSprite.setTextureRect(LINK_FALL_1);
		aniSprite.setOrigin(8,8);
		aniSprite.setScale(SCALE,SCALE);

		int newXPos = tileX * ZOOMSIZE;
		int newYPos = tileY * ZOOMSIZE;

		sf::Vector2f newPos = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(newXPos,newYPos));

		aniSprite.setPosition(newPos.x+16,newPos.y+16);
	}
}

void Player_Manager::doMagicwandAni()
{
	if(currentAnimation == -1)
	{
		if(canMove)
		{
			aniSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
			aniSprite.setScale(SCALE - 0.3,SCALE - 0.3);
			aniSprite.setTextureRect(sf::IntRect(6*16,6*16,16,16));
			aniSprite.setOrigin(8,8);

			drawAniSprite = true;

			currentAnimation = 6;
			canMove = false;
			current_msAniTime = 500;
			lastLinkAniRect = spr_Player.getTextureRect();

			switch(dir)
			{
			case LEFT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_LEFT);
				aniSprite.setPosition(posX-5,posY+24);
				aniSprite.setRotation(270);
				drawAniBeforPlayer = false;
				break;
			case RIGHT:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_RIGHT);
				aniSprite.setPosition(posX+35,posY+24);
				aniSprite.setRotation(90);
				drawAniBeforPlayer = false;
				break;
			case DOWN:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_DOWN);
				aniSprite.setPosition(posX+24,posY+40);
				aniSprite.setRotation(180);
				drawAniBeforPlayer = true;
				break;
			case UP:
				spr_Player.setTextureRect(LINK_FIGHT_ANI_UP);
				aniSprite.setPosition(posX+6,posY-10);
				aniSprite.setRotation(0);
				drawAniBeforPlayer = false;
				break;
			}

			weaponHitWaiter.restart();
		}

		aniClock.restart();
	}
}

void Player_Manager::doSleepAni()
{
	if(currentAnimation == -1)
	{
		drawAniSprite = false;
		currentAnimation = 7;
		canMove = false;
		current_msAniTime = 500;
		aniMode = 0;

		lastLinkAniRect = spr_Player.getTextureRect();
		spr_Player.setTextureRect(LINK_DIE);

		sleepText.setPosition(getPosX(),getPosY()-15);
	}
}

void Player_Manager::doBarrierAni()
{
	if(currentAnimation == -1 && canMove)
	{
		drawAniSprite = false;
		showBarrier = true;
		currentAnimation = 9;
		canMove = false;
		current_msAniTime = 50;
		aniMode = 0;

		barrier.setPosition(posX+16,posY+16);
		barrierClock.restart();

		lastLinkAniRect = spr_Player.getTextureRect();
		spr_Player.setTextureRect(LINK_FIND_ONE_HAND);
	}
}

void Player_Manager::doEndAnimationMovie()
{
	if(currentAnimation == -1)
	{
		Screen_Manager::getMe().fadeOut();

		Map_Manager::getMe().changeCurrentMap("Schloss Neara",16);

		int middleX = WINDOW_SIZE_X / 2 - ZOOMSIZE / 2;
		int middleY = WINDOW_SIZE_Y / 2 - ZOOMSIZE / 2;

		int tpX = middleX - posX;
		int tpY = middleY - posY;

		setPos(middleX-12,middleY);
		Map_Manager::getMe().setMapPos(Map_Manager::getMe().getMapPosX()+tpX-12,Map_Manager::getMe().getMapPosY()+tpY-80);
		spr_Player.setPosition(posX,posY);

		aniSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
		aniSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(Graphic_Manager::People,432));
		aniSprite.setScale(SCALE,SCALE);
		aniSprite.setRotation(0);
		aniSprite.setOrigin(0,0);
		aniSprite.setPosition(posX+32,posY);

		drawAniSprite = true;
		currentAnimation = 8;
		canMove = false;
		current_msAniTime = 0;
		aniMode = 0;

		spr_Player.setTextureRect(LINK_MOVE_ANI_RIGHT);
		lastLinkAniRect = spr_Player.getTextureRect();
	}
}


void Player_Manager::move(int dir)
{
	bool isMoving = false;
	bool isColliding = false;
	
	this->dir = dir;

	if(getMapPosX() + TILESIZE > lastUpdatePositionX  + LINK_UPDATE_RADIUS || getMapPosX() + TILESIZE < lastUpdatePositionX - LINK_UPDATE_RADIUS)
	{
		updatePlayerHitBoxes();
	}
	else if(getMapPosY() + TILESIZE > lastUpdatePositionY + LINK_UPDATE_RADIUS || getMapPosY() + TILESIZE < lastUpdatePositionY - LINK_UPDATE_RADIUS)
	{
		updatePlayerHitBoxes();
	}

	switch(dir)
	{
	case LEFT:
	{
		playerCollisionbox.mapPosX -= movespeed;
		collisionbox* collision = Collision_System::getMe().isCollide(nearPlayerCollisionBoxes,&playerCollisionbox);

		if(collision->type == None_Collisionbox || collision->type == Interact_Only || godmode)
		{
			posX -= movespeed;
			lastPlayerMove.x = -1;
			isMoving = true;
		}
		else if(collision->type == Interactiv_Collisionbox || collision->type == Grabable_Collisionbox)
		{
			if(collision->boundObj != nullptr)
				if(collision->boundObj->isActive())
					collision->boundObj->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosX += movespeed;
		}
		else if(collision->type == NPC_Collisionbox)
		{
			if(collision->boundNPC != nullptr)
				collision->boundNPC->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosX += movespeed;
		}
		else if(collision->type == Enemy_Collisionbox)
		{
			if(collision->boundEnemy != nullptr)
				collision->boundEnemy->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosX += movespeed;
		}
		else
		{
			isColliding = true;
			playerCollisionbox.mapPosX += movespeed;
		}

	break;
	}
	case RIGHT:
	{
		playerCollisionbox.mapPosX += movespeed;
		collisionbox* collision = Collision_System::getMe().isCollide(nearPlayerCollisionBoxes,&playerCollisionbox);
	
		if(collision->type == None_Collisionbox || collision->type == Interact_Only  || godmode)
		{
			posX += movespeed;
			lastPlayerMove.x = 1;
			isMoving = true;
		}
		else if(collision->type == Interactiv_Collisionbox || collision->type == Grabable_Collisionbox)
		{
			if(collision->boundObj != nullptr)
				if(collision->boundObj->isActive())
					collision->boundObj->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosX -= movespeed;
		}
		else if(collision->type == NPC_Collisionbox)
		{
			if(collision->boundNPC != nullptr)
				collision->boundNPC->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosX -= movespeed;
		}
		else if(collision->type == Enemy_Collisionbox)
		{
			if(collision->boundEnemy != nullptr)
				collision->boundEnemy->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosX -= movespeed;
		}
		else
		{
			isColliding = true;
			playerCollisionbox.mapPosX -= movespeed;
		}

	break;
	}
	case DOWN:
	{	
		playerCollisionbox.mapPosY += movespeed;
		collisionbox* collision = Collision_System::getMe().isCollide(nearPlayerCollisionBoxes,&playerCollisionbox);

		if(collision->type == None_Collisionbox || collision->type == Interact_Only  || godmode)
		{
			posY += movespeed;
			lastPlayerMove.y = 1;
			isMoving = true;
		}
		else if(collision->type == Interactiv_Collisionbox || collision->type == Grabable_Collisionbox)
		{
			if(collision->boundObj != nullptr)
				if(collision->boundObj->isActive())
					collision->boundObj->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosY -= movespeed;
		}
		else if(collision->type == NPC_Collisionbox)
		{
			if(collision->boundNPC != nullptr)
					collision->boundNPC->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosY -= movespeed;
		}
		else if(collision->type == Enemy_Collisionbox)
		{
			if(collision->boundEnemy != nullptr)
				collision->boundEnemy->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosY -= movespeed;
		}
		else
		{
			isColliding = true;
			playerCollisionbox.mapPosY -= movespeed;
		}
		
	break;
	}
	case UP:
	{
		playerCollisionbox.mapPosY -= movespeed;
		collisionbox* collision = Collision_System::getMe().isCollide(nearPlayerCollisionBoxes,&playerCollisionbox);

		if(collision->type == None_Collisionbox || collision->type == Interact_Only  || godmode)
		{
			posY -= movespeed;
			lastPlayerMove.y = -1;
			isMoving = true;
		}
		else if(collision->type == Interactiv_Collisionbox || collision->type == Grabable_Collisionbox)
		{
			if(collision->boundObj != nullptr)
				if(collision->boundObj->isActive())
					collision->boundObj->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosY += movespeed;
		}
		else if(collision->type == NPC_Collisionbox)
		{
			if(collision->boundNPC != nullptr)
				collision->boundNPC->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosY += movespeed;
		}
		else if(collision->type == Enemy_Collisionbox)
		{
			if(collision->boundEnemy != nullptr)
				collision->boundEnemy->interact(Collision_Interact);

			isColliding = true;
			playerCollisionbox.mapPosY += movespeed;
		}
		else
		{
			isColliding = true;
			playerCollisionbox.mapPosY += movespeed;
		}
	break;
	}
	}

	if(isColliding) tryToTeleportOutOfHitBox();
}

void Player_Manager::tp(int x,int y)
{
	IOUT("Player-TP: " << x << "," << y);

	lastTpPos = sf::Vector2f(x,y);
	Map_Manager::getMe().setMapPos(posX - x, posY - y); 

	focusPlayer();

	playerCollisionbox.mapPosX = getMapPosX();
	playerCollisionbox.mapPosY = getMapPosY();
	spr_Player.setPosition(posX,posY);
	
	updatePlayerHitBoxes();
}

void Player_Manager::focusPlayer()
{
	int middleX = WINDOW_SIZE_X / 2 - ZOOMSIZE / 2;
	int middleY = WINDOW_SIZE_Y / 2 - ZOOMSIZE / 2;

	int tpX = middleX - posX;
	int tpY = middleY - posY;

	Player_Manager::getMe().setPos(middleX,middleY);
	Map_Manager::getMe().setMapPos(Map_Manager::getMe().getMapPosX()+tpX,Map_Manager::getMe().getMapPosY()+tpY);
}

void Player_Manager::resetItemVariables()
{
	hasBoomerang = true;
	hasGrapplibghook = true;
	isSwordAniFinished = true;
}


void Player_Manager::toggleGodmode()
{
	godmode= !godmode;
}


void Player_Manager::hurt(float damage,bool resetNoDmg)
{
	if(noDmgClock.getElapsedTime().asMilliseconds() > noDmgTime && !godmode && !showBarrier)
	{
		damage = damage - (damage * (float(stats.armor)/100.f));

		DOUT("Schaden: " << damage)

		Sound_Manager::getMe().playEffectSound(Sound_Manager::SoundName::Link_Hurt);

		game_HUD->setLive(game_HUD->getLive() - damage);

		DOUT("Leben: " << this->game_HUD->getLive());

		if(spr_Player.getColor() == sf::Color(255,255,255))
		{	
			Animation_Manager::getMe().doBlinkAnimation(spr_Player,dmgColor,sf::Color::White,4,noDmgTime / 8);
		}

		if(resetNoDmg) noDmgClock.restart();
	}
}

void Player_Manager::heal(float live)
{
	DOUT("Heilung: " << live)

	game_HUD->setLive(game_HUD->getLive() + live);
}

void Player_Manager::give(int itemNumber,unsigned int amount)
{
	switch(itemNumber)
	{
	case 0:
		game_HUD->addEdelsteine(amount);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Rupee);
		break;
	case 1:
		game_HUD->addEdelsteine(amount*5);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Rupee);
		break;
	case 2:
		game_HUD->addEdelsteine(amount*20);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Rupee);
		break;
	case 3:
		game_HUD->addEdelsteine(amount*50);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Rupee);
		break;
	case 4:
		game_HUD->addEdelsteine(amount*100);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Rupee);
		break;
	case 5:
		game_HUD->addEdelsteine(amount*200);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Rupee);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		break;
	case 6:
	case 7:
	case 8:
	case 9:
		Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(itemNumber-6));
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		break;
	case 10:case 11:case 12:case 13:case 14:
	case 15:case 16:case 17:case 18:case 19:
	case 20:case 23:case 24:case 25:case 26:
	case 27:case 28:
		Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(itemNumber-3));
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		break;
	case 21:
	case 22:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Small_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);

		if(Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::slotID(itemNumber-3)))
		{
			Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::slotID(itemNumber-3),Inventory_Manager::getMe().getSlotAmount(Inventory_Manager::slotID(itemNumber-3))+amount);
		}
		else
		{
			Message_Manager::getMe().showMsg("Aber du hast leider keine passende Tasche dafür, du kannst sie nicht tragen.",sf::Color::Red);
		}
		break;
	case 29:
	case 30:
	case 53:
	{
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Small_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);

		bool found = false;
		
		for(int i = 26; i < 30;i++)
		{
			if(Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::slotID(i)) && Inventory_Manager::getMe().getSlotItem(Inventory_Manager::slotID(i)) == 31)
			{
				Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(i));
				Inventory_Manager::getMe().setSlotItem(Inventory_Manager::slotID(i),itemNumber);
				found = true;
				break;
			}
		}

		if(!found)
		{
			Message_Manager::getMe().showMsg("Du hast kein leeres Glas übrig! Der Trank kann nicht umgefüllt werden!",sf::Color::Red);
		}

		break;
	}
	case 31:
	{
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);

		bool found = false;
		
		for(int i = 26; i < 30;i++)
		{
			if(!Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::slotID(i)))
			{
				Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(i));
				Inventory_Manager::getMe().setSlotItem(Inventory_Manager::slotID(i),31);
				found = true;
				break;
			}
		}
		
		if(!found)
		{
			Message_Manager::getMe().showMsg("Dein Inventar ist zu voll um noch weiter Gläser aufzunehmen",sf::Color::Red);
		}
	}
	break;
	case 33:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		Inventory_Manager::getMe().setSlotItem(Inventory_Manager::Moneybag,itemNumber);
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		break;
	case 34:
	case 35:
	case 36:
		if(Inventory_Manager::getMe().isSlotUnlocked(Inventory_Manager::Arrowbag))
		{
			Inventory_Manager::getMe().setSlotAmount(Inventory_Manager::Bow,Inventory_Manager::getMe().getSlotAmount(Inventory_Manager::Bow)+amount);
			Sound_Manager::getMe().playEffectSound(Sound_Manager::Inventory_Select);
		}
		else
		{
			Message_Manager::getMe().showMsg("Ich hab leider keine passende Tasche dafür, ich kann sie nicht tragen.",sf::Color::Red);
		}
		break;
	case 37:
	case 38:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));

		Inventory_Manager::getMe().unlockSlot(Inventory_Manager::Candle);

		if(itemNumber == 38)
		{
			Inventory_Manager::getMe().setSlotItem(Inventory_Manager::Candle,38);
		}

		break;
	case 39:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(31));
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		break;
	case 40:
	case 41:
	case 42:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(itemNumber-36));
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		Message_Manager::getMe().showMsg("Du hast einen neue Umhang erhalten!");
		break;
	case 43:
	case 44:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Inventory_Manager::getMe().unlockSlot(Inventory_Manager::slotID(itemNumber-25));
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		break;
	case 45:
		heal(0.25);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		break;
	case 46:
		heal(0.5);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		break;
	case 47:
		heal(0.75);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		break;
	case 48:
		heal(1);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		break;
	case 49:
		heal(2);
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Get_Heart);
		break;
	case 50:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		game_HUD->setMaxLive(game_HUD->getMaxLive() + 1);
		heal(game_HUD->getMaxLive());
		break;
	case 100:
	case 101:
	case 102:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Big_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		Inventory_Manager::getMe().addTriforceFragment();
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		Dialog_Manager::getMe().doDialog(2);
		break;
	default:
		Sound_Manager::getMe().playEffectSound(Sound_Manager::Fanfare_Small_Item);
		Player_Manager::getMe().doItemFoundAni(itemNumber);
		Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
		break;
	};

	Quest_Manager::getMe().doQuestCommand("found-" + std::to_string(itemNumber));
}


void Player_Manager::checkMoveKeys()
{
	//Wenn irgend eine lauftaste gedrückt wurde
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) || sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		//Überprüfen ob Laufpause überschritten und ob kein Schlag statt findet
		if(moveSpeedClock.getElapsedTime().asMilliseconds() > moveSpeedTime && canMove)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S))
			{
				//Wenn A und D  oder  W und S  gleichzeitig gedrückt werden
				//Do nothing - No Move Animation
			}
			else
			{
				lastPlayerMove = sf::Vector2f(0,0);

				//laufen
				if(sf::Keyboard::isKeyPressed(sf::Keyboard::A)) //Move Left
				{
					move(LEFT);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::D)) //Move Right
				{
					move(RIGHT);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::S)) //Move Down
				{
					move(DOWN);
				}

				if(sf::Keyboard::isKeyPressed(sf::Keyboard::W)) //Move Up
				{
					move(UP);
				}
			}

			//Laufpause zurücksetzen
			moveSpeedClock.restart();
		}

		if(!(sf::Keyboard::isKeyPressed(sf::Keyboard::A) && sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::W) && sf::Keyboard::isKeyPressed(sf::Keyboard::S)))
		{
			if(moveAniClock.getElapsedTime().asMilliseconds() > moveAniTime)
			{
				if(dirAnimation == 1)
				{
					dirAnimation = 0;
					if(moving_Mode == 1)
					{
						Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Swimm);
					}
				}
				else
				{
					dirAnimation = 1;
				}

				gameStats->addStep();

				moveAniClock.restart();
			}
		}
	}
	else
	{
		if(moving_Mode == 1)
		{
			if(swimmAniClock.getElapsedTime().asMilliseconds() > swimmAniTime)
			{
				if(dirAnimation == 1)
				{	
					dirAnimation = 0;
					Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Swimm);
				}
				else
				{
					dirAnimation = 1;
				}

				swimmAniClock.restart();
			}
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift) && game_HUD->getEnergy() > 0)
	{
		if(moving_Mode == 0)
		{
			moveSpeedTime = 10 - (stats.moveSpeed / 10) - 3;
			moveAniTime = 150 - stats.moveSpeed - 30;
		}
		else if(moving_Mode == 1)
		{
			moveSpeedTime = 10 - (stats.swimmSpeed / 10) - 3;
			moveAniTime = 150 - stats.swimmSpeed - 30;
		}
	}
	else
 	{
		if(moving_Mode == 0)
		{
			moveSpeedTime = 10 - (stats.moveSpeed / 10);
			moveAniTime = 200 - stats.moveSpeed - 30;
		}
		else if(moving_Mode == 1)
		{
			moveSpeedTime = 10 - (stats.swimmSpeed / 10);
			moveAniTime = 200 - stats.swimmSpeed - 30;
		}
	}
}

void Player_Manager::ckeckFightKeys()
{
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		if(!swordKeyPressed)
		{
			if(currentAnimation == -1 && moving_Mode == 0) 
			{
				if(Inventory_Manager::getMe().getSelectedSword() == Inventory_Manager::None)
				{
					doKickAni(220);
				}
				else
				{
					doWeapon(Inventory_Manager::getMe().getSelectedSword());
					doSwordAni();
				}
			
				swordKeyPressed = true;
				hitWaiterClock.restart();
			}
		}
	}
	else
	{
		if(hitWaiterClock.getElapsedTime().asMilliseconds() >= 333)
		{
			if(swordKeyPressed) swordKeyPressed = false;
			hitWaiterClock.restart();
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		if(!weaponKeyPressed)
		{
			Inventory_Manager::slotID weapon = Inventory_Manager::getMe().getSelectedWeapon();

			if(weapon != Inventory_Manager::slotID::None && weapon != Inventory_Manager::slotID::Flute)
			{
				if(Inventory_Manager::getMe().getSlot(weapon).amount > 0)
				{
					doWeapon(weapon);
				}
			}
			else if(weapon == Inventory_Manager::slotID::Flute)
			{
				Screen_Manager::getMe().changeScreen(Screen::Flute,false);
			}

			weaponKeyPressed = true;
			weaponHitWaiter.restart();
		}
	}
	else
	{
		if(weaponHitWaiter.getElapsedTime().asMilliseconds() >= 333)
		{
			if(weaponKeyPressed) weaponKeyPressed = false;
		}
	}
}

void Player_Manager::checkDebugKeys()
{

}

void Player_Manager::checkPlayKeys()
{
	//Interact with Object
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Q) && interactKeyClock.getElapsedTime().asMilliseconds() > 50)
	{
		interactKeyClock.restart();

		std::list<collisionbox*> interactivCollisionList;

		switch(dir)
		{
		case LEFT:
		{
			playerCollisionbox.mapPosX -= movespeed;
			Collision_System::getMe().get_colliding_Interactiv_Collisionboxes(interactivCollisionList,&playerCollisionbox);
			playerCollisionbox.mapPosX += movespeed;

			break;
		}
		case RIGHT:
		{
			playerCollisionbox.mapPosX += movespeed;
			Collision_System::getMe().get_colliding_Interactiv_Collisionboxes(interactivCollisionList,&playerCollisionbox);
			playerCollisionbox.mapPosX -= movespeed;
			
			break;
		}
		case DOWN:
		{	
			playerCollisionbox.mapPosY += movespeed;
			Collision_System::getMe().get_colliding_Interactiv_Collisionboxes(interactivCollisionList,&playerCollisionbox);
			playerCollisionbox.mapPosY -= movespeed;

			break;
		}
		case UP:
		{
			playerCollisionbox.mapPosY -= movespeed;
			Collision_System::getMe().get_colliding_Interactiv_Collisionboxes(interactivCollisionList,&playerCollisionbox);
			playerCollisionbox.mapPosY += movespeed;

			break;
		}
		}
		
		//Interagieren
		for(auto cursor:interactivCollisionList)
		{
			if(cursor->type == Interactiv_Collisionbox || cursor->type == Grabable_Collisionbox)
			{
				if(cursor->boundObj != nullptr)
					cursor->boundObj->interact(Key_Interact);
			}
			else if(cursor->type == NPC_Collisionbox)
			{
				if(cursor->boundNPC != nullptr)
					cursor->boundNPC->interact(Key_Interact);
			}
		}

		//check to Block where Link standing in;
		tile *inTile = Map_Manager::getMe().getCurrentMap().getTile(Map_Manager::getMe().getOnMapPosX(posX+TILESIZE), Map_Manager::getMe().getOnMapPosY(posY+TILESIZE));
		bool breakSearch = false;
		
		if(inTile->settings == INTERACTIV)
		{
			inTile->boundObject->interact(Key_Interact);
		}
	}

	if(sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) //LShift
	{
		if(game_HUD->getEnergy() > 0)
		{
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::W) || sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||  sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				if(enegryEmptyClock.getElapsedTime().asMilliseconds() > ms_energyEmptyTime)
				{
					enegryEmptyClock.restart();

					if(game_HUD->getEnergy() > 0)
					game_HUD->setEnergy(game_HUD->getEnergy()-0.5);
				}
			}
			else
			{
				if(energyFillClock.getElapsedTime().asMilliseconds() > ms_energyFillTime)
				{
					energyFillClock.restart();

					if(game_HUD->getEnergy() < game_HUD->getMaxEnergy())
					game_HUD->setEnergy(game_HUD->getEnergy()+0.5);
				}
			}
		}
	}
	else
	{
		if(energyFillClock.getElapsedTime().asMilliseconds() > ms_energyFillTime)
		{
			energyFillClock.restart();

			if(game_HUD->getEnergy() < game_HUD->getMaxEnergy())
			game_HUD->setEnergy(game_HUD->getEnergy()+0.5);
		}
	}
}


void Player_Manager::checkEnvironment()
{
	//check the Block where Link is standing on;
	tile *inTile = Map_Manager::getMe().getCurrentMap().getTile(Map_Manager::getMe().getOnMapPosX(posX+TILESIZE), Map_Manager::getMe().getOnMapPosY(posY+TILESIZE+TILESIZE/2));

	if(inTile->settings == INTERACTIV)
	{
		if(inTile->boundObject != nullptr)
			inTile->boundObject->interact(Collision_Interact);

		if(moving_Mode == 1)
			moving_Mode = 0;
	}
	else if(inTile->settings == WATER)
	{
		if(currentAnimation == -1)
		{
			if(moving_Mode != 1)
			{
				moving_Mode = 1;
				Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Splash);
			}
		}
	}
	else if(inTile->settings == LAVA)
	{
		if(currentAnimation == -1)
		{
			if(moving_Mode != 1)
				moving_Mode = 1;

			hurt(2.5);
		}
	}
	else if(inTile->settings == ABYSS)
	{
		if(currentAnimation == -1)
		{
			if(canMove)
			{
				Sound_Manager::getMe().playEffectSound(Sound_Manager::Link_Fall);
				doFallAni(inTile->x,inTile->y);
				hurt(0.5);
			}
		}
	}
	else
	{
		if(moving_Mode == 1)
			moving_Mode = 0;
	}

	checkItemPaticleCollision();
}

void Player_Manager::checkPlayerMoveBox()
{
	if(posX < 6*ZOOMSIZE)
	{
		int mapMoveX = 6*ZOOMSIZE - posX;
		posX = 6*ZOOMSIZE;

		Map_Manager::getMe().setMapPos( Map_Manager::getMe().getMapPosX() + mapMoveX , Map_Manager::getMe().getMapPosY());
	}

	if(posX > 12*ZOOMSIZE)
	{
		int mapMoveX = posX - 12*ZOOMSIZE;
		posX = 12*ZOOMSIZE;

		Map_Manager::getMe().setMapPos( Map_Manager::getMe().getMapPosX() - mapMoveX , Map_Manager::getMe().getMapPosY());
	}

	if(posY < 6*ZOOMSIZE)
	{
		int mapMoveY = 6*ZOOMSIZE - posY;
		posY = 6*ZOOMSIZE;

		Map_Manager::getMe().setMapPos( Map_Manager::getMe().getMapPosX(), Map_Manager::getMe().getMapPosY() + mapMoveY);
	}

	if(posY > 12*ZOOMSIZE)
	{
		int mapMoveY = posY - 12*ZOOMSIZE;
		posY = 12*ZOOMSIZE;

		Map_Manager::getMe().setMapPos( Map_Manager::getMe().getMapPosX(), Map_Manager::getMe().getMapPosY() - mapMoveY);
	}
}

void Player_Manager::checkItemPaticleCollision()
{
	int foundItem = Particle_Manager::getMe().isCollidingWithItemParticle(&playerCollisionbox);

	if(foundItem > -1)
	{
		give(foundItem,1);
	}
}


void Player_Manager::tryToTeleportOutOfHitBox()
{
	int tries = 0;

	while(true)
	{
		collisionbox *box = Collision_System::getMe().isCollide(nearPlayerCollisionBoxes,&playerCollisionbox);

		if(box->type == None_Collisionbox || box->type == Interact_Only || tries > 100)
		{
			break;
		}

		switch(dir)
		{
		case LEFT:
			posX++;
			break;
		case RIGHT:
			posX--;
			break;
		case DOWN:
			posY--;
			break;
		case UP:
			posY++;
			break;
		}

		playerCollisionbox.mapPosX = getMapPosX();
		playerCollisionbox.mapPosY = getMapPosY();

		updatePlayerHitBoxes();

		tries++;
	}

	if(tries>100)
	{
		FOUT("[CRITICAL] - Teleportations-Fehler!!!!");
	}
}


float Player_Manager::getPosX()
{
	return posX;
}

float Player_Manager::getPosY()
{
	return posY;
}

float Player_Manager::getMapPosX()
{
	return posX - Map_Manager::getMe().getMapPosX();
}

float Player_Manager::getMapPosY()
{
	return posY - Map_Manager::getMe().getMapPosY();
}

short Player_Manager::getLookDir()
{
	return dir;
}

sf::Vector2f Player_Manager::getLastPos()
{
	sf::Vector2f lastPos;

	switch(dir)
	{
	case LEFT:
		lastPos.x = posX+movespeed*2;
		lastPos.y = posY;
		break;
	case RIGHT:
		lastPos.x = posX-movespeed*2;
		lastPos.y = posY;
		break;
	case DOWN:
		lastPos.x = posX;
		lastPos.y = posY-movespeed*2;
		break;
	case UP:
		lastPos.x = posX;
		lastPos.y = posY+movespeed*2;
		break;
	}

	return lastPos;
}

sf::Vector2f Player_Manager::getLastMapPos()
{
	sf::Vector2f lastPos;

	switch(dir)
	{
	case LEFT:
		lastPos.x = posX+movespeed*2;
		lastPos.y = posY;
		break;
	case RIGHT:
		lastPos.x = posX-movespeed*2;
		lastPos.y = posY;
		break;
	case DOWN:
		lastPos.x = posX;
		lastPos.y = posY-movespeed*2;
		break;
	case UP:
		lastPos.x = posX;
		lastPos.y = posY+movespeed*2;
		break;
	}

	lastPos = Map_Manager::getMe().convert_Pos_to_MapPos(lastPos);

	return lastPos;
}


collisionbox* Player_Manager::getCollisionbox()
{
	return &playerCollisionbox;
}

sf::Sprite &Player_Manager::getPlayerSprite()
{
	return spr_Player;
}

Player_Manager::playerStats Player_Manager::getPlayerStats()
{
	return stats;
}

bool Player_Manager::isBarrierOn()
{
	return showBarrier;
}

HUD &Player_Manager::getGameHUD()
{
	return *game_HUD;
}

InGame_Stats &Player_Manager::getGameStats()
{
	return *gameStats;
}


void Player_Manager::setPos(int x, int y)
{
	posX = x;
	posY = y;
}

void Player_Manager::setPlayerColor(sf::Color color)
{
	if(color == sf::Color(0,192,0))
	{
		tex_Player = Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People);
		spr_Player.setTexture(tex_Player);
		return;
	}

	sf::Image img;
	img = Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People).copyToImage();

	int r = color.r;
	int g = color.g;
	int b = color.b;
	int a = color.a;

	if(r < 64) r += 64;
	if(g < 64) g += 64;
	if(b < 64) b += 64;

	sf::Color pixelColor;

	for(int x = 0; x < img.getSize().x; x++)
	{
		for(int y = 0; y < img.getSize().y; y++)
		{
			pixelColor = img.getPixel(x,y);

			if(a > 0)
			{
				if(pixelColor == sf::Color(0,192,0))
				{
					img.setPixel(x,y,sf::Color(r,g,b,a));
				}
				else if(pixelColor == sf::Color(0,128,0))
				{
					img.setPixel(x,y,sf::Color(r-64,g-64,b-64,a));
				}
			}
			else
			{
				if(pixelColor != sf::Color(0,0,0))
				{
					img.setPixel(x,y,sf::Color::Transparent);
				}
			}
		}
	}

	tex_Player.loadFromImage(img);

	spr_Player.setTexture(tex_Player);
}

void Player_Manager::setPlayerCanMove(bool canMove)
{
	this->canMove = canMove;
}

Player_Manager::~Player_Manager(void)
{
	if(game_HUD != nullptr)
	{
		delete game_HUD;
		game_HUD = nullptr;
	}

	if(gameStats != nullptr)
	{
		delete gameStats;
		gameStats = nullptr;
	}

	DELOUT("Ein Player wurde geloescht!\t(" << this << ")");
}