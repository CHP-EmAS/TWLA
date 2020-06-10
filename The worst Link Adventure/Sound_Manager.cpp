#include "Sound_Manager.h"

Sound_Manager::Sound_Manager(void){
	backgroundMusic = nullptr;
	smoothBackgroundMusic = nullptr;

	isMusicChangingSmoothly = false;
	musicChangeSpeed_ms = 10;

	masterVolume = 75;
	effektVolume = 100;
	musikVolume = 50;

	errorSound.mainSound.stop();

	lockMusic = false;
	doDialogSound = true;

	NEWOUT("Neuer Sound_Manager wurde erstellt! (" << this << ")")
}

bool Sound_Manager::loadAllSounds()
{
	bool loadingFails = false;
	
	if(!link_hurt_SB.loadFromFile("Sounds/Link/Link_Hurt.wav")) 
		loadingFails = true;

	if(!link_Die_Explosion_SB.loadFromFile("Sounds/Link/Link_Die_Explosion.wav")) 
		loadingFails = true;

	if(!link_Swimm_SB.loadFromFile("Sounds/Link/Water_Swim.wav")) 
		loadingFails = true;

	if(!link_splash_SB.loadFromFile("Sounds/Link/Water_Splash.wav")) 
		loadingFails = true;

	if(!link_Die_Explosion_SB.loadFromFile("Sounds/Link/Link_Die_Explosion.wav")) 
		loadingFails = true;
	if(!game_over_SB.loadFromFile("Sounds/Link/Game_Over.wav")) 
		loadingFails = true;

	if(!link_fall_SB.loadFromFile("Sounds/Link/Link_Fall.wav")) 
		loadingFails = true;

	if(!fanfare_Big_Item_SB.loadFromFile("Sounds/Link/Fanfare_Big_Item.wav")) 
		loadingFails = true;

	if(!fanfare_Small_Item_SB.loadFromFile("Sounds/Link/Fanfare_Item.wav")) 
		loadingFails = true;

	if(!bomb_explosion_SB.loadFromFile("Sounds/Weapons/Bomb_Explode.wav")) 
		loadingFails = true;

	if(!bomb_defuse_SB.loadFromFile("Sounds/Weapons/Bomb_Defuse.wav")) 
		loadingFails = true;

	if(!bomb_fuse_SB.loadFromFile("Sounds/Weapons/Bomb_Fuse.wav")) 
		loadingFails = true;

	if(!bomb_drop_SB.loadFromFile("Sounds/Weapons/Bomb_Drop.wav")) 
		loadingFails = true;

	if(!boomerang_fly_SB.loadFromFile("Sounds/Weapons/Boomerang_Fly.wav")) 
		loadingFails = true;

	if(!boomerang_Catch_SB.loadFromFile("Sounds/Weapons/Boomerang_Catch.wav")) 
		loadingFails = true;

	if(!boomerang_Throw_SB.loadFromFile("Sounds/Weapons/Boomerang_Throw.wav")) 
		loadingFails = true;

	if(!hook_fly_SB.loadFromFile("Sounds/Weapons/Hook_Fly.wav")) 
		loadingFails = true;

	if(!hook_no_target_hit_SB.loadFromFile("Sounds/Weapons/Hook_No_Taget_Hit.wav")) 
		loadingFails = true;

	if(!hook_target_hit_SB.loadFromFile("Sounds/Weapons/Hook_Target_Hit.wav")) 
		loadingFails = true;

	if(!get_Rupee_SB.loadFromFile("Sounds/Link/Get_Rupee.wav")) 
		loadingFails = true;

	if(!get_Heart_SB.loadFromFile("Sounds/Link/Get_Heart.wav")) 
		loadingFails = true;

	if(!link_Falldown_SB.loadFromFile("Sounds/Link/Link_FallDown.wav")) 
		loadingFails = true;

	if(!text_letter_SB.loadFromFile("Sounds/Others/Text.wav")) 
		loadingFails = true;

	if(!arrow_hit_SB.loadFromFile("Sounds/Weapons/Arrow_Hit.wav")) 
		loadingFails = true;

	if(!arrow_hit_no_target_SB.loadFromFile("Sounds/Weapons/Arrow_Hit_No_Target.wav")) 
		loadingFails = true;

	if(!arrow_shoot_SB.loadFromFile("Sounds/Weapons/Arrow_Shoot.wav")) 
		loadingFails = true;

	if(!bow_pull_SB.loadFromFile("Sounds/Weapons/Bow_Pull.wav")) 
		loadingFails = true;

	if(!sword_slash_1_SB.loadFromFile("Sounds/Weapons/Sword_Slash1.wav")) 
		loadingFails = true;

	if(!sword_slash_2_SB.loadFromFile("Sounds/Weapons/Sword_Slash2.wav")) 
		loadingFails = true;

	if(!sword_slash_3_SB.loadFromFile("Sounds/Weapons/Sword_Slash3.wav")) 
		loadingFails = true;

	if(!sword_slash_4_SB.loadFromFile("Sounds/Weapons/Sword_Slash4.wav")) 
		loadingFails = true;

	if(!enemy_Hit_SB.loadFromFile("Sounds/Enemy/Enemy_Hit.wav")) 
		loadingFails = true;

	if(!magicLoop_SB.loadFromFile("Sounds/Enemy/Magic_Loop.wav")) 
		loadingFails = true;
	
	if(!enemy_Jump_SB.loadFromFile("Sounds/Enemy/Enemy_Jump.wav")) 
		loadingFails = true;

	if(!power_SB.loadFromFile("Sounds/Enemy/Power.wav")) 
		loadingFails = true;

	if(!pause_open_SB.loadFromFile("Sounds/Others/Pause Open.wav")) 
		loadingFails = true;

	if(!pause_close_SB.loadFromFile("Sounds/Others/Pause Close.wav")) 
		loadingFails = true;

	if(!found_Secret_SB.loadFromFile("Sounds/Others/Secret.wav")) 
		loadingFails = true;

	if(!inventory_Deselect_SB.loadFromFile("Sounds/Others/Inv_Deselect_Item.wav")) 
		loadingFails = true;

	if(!inventory_Select_SB.loadFromFile("Sounds/Others/Inv_Select_Item.wav")) 
		loadingFails = true;

	if(!hammer_SB.loadFromFile("Sounds/Weapons/Hammer.wav")) 
		loadingFails = true;

	if(!laser_SB.loadFromFile("Sounds/Weapons/Laser.wav")) 
		loadingFails = true;

	if(!magic_SB.loadFromFile("Sounds/Weapons/Magic.wav")) 
		loadingFails = true;

	if(!grass_cut_SB.loadFromFile("Sounds/Others/Grass_Cut.wav")) 
		loadingFails = true;

	if(!bing_SB.loadFromFile("Sounds/Others/bing.wav")) 
		loadingFails = true;

	if(!smack_SB.loadFromFile("Sounds/Others/Smack.wav")) 
		loadingFails = true;

	if(!clang_SB.loadFromFile("Sounds/Others/Clang.wav")) 
		loadingFails = true;
	
	if(!note_D_SB.loadFromFile("Sounds/Others/D.wav")) 
		loadingFails = true;

	if(!note_F_SB.loadFromFile("Sounds/Others/F.wav")) 
		loadingFails = true;

	if(!note_A_SB.loadFromFile("Sounds/Others/A.wav")) 
		loadingFails = true;

	if(!note_B_SB.loadFromFile("Sounds/Others/B.wav")) 
		loadingFails = true;

	if(!note_D2_SB.loadFromFile("Sounds/Others/D2.wav")) 
		loadingFails = true;
	
	return !loadingFails;
}

void Sound_Manager::changeBackgroundMusic(std::string musicPath)
{	
	if(lockMusic) return;

	if(backgroundMusic != nullptr) delete backgroundMusic;

	if(smoothBackgroundMusic != nullptr)
	{
		delete smoothBackgroundMusic;
		smoothBackgroundMusic = nullptr;
	}

	backgroundMusic = new sf::Music();

	if(!backgroundMusic->openFromFile(musicPath))
	{
		FOUT("Musik <" << musicPath << "> konnte nicht geladen werden!")
	}

	backgroundMusic->setVolume(musikVolume*masterVolume/100);
	backgroundMusic->setLoop(true);

	backgroundMusic->play();
}

void Sound_Manager::setBackgroundMusicStatus(sf::Music::Status status)
{
	switch(status)
	{
	case sf::Music::Playing:

		if(smoothBackgroundMusic != nullptr)
			smoothBackgroundMusic->play();

		if(backgroundMusic != nullptr)
			backgroundMusic->play();
	
		break;
	case sf::Music::Paused:

		if(smoothBackgroundMusic != nullptr)
			smoothBackgroundMusic->pause();

		if(backgroundMusic != nullptr)
			backgroundMusic->pause();
	
		break;
	case sf::Music::Stopped:

		if(smoothBackgroundMusic != nullptr)
		{
			smoothBackgroundMusic->stop();
			delete smoothBackgroundMusic;
			smoothBackgroundMusic = nullptr;
		}

		if(backgroundMusic != nullptr)
		{
			backgroundMusic->stop();
			delete backgroundMusic;
			backgroundMusic = nullptr;
		}

		break;
	}
}

Sound_Manager::sound *Sound_Manager::playEffectSound(SoundName effekt, bool loop, bool parallel)
{
	if(soundList.size() > 150)
	{
		WOUT("Effekt-Sound " << effekt << " kann nicht abgespielt werden! -> SoundBuffer-Liste ist zu groß!");
		return &errorSound;
	}

	if(!parallel) clearAllEffectSounds();

	sound *newSound = new sound;
	
	newSound->mainSound.setBuffer(*getSoundBuffer(effekt));
	newSound->mainSound.setVolume(effektVolume*masterVolume/100);

	if(loop) newSound->mainSound.setLoop(true);
	newSound->mainSound.play();

	soundList.push_back(newSound);

	return newSound;
}

void Sound_Manager::clearAllEffectSounds()
{
	for(auto cursor:soundList)
	{
		delete cursor;
	}

	soundList.clear();
}

void Sound_Manager::changeBackgroundMusicSmoothly(std::string musicPath,int changDuration_ms)
{
	if(lockMusic) return;

	if(backgroundMusic == nullptr)
	{
		changeBackgroundMusic(musicPath);
		return;
	}

	if(smoothBackgroundMusic != nullptr) delete smoothBackgroundMusic;

	smoothBackgroundMusic = new sf::Music();

	if(!smoothBackgroundMusic->openFromFile(musicPath))
	{
		FOUT("Musik <" << musicPath << "> konnte nicht geladen werden!")
	}

	smoothBackgroundMusic->setVolume(0.f);
	smoothBackgroundMusic->setLoop(true);

	smoothBackgroundMusic->play();

	isMusicChangingSmoothly = true;
	musicChangeSpeed_ms = changDuration_ms / 200;

	changeMusicClock.restart();
}

void Sound_Manager::deleteStoppedSounds()
{
	std::list<sound*>::iterator cursor;

	for(cursor = soundList.begin(); cursor != soundList.end(); ++cursor)
	{
		if((*cursor)->mainSound.getStatus() == sf::Sound::Stopped)
		{
			try
			{
				delete *cursor;

				cursor = soundList.erase(cursor);

				if(cursor == soundList.end()) 
					break;
			}
			catch (const std::exception& e) 
			{
				FOUT(e.what())
			}
		}
	}

	
}

void Sound_Manager::updateManager()
{
	deleteStoppedSounds();

	if(isMusicChangingSmoothly)
	{
		if(changeMusicClock.getElapsedTime().asMilliseconds() > 10)
		{
			if(smoothBackgroundMusic != nullptr && backgroundMusic != nullptr)
			{
				if(backgroundMusic->getVolume() > 0)
				{
					if(backgroundMusic->getVolume() > 2)
					{
						backgroundMusic->setVolume(backgroundMusic->getVolume()-1);
					}
					else
					{
						backgroundMusic->setVolume(0.f);

						backgroundMusic->stop();
						smoothBackgroundMusic->play();
					}
				}
				else
				{
					if(smoothBackgroundMusic->getVolume() < musikVolume*masterVolume/100 - 2)
					{
						smoothBackgroundMusic->setVolume(smoothBackgroundMusic->getVolume()+1);
					}
					else
					{
						smoothBackgroundMusic->setVolume(musikVolume*masterVolume/100);
					}
				}

				if(backgroundMusic->getVolume() == 0 && smoothBackgroundMusic->getVolume() == musikVolume*masterVolume/100)
				{
					isMusicChangingSmoothly = false;

					delete backgroundMusic;

					backgroundMusic = smoothBackgroundMusic;

					smoothBackgroundMusic = nullptr;
				}
			}

			changeMusicClock.restart();
		}
	}
}

void Sound_Manager::setMasterVolume(int volume)
{
	if(volume >= 0 && volume <= 100) masterVolume = volume;
	else
	{
		WOUT("Master-Volumen kann nicht auf " << volume << " gesetzt werden!")
	}

	updateSoundVolume();
}

void Sound_Manager::setMusikVolume(int volume)
{
	if(volume >= 0 && volume <= 100) musikVolume = volume;
	else
	{
		WOUT("Musik-Volumen kann nicht auf " << volume << " gesetzt werden!")
	}

	updateSoundVolume();
}

void Sound_Manager::setEffektVolume(int volume)
{
	if(volume >= 0 && volume <= 100) effektVolume = volume;
	else
	{
		WOUT("Effekt-Volumen kann nicht auf " << volume << " gesetzt werden!")
	}

	updateSoundVolume();
}

int Sound_Manager::getMasterVolume()
{
	return masterVolume;
}

int Sound_Manager::getMusikVolume()
{
	return musikVolume;
}

int Sound_Manager::getEffektVolume()
{
	return effektVolume;
}

void Sound_Manager::updateSoundVolume()
{
	if(backgroundMusic != nullptr)
		backgroundMusic->setVolume(musikVolume*masterVolume/100);

	for(auto cursor:soundList)
	{
		cursor->mainSound.setVolume(effektVolume*masterVolume/100);
	}
}

sf::SoundBuffer* Sound_Manager::getSoundBuffer(SoundName name)
{
	switch(name)
	{
	case Link_Hurt:
		return &link_hurt_SB;
	case Game_Over:
		return &game_over_SB;
	case Link_Die_Explosion:
		return &link_Die_Explosion_SB;
	case Link_Fall:
		return &link_fall_SB;
	case Link_Swimm:
		return &link_Swimm_SB;
	case Link_Splash:
		return &link_splash_SB;
	case Link_Falldown:
		return &link_Falldown_SB;
	case Fanfare_Big_Item:
		return &fanfare_Big_Item_SB;
	case Fanfare_Small_Item:
		return &fanfare_Small_Item_SB;
	case Get_Rupee:
		return &get_Rupee_SB;
	case Get_Heart:
		return &get_Heart_SB;
	case Bomb_Explosion:
		return &bomb_explosion_SB;
	case Bomb_Fuse:
		return &bomb_fuse_SB;
	case Bomb_Defuse:
		return &bomb_defuse_SB;
	case Bomb_Drop:
		return &bomb_drop_SB;
	case Boomerang_Fly:
		return &boomerang_fly_SB;
	case Boomerang_Catch:
		return &boomerang_Catch_SB;
	case Boomerang_Throw:
		return &boomerang_Throw_SB;
	case Hook_Fly:
		return &hook_fly_SB;
	case Hook_No_Taget_Hit:
		return &hook_no_target_hit_SB;
	case Hook_Target_Hit:
		return &hook_target_hit_SB;
	case Arrow_Hit:
		return &arrow_hit_SB;
	case Arrow_Hit_No_Target:
		return &arrow_hit_no_target_SB;
	case Arrow_Shoot:
		return &arrow_shoot_SB;
	case Bow_Pull:
		return &bow_pull_SB;
	case Sword_Slash_1:
		return &sword_slash_1_SB;
	case Sword_Slash_2:
		return &sword_slash_2_SB;
	case Sword_Slash_3:
		return &sword_slash_3_SB;
	case Sword_Slash_4:
		return &sword_slash_4_SB;
	case Laser:
		return &laser_SB;
	case Hammer:
		return &hammer_SB;
	case Magic:
		return &magic_SB;
	case Text_Letter:
		return &text_letter_SB;
	case Enemy_Hit:
		return &enemy_Hit_SB;
	case Enemy_Jump:
		return &enemy_Jump_SB;
	case Power:
		return &power_SB;
	case Magic_Loop:
		return &magicLoop_SB;
	case Pause_Open:
		return &pause_open_SB;
	case Pause_Close:
		return &pause_close_SB;
	case Found_Secret:
		return &found_Secret_SB;
	case Inventory_Deselect:
		return &inventory_Deselect_SB;
	case Inventory_Select:
		return &inventory_Select_SB;
	case Grass_Cut:
		return &grass_cut_SB;
	case Bing:
		return &bing_SB;
	case Clang:
		return &clang_SB;
	case Smack:
		return &smack_SB;
	case Note_D:
		return &note_D_SB;
	case Note_F:
		return &note_F_SB;
	case Note_A:
		return &note_A_SB;
	case Note_B:
		return &note_B_SB;
	case Note_D2:
		return &note_D2_SB;
	default:
		FOUT("Sound Buffer <" << name << "> konnte nicht gefunden werden!")
		return &link_hurt_SB;
	};
}

void Sound_Manager::setMusicLockStatus(bool status)
{
	lockMusic = status;
}

void Sound_Manager::setDoDialogSound(bool doSound)
{
	doDialogSound = doSound;
}

bool Sound_Manager::getDoDialogSound()
{
	return doDialogSound;
}

Sound_Manager::~Sound_Manager(void)
{
	setBackgroundMusicStatus(sf::Music::Stopped);
	clearAllEffectSounds();

	DELOUT("Sound_Manager wurde geloescht!\t(" << this << ")")
}
