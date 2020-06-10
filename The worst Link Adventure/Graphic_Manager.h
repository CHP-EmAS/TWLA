#pragma once

#include "SFML\Graphics.hpp"

#include "Defines.h"

class Graphic_Manager : public Singleton<Graphic_Manager>
{
friend class Singleton<Graphic_Manager>;
public:
	//Enum
	enum tileset
	{
		Overworld,
		People,
		Baum_Dungeon,
		Burg_Dungeon,
		Kerker_Dungeon,
		Lava_Dungeon,
		Schloss_Dungeon,
		Turm_Dungeon,
		Wasser_Dungeon,
		Wüste_Dungeon,
		Haus,
		Old_Overworld
	};

	enum font
	{
		Arial,
		Pixel
	};

	enum texture
	{
		Explosion,
	};

	enum animation
	{
		Water_Animation,
		Lava_Animation
	};

	//methoden
	void loadAllGraphics();
	void loadAllFonts();
	void initAnimations();

	void updateAnimations();

	//get
	const sf::Texture &getTilesetTexture(tileset) const;
	sf::Texture getTilesetTextureCopy(tileset);

	const sf::Texture &getTexture(texture) const;

	const sf::Font &getFont(font) const;

	bool isLoadedCorrect();

	sf::IntRect getIntRect(tileset, int tile);

	const sf::Sprite getAnimationSprite(animation ani) const;

	~Graphic_Manager(void);
private:

	Graphic_Manager(void);
private:

	//------Tilesets------//
	sf::Texture text_People_Tileset;
	sf::Texture text_Overworld_Tileset;
	sf::Texture text_Baum_Dungeon_Tileset;
	sf::Texture text_Burg_Dungeon_Tileset;
	sf::Texture text_Kerker_Dungeon_Tileset;
	sf::Texture text_Lava_Dungeon_Tileset;
	sf::Texture text_Schloss_Dungeon_Tileset;
	sf::Texture text_Turm_Dungeon_Tileset;
	sf::Texture text_Wasser_Dungeon_Tileset;
	sf::Texture text_Wüste_Dungeon_Tileset;
	sf::Texture text_Haus_Tileset;
	sf::Texture text_Old_Overworld_Tileset;

	//------Textures------//
	sf::Texture text_Explosion;
	sf::Texture text_Water_Ani;
	sf::Texture text_Lava_Ani;

	//------Fonts------//
	sf::Font arial_Font;
	sf::Font pixel_Font;

	//------Attributes------//
	bool loadingFails;

	//Wasser-Animation
	short waterAnimationTick;
	sf::Clock warterTickClock;
	sf::Sprite warterAniSprite;

	//Lava-Animation
	short lavaAnimationTick;
	sf::Clock lavaTickClock;
	sf::Sprite lavaAniSprite;
};

