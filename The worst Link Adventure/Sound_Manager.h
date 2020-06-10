#pragma once

#include "SFML/Audio.hpp"
#include <list>

#include "Defines.h"

class Sound_Manager : public Singleton<Sound_Manager>
{

friend class Singleton<Sound_Manager>;

public:

	struct sound
	{
		sf::Sound mainSound;
		sf::SoundBuffer buffer;
	};

	enum SoundName
	{
		//Link
		Link_Hurt,
		Get_Rupee,
		Get_Heart,
		Link_Die_Explosion,
		Link_Fall,
		Link_Swimm,
		Link_Splash,
		Fanfare_Big_Item,
		Fanfare_Small_Item,
		Link_Falldown,

		//Weapons
		Bomb_Explosion,
		Bomb_Fuse,
		Bomb_Defuse,
		Bomb_Drop,

		Boomerang_Fly,
		Boomerang_Catch,
		Boomerang_Throw,

		Hook_Fly,
		Hook_No_Taget_Hit,
		Hook_Target_Hit,

		Arrow_Hit,
		Arrow_Hit_No_Target,
		Arrow_Shoot,

		Bow_Pull,

		Sword_Slash_1,
		Sword_Slash_2,
		Sword_Slash_3,
		Sword_Slash_4,

		Hammer,
		Laser,
		Magic,

		//Enemy
		Enemy_Hit,
		Enemy_Jump,
		Power,
		Magic_Loop,

		//Sonstiges
		Text_Letter,
		Game_Over,
		Pause_Open,
		Pause_Close,
		Found_Secret,
		Inventory_Deselect,
		Inventory_Select,
		Grass_Cut,
		Bing,
		Smack,
		Clang,
		Note_D,
		Note_F,
		Note_A,
		Note_B,
		Note_D2,
	};

	bool loadAllSounds();
	void checkSoundFiles();

	void changeBackgroundMusic(std::string musicPath);
	void changeBackgroundMusicSmoothly(std::string musicPath,int changeDuration_ms = 1000);
	void setBackgroundMusicStatus(sf::Music::Status status);

	sound *playEffectSound(SoundName effekt,bool loop = false, bool parallel = true);
	void clearAllEffectSounds();

	void deleteStoppedSounds();

	void updateManager();

	void setMasterVolume(int volume);
	void setMusikVolume(int volume);
	void setEffektVolume(int volume);

	int getMasterVolume();
	int getMusikVolume();
	int getEffektVolume();

	void updateSoundVolume();

	sf::SoundBuffer *getSoundBuffer(SoundName name);

	void setMusicLockStatus(bool status);

	void setDoDialogSound(bool doSound);
	bool getDoDialogSound();

	~Sound_Manager(void);
private:
	Sound_Manager(void);

	std::list<sound*> soundList;

	//***Sound Buffer***//

	//Link
	sf::SoundBuffer link_hurt_SB;
	sf::SoundBuffer get_Rupee_SB;
	sf::SoundBuffer get_Heart_SB;
	sf::SoundBuffer link_Die_Explosion_SB;
	sf::SoundBuffer game_over_SB;
	sf::SoundBuffer link_fall_SB;
	sf::SoundBuffer link_Swimm_SB;
	sf::SoundBuffer link_splash_SB;
	sf::SoundBuffer	fanfare_Big_Item_SB;
	sf::SoundBuffer	fanfare_Small_Item_SB;
	sf::SoundBuffer link_Falldown_SB;

	//Weaopns
	sf::SoundBuffer bomb_explosion_SB;
	sf::SoundBuffer bomb_fuse_SB;
	sf::SoundBuffer bomb_defuse_SB;
	sf::SoundBuffer bomb_drop_SB;

	sf::SoundBuffer boomerang_fly_SB;
	sf::SoundBuffer boomerang_Catch_SB;
	sf::SoundBuffer boomerang_Throw_SB;

	sf::SoundBuffer hook_fly_SB;
	sf::SoundBuffer hook_no_target_hit_SB;
	sf::SoundBuffer hook_target_hit_SB;

	sf::SoundBuffer arrow_hit_SB;
	sf::SoundBuffer arrow_hit_no_target_SB;
	sf::SoundBuffer arrow_shoot_SB;

	sf::SoundBuffer bow_pull_SB;

	sf::SoundBuffer sword_slash_1_SB;
	sf::SoundBuffer sword_slash_2_SB;
	sf::SoundBuffer sword_slash_3_SB;
	sf::SoundBuffer sword_slash_4_SB;

	sf::SoundBuffer hammer_SB;
	sf::SoundBuffer laser_SB;
	sf::SoundBuffer magic_SB;

	//Enemy
	sf::SoundBuffer enemy_Hit_SB;
	sf::SoundBuffer enemy_Jump_SB;
	sf::SoundBuffer power_SB;
	sf::SoundBuffer magicLoop_SB;

	//Sonstiges
	sf::SoundBuffer text_letter_SB;
	sf::SoundBuffer pause_open_SB;
	sf::SoundBuffer pause_close_SB;
	sf::SoundBuffer found_Secret_SB;
	sf::SoundBuffer	inventory_Deselect_SB;
	sf::SoundBuffer	inventory_Select_SB;
	sf::SoundBuffer grass_cut_SB;
	sf::SoundBuffer bing_SB;
	sf::SoundBuffer smack_SB;
	sf::SoundBuffer clang_SB;
	sf::SoundBuffer note_D_SB;
	sf::SoundBuffer note_F_SB;
	sf::SoundBuffer note_A_SB;
	sf::SoundBuffer note_B_SB;
	sf::SoundBuffer note_D2_SB;

	sound errorSound;

	//***Musik***//
	sf::Music *backgroundMusic;
	sf::Music *smoothBackgroundMusic;

	bool lockMusic;
	bool isMusicChangingSmoothly;
	int musicChangeSpeed_ms;
	sf::Clock changeMusicClock;

	int masterVolume;
	int musikVolume;
	int effektVolume;
	bool doDialogSound;
};

