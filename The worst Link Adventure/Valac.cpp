#include "Enemys.h"

#include "Graphic_Manager.h"
#include "Map_Manager.h"
#include "Collision_System.h"
#include "Weapon_Manager.h"
#include "Animation_Manager.h"
#include "Particle_Manager.h"
#include "Quest_Manager.h"
#include "Player_Manager.h"
#include "Sound_Manager.h"
#include "Command_Manager.h"
#include "Message_Manager.h"

#define VALAC_STEP_1 sf::IntRect(21*TILESIZE,17*TILESIZE,TILESIZE*2,TILESIZE*2)
#define VALAC_STEP_2 sf::IntRect(23*TILESIZE,17*TILESIZE,TILESIZE*2,TILESIZE*2)
#define VALAC_LOAD_ENEGRY sf::IntRect(29*TILESIZE,17*TILESIZE,TILESIZE*2,TILESIZE*2)
#define VALAC_STOMP_1 sf::IntRect(25*TILESIZE,17*TILESIZE,TILESIZE*2,TILESIZE*2)
#define VALAC_STOMP_2 sf::IntRect(27*TILESIZE,17*TILESIZE,TILESIZE*2,TILESIZE*2)

Valac::Valac(int mapPosX,int mapPosY) : Enemy(mapPosX,mapPosY)
{
	enemySprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	enemySprite.setTextureRect(VALAC_STEP_1);
	enemySprite.setScale(2,2);

	valacCopy.setTexture(Graphic_Manager::getMe().getTilesetTexture(Graphic_Manager::People));
	valacCopy.setTextureRect(VALAC_STEP_1);
	valacCopy.setScale(2,2);

	m_collisionbox.type = collisionboxType::Interact_Only;
	showLiveBar = false;
	init = false;
	isAttacking = true;
	updateAlways = true;
	canCollide = false;

	Enemy::type = EnemyType::Valac;

	name = "Valac";

	m_collisionbox.height = 58;
	m_collisionbox.width = 50;
	moveable::cBoxPosX = 0;
	moveable::cBoxPosY = 6;
	
	switch(File_Manager::getMe().getGameDifficulty())
	{
	case IMMORTAL:
	case EASY:
		damage = 0.25;
		live = 30;
		break;
	case NORMAL:
		damage = 0.5;
		live = 50;
		break;
	case DIFFICULT:
		damage = 0.75;
		live = 50;
		break;
	case HARDCORE:
		damage = 1;
		live = 75;
		break;
	default:
		damage = 0.5;
		live = 50;
		break;
	}

	fullLive = live;

	bossMode = -2;
	moveAniState = false;

	energyWaveStatus = 0;

	//Shield
	showShield = false;
	shield.setFillColor(sf::Color(0,255,255,40));
	shield.setOutlineColor(sf::Color(0,255,255));
	shield.setOutlineThickness(2);
	shield.setRadius(35);
	shield.setOrigin(shield.getRadius(),shield.getRadius());
}

void Valac::update()
{
	if(!init)
	{
		Sound_Manager::getMe().changeBackgroundMusic("Music/Ganon Blight Battle.ogg");
	}

	Enemy::update();

	if(updateClock.getElapsedTime().asMilliseconds() >= 5)
	{
		updateClock.restart();

		switch(bossMode)
		{
		case -2:
			tp(300,338);
			Enemy::update();
			Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x+32,enemySprite.getPosition().y+32,650,true,sf::Color::Red);
			bossMode = -1;
			break;
		case -1:
			if(Animation_Manager::getMe().getRunningAnimationAmount() == 0 && Particle_Manager::getMe().getActivParticleAmount() == 0)
			{
				Message_Manager::getMe().showMsg("Na sieh mal einer an! Ich hätte nicht gedacht das du an Hecktor vorbeikommst!",sf::Color::White,"Valac");
				Message_Manager::getMe().showMsg("Tja, hier wird dein Weg wohl enden!",sf::Color::White,"Valac");
				Message_Manager::getMe().showMsg("Wie wärs mit ein paar Kopien von mir?",sf::Color::White,"Valac");
				bossMode = 0;
			}
			break;
		case 0:
			spawnCopys();

			switch(realPos)
			{
			case 0:
				tp(160,192);
				break;
			case 1:
				tp(448,192);
				break;
			case 2:
				tp(448,480);
				break;
			case 3:
				tp(160,480);
				break;
			}

			waitClock.restart();
			aniClock.restart();
			bossMode = 1;
			break;
		case 1:
			updateCopys();

			if(waitClock.getElapsedTime().asMilliseconds() < 5000)
			{
				if(aniClock.getElapsedTime().asMilliseconds() > 500)
				{
					aniClock.restart();

					if(moveAniState)
					{
						enemySprite.setTextureRect(VALAC_STEP_1);
						valacCopy.setTextureRect(VALAC_STEP_1);
					}
					else
					{
						enemySprite.setTextureRect(VALAC_STEP_2);
						valacCopy.setTextureRect(VALAC_STEP_2);
					}

					moveAniState = !moveAniState;
				}
			}
			else
			{
				enemySprite.setTextureRect(VALAC_STEP_1);
				valacCopy.setTextureRect(VALAC_STEP_1);
				moveAniState = false;
				bossMode = 2;
			}
			break;
		case 2:
		{
			sf::Vector2f expPos1 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(valacCopyPos1) + sf::Vector2f(32,32));
			sf::Vector2f expPos2 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(valacCopyPos2) + sf::Vector2f(32,32));
			sf::Vector2f expPos3 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(valacCopyPos3) + sf::Vector2f(32,32));

			Animation_Manager::getMe().doExplosion(expPos1.x,expPos1.y,650,true,sf::Color::Red);
			Animation_Manager::getMe().doExplosion(expPos2.x,expPos2.y,650,true,sf::Color::Red);
			Animation_Manager::getMe().doExplosion(expPos3.x,expPos3.y,650,true,sf::Color::Red);
			Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x+32,enemySprite.getPosition().y+32,650,true,sf::Color::Red);

			tp(-128,-128);
			waitClock.restart();

			bossMode = 3;
			break;
		}
		case 3:
			if(waitClock.getElapsedTime().asMilliseconds() > 2000)
			{
				tp(300,338);
				Enemy::update();

				Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x+32,enemySprite.getPosition().y+32,650,true,sf::Color::Red);
				bossMode = 8;
			}
			break;
		case 8:
			enemySprite.setTextureRect(VALAC_LOAD_ENEGRY);

			waitClock.restart();
			aniClock.restart();
			
			bossMode = 9;

			MagicLoopSound = Sound_Manager::getMe().playEffectSound(Sound_Manager::Magic_Loop,true);
			MagicLoopSound->mainSound.setPitch(0.5);
			showShield = true;
			break;
		case 9:
			if(waitClock.getElapsedTime().asMilliseconds() < 3000)
			{
				if(aniClock.getElapsedTime().asMilliseconds() > 50)
				{
					aniClock.restart();
					Particle_Manager::getMe().addParticle(Particle_Manager::ParticleType(rand()%2),rand()%6+4,sf::Color(rand()%255,0,0),300,30,enemySprite.getPosition().x+7,enemySprite.getPosition().y+14,rand()%5-2,rand()%5-2,true,rand()%2);
				}
			}
			else
			{
				enemySprite.setTextureRect(VALAC_STOMP_1);
				waitClock.restart();
				bossMode = 10;
			}
			break;
		case 10:
			if(waitClock.getElapsedTime().asMilliseconds() < 1000)
			{
				if(aniClock.getElapsedTime().asMilliseconds() > 20)
				{
					aniClock.restart();
					Particle_Manager::getMe().addParticle(Particle_Manager::ParticleType(rand()%2),rand()%6+4,sf::Color(rand()%255,0,0),300,30,enemySprite.getPosition().x+15,enemySprite.getPosition().y+32,rand()%5-2,rand()%5-2,true,rand()%2);
				}
			}
			else
			{
				enemySprite.setTextureRect(VALAC_STOMP_2);

				waitClock.restart();

				Sound_Manager::getMe().playEffectSound(Sound_Manager::Power);
				spawnEnergyWave(getMapPosX()+25,getMapPosY()+32,0,sf::Color(255,0,0));

				MagicLoopSound->mainSound.stop();
				showShield = false;
				bossMode = 11;
			}
			break;
		case 11:

			break;
		case 12://Valac freez
			if(waitClock.getElapsedTime().asMilliseconds() > 3000)
			{
				enemySprite.setColor(sf::Color(255,255,255));
				bossMode = 0;
			}
			break;
		case 13://Player freez
			if(waitClock.getElapsedTime().asMilliseconds() < 2000)
			{
				if(aniClock.getElapsedTime().asMilliseconds() > 500)
				{
					aniClock.restart();

					Player_Manager::getMe().hurt(damage,false);
				}
			}
			else
			{
				bossMode = 0;
				Player_Manager::getMe().setPlayerCanMove(true);
				Player_Manager::getMe().getPlayerSprite().setColor(sf::Color(255,255,255,255));
			}
			break;
		case 14: //Die
			if(waitClock.getElapsedTime().asMilliseconds() < 5000)
			{
				if(aniClock.getElapsedTime().asMilliseconds() > 200)
				{
					aniClock.restart();

					Sound_Manager::getMe().playEffectSound(Sound_Manager::Power);
					spawnEnergyWave(getMapPosX()+25,getMapPosY()+32,2,sf::Color(0,255,255));
				}
			}
			else
			{
				Screen_Manager::getMe().fadeOut(sf::Color::White);
				std::vector<int> temp;
				kill(temp,0);
			}
			break;
		}

		updateEnegryWaves();

		if(showShield)
		{
			if(rand()%3 == 0)
			{
				if(shield.getRadius() > 34 && shield.getRadius() < 36)
				{
					shield.setRadius(37);
				}
				else
				{
					shield.setRadius(35);
				}

				shield.setOrigin(shield.getRadius(),shield.getRadius());
			}
		}
	}
}

void Valac::draw(sf::RenderWindow &mainWindow)
{
	Enemy::draw(mainWindow);

	if(showShield)
	{
		shield.setPosition(enemySprite.getPosition()+sf::Vector2f(25,32));
		mainWindow.draw(shield);
	}

	if(bossMode == 0 || bossMode == 1)
	{
		valacCopy.setPosition(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(valacCopyPos1)));
		mainWindow.draw(valacCopy);
		valacCopy.setPosition(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(valacCopyPos2)));
		mainWindow.draw(valacCopy);
		valacCopy.setPosition(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(valacCopyPos3)));
		mainWindow.draw(valacCopy);
	}

	for(auto cursor:energyWaves)
	{
		cursor->wave.setPosition(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(cursor->mapPos)));
		mainWindow.draw(cursor->wave);
	}
}

void Valac::interact(interactType interact_Type)
{
	if(bossMode != 8 && bossMode != 9 && bossMode != 10 && bossMode != 11)
	{
		switch(interact_Type)
		{
		case interactType::Arrow_Interact:
		case interactType::Boomerang_Interact:
		case interactType::Explosion_Interact:
		case interactType::Hit_Interact:
		case interactType::Laser_Interact:
		{
			Weapon *interactWeapon = &Weapon_Manager::getMe().getCurrentInteractWeapon();

			if(interactWeapon != nullptr)
			{
				isAttacking = true;
				hurt(interactWeapon->getDamage());
			}
			else
			{
				FOUT("Die aktuelle interaktions Waffe ist NULL!")
			}

			if(bossMode == 1)
			{
				bossMode = 2;
			}

			break;
		}
		}
	}
}

void Valac::hurt(float damage)
{
	DOUT("Enemy damage: " << damage)

	live -= damage;

	Animation_Manager::getMe().doBlinkAnimation(enemySprite,sf::Color::Red,sf::Color().White);

	if(live <= 0)
	{
		bossMode = 14;
	}

	Sound_Manager::getMe().playEffectSound(Sound_Manager::Enemy_Hit);
}

void Valac::kill(std::vector<int> dropItems,unsigned short itemDropAmount)
{
	std::string command = "kill-valac";
	Quest_Manager::getMe().doQuestCommand(command);

	Sound_Manager::getMe().setBackgroundMusicStatus(sf::Music::Stopped);

	Command_Manager::getMe().doCommand("toggleGlobalWall");

 	setActiv(false);

	Screen_Manager::getMe().fadeIn(sf::Color::White);

	Player_Manager::getMe().getGameStats().addMonsterKill();
}

std::string Valac::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active)  + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->live)    + ",";
	fullData += std::to_string(this->isAttacking);

	return fullData;
}

bool Valac::loadMySaveData(std::string data)
{
	int pos = data.find(",");
	std::string str_active = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_mapPosX = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_mapPosY = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_live = data.substr(0,pos);

	data = data.substr(pos+1);
	pos = data.find(",");
	std::string str_isAttacking = data.substr(0,pos);

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));
	
	this->live = atof(str_live.c_str());
	this->isAttacking = atoi(str_isAttacking.c_str());

	return true;
}

void Valac::spawnEnergyWave(int mapX, int mapY, short waveMode, sf::Color waveColor)
{
	energyWave *newWave = new energyWave;

	newWave->wave.setFillColor(sf::Color(200,0,0,0));
	newWave->wave.setOutlineColor(waveColor);
	newWave->wave.setOutlineThickness(4);
	newWave->wave.setPosition(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(mapX,mapY)));

	newWave->mapPos = sf::Vector2i(mapX,mapY);

	newWave->waveMode = waveMode;
	newWave->hitTarget = false;

	if(waveMode == 2)
	{
		newWave->wave.setRadius(750);
	}
	else
	{
		newWave->wave.setRadius(1);
	}

	energyWaves.push_back(newWave);
}

void Valac::updateEnegryWaves()
{
	for(std::list<Valac::energyWave*>::iterator cursor = energyWaves.begin() ; cursor != energyWaves.end(); NULL)
	{
		if(((*cursor)->waveMode != 2 && (*cursor)->wave.getRadius() < 750) || ((*cursor)->waveMode == 2 && (*cursor)->wave.getRadius() > 5))
		{
			if((*cursor)->waveMode == 2) (*cursor)->wave.setRadius((*cursor)->wave.getRadius()-4);
			else (*cursor)->wave.setRadius((*cursor)->wave.getRadius()+4);
			
			(*cursor)->wave.setOrigin((*cursor)->wave.getRadius(),(*cursor)->wave.getRadius());

			if(!(*cursor)->hitTarget)
			{
				if((*cursor)->waveMode == 1)
				{
					if(Collision_System::getMe().distance(enemySprite.getPosition() + sf::Vector2f(32,32),(*cursor)->wave.getPosition()) < (*cursor)->wave.getRadius())
					{
						(*cursor)->hitTarget = true;
						Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x+32,enemySprite.getPosition().y+32,650,true,sf::Color(0,255,255));
						enemySprite.setColor(sf::Color(0,255,255));
						bossMode = 12;
					}
				}
				else if((*cursor)->waveMode == 0)
				{
					if(Collision_System::getMe().distance(Player_Manager::getMe().getPlayerSprite().getPosition() + sf::Vector2f(16,16),(*cursor)->wave.getPosition()) < (*cursor)->wave.getRadius())
					{
						(*cursor)->hitTarget = true;

						if(Player_Manager::getMe().isBarrierOn())
						{
							Sound_Manager::getMe().playEffectSound(Sound_Manager::Power);
							spawnEnergyWave(Player_Manager::getMe().getMapPosX(),Player_Manager::getMe().getMapPosY(),1,sf::Color(255,255,0));
						}
						else
						{
							Player_Manager::getMe().setPlayerCanMove(false);
							Player_Manager::getMe().getPlayerSprite().setColor(sf::Color(0,255,255));
							Animation_Manager::getMe().doExplosion(Player_Manager::getMe().getPosX()+16,Player_Manager::getMe().getPosY()+16,650,true,sf::Color(0,255,255));
							bossMode = 13;
						}
					}
				}
			}

			cursor++;
		}
		else
		{
			if((*cursor)->waveMode == 2) Animation_Manager::getMe().doExplosion(enemySprite.getPosition().x+rand()%64,enemySprite.getPosition().y+rand()%64,650,true,sf::Color(10,10,10));
			cursor = energyWaves.erase(cursor);
		}
	}
}

void Valac::spawnCopys()
{
	realPos = rand()%4;

	sf::Vector2f expPos1 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(160+32,192+32));
	sf::Vector2f expPos2 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(448+32,192+32));
	sf::Vector2f expPos3 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(448+32,480+32));
	sf::Vector2f expPos4 = Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2f(160+32,480+32));

	Animation_Manager::getMe().doExplosion(expPos1.x,expPos1.y,650,true,sf::Color::Red);
	Animation_Manager::getMe().doExplosion(expPos2.x,expPos2.y,650,true,sf::Color::Red);
	Animation_Manager::getMe().doExplosion(expPos3.x,expPos3.y,650,true,sf::Color::Red);
	Animation_Manager::getMe().doExplosion(expPos4.x,expPos4.y,650,true,sf::Color::Red);

	switch(realPos)
	{
	case 0:
	{
		valacCopyPos1 = sf::Vector2i(448,192);
		valacCopyPos2 = sf::Vector2i(448,480);
		valacCopyPos3 = sf::Vector2i(160,480);
		break;
	}
	case 1:
	{
		valacCopyPos1 = sf::Vector2i(448,480);
		valacCopyPos2 = sf::Vector2i(160,480);
		valacCopyPos3 = sf::Vector2i(160,192);
		break;
	}
	case 2:
	{
		valacCopyPos1 = sf::Vector2i(160,480);
		valacCopyPos2 = sf::Vector2i(160,192);
		valacCopyPos3 = sf::Vector2i(448,192);
		break;
	}
	case 3:
	{
		valacCopyPos1 = sf::Vector2i(160,192);
		valacCopyPos2 = sf::Vector2i(448,192);
		valacCopyPos3 = sf::Vector2i(448,480);
		break;
	}
	}

	Enemy::update();
}

void Valac::updateCopys()
{
	if(getMapPosX() <= 160 && getMapPosY() > 192)
	{
		move(UP);
		valacCopyPos1.x+=1;
		valacCopyPos2.y+=1;
		valacCopyPos3.x-=1;
	}
	else if(getMapPosX() < 448 && getMapPosY() <= 192)
	{
		move(RIGHT);
		valacCopyPos1.y+=1;
		valacCopyPos2.x-=1;
		valacCopyPos3.y-=1;
	}
	else if(getMapPosX() >= 448 && getMapPosY() < 480)
	{
		move(DOWN);
		valacCopyPos1.x-=1;
		valacCopyPos2.y-=1;
		valacCopyPos3.x+=1;
	}
	else if(getMapPosX() > 160 && getMapPosY() >= 480)
	{
		move(LEFT);
		valacCopyPos1.y-=1;
		valacCopyPos2.x+=1;
		valacCopyPos3.y+=1;
	}
}

Valac::~Valac(void)
{
}