#include "Graphic_Manager.h"

Graphic_Manager::Graphic_Manager(void)
{
	NEWOUT("Neuer Graphic_Manager wurde erstellt! (" << this << ")")
}

void Graphic_Manager::loadAllGraphics()
{
	sf::Image img;
	
	DOUT("")
	IOUT("Tilesets werden geladen...")

	if(!img.loadFromFile("Img/People Tileset.bmp"))
	{
		FOUT("People Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("People Tileset wurde erfolgreich geladen!")
		img.createMaskFromColor(sf::Color::Magenta);
		text_People_Tileset.loadFromImage(img);
	}

	if(!img.loadFromFile("Img/Overworld Tileset.png"))
	{
		FOUT("Overworld Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Overworld Tileset wurde erfolgreich geladen!")
		img.createMaskFromColor(sf::Color::White);
		text_Overworld_Tileset.loadFromImage(img);
	}

	if(!text_Baum_Dungeon_Tileset.loadFromFile("Img/Dungeons/Baum Dungeon Tileset.png"))
	{
		FOUT("Baum Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Baum Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Burg_Dungeon_Tileset.loadFromFile("Img/Dungeons/Burg Dungeon Tileset.png"))
	{
		FOUT("Burg Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Burg Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Kerker_Dungeon_Tileset.loadFromFile("Img/Dungeons/Kerker Dungeon Tileset.png"))
	{
		FOUT("Kerker Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Kerker Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Lava_Dungeon_Tileset.loadFromFile("Img/Dungeons/Lava Dungeon Tileset.png"))
	{
		FOUT("Lava Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Lava Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Schloss_Dungeon_Tileset.loadFromFile("Img/Dungeons/Schloss Dungeon Tileset.png"))
	{
		FOUT("Schloss Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Schloss Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Turm_Dungeon_Tileset.loadFromFile("Img/Dungeons/Turm Dungeon Tileset.png"))
	{
		FOUT("Turm Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Turm Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Wasser_Dungeon_Tileset.loadFromFile("Img/Dungeons/Wasser Dungeon Tileset.png"))
	{
		FOUT("Wasser Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Wasser Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Wüste_Dungeon_Tileset.loadFromFile("Img/Dungeons/Wuesten Dungeon Tileset.png"))
	{
		FOUT("Wuesten Dungeon Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Wuesten Dungeon Tileset wurde erfolgreich geladen!")
	}

	if(!text_Haus_Tileset.loadFromFile("Img/Haus Tileset.png"))
	{
		FOUT("Haus Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Haus Tileset wurde erfolgreich geladen!")
	}

	if(!text_Old_Overworld_Tileset.loadFromFile("Img/Schattenwelt.png"))
	{
		FOUT("Schattenwelt Tileset konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Schattenwelt Tileset wurde erfolgreich geladen!")
	}

	DOUT("")
	IOUT("Sonstige Grafiken werden geladen...")

	if(!img.loadFromFile("Img/Animations/Explosions.png"))
	{
		FOUT("Explosions Grafiken konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Explosions Grafiken wurde erfolgreich geladen!")
		img.createMaskFromColor(sf::Color(255,192,255));
		text_Explosion.loadFromImage(img);
	}

	if(!img.loadFromFile("Img/Animations/water_ani.png"))
	{
		FOUT("Wasser-Animation konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Wasser-Animation wurde erfolgreich geladen!")
		text_Water_Ani.loadFromImage(img);
	}

	if(!img.loadFromFile("Img/Animations/lava_ani.png"))
	{
		FOUT("Lava-Animation konnte nicht geladen werden!")
		loadingFails = true;
	}else
	{
		SOUT("Lava-Animation wurde erfolgreich geladen!")
		text_Lava_Ani.loadFromImage(img);
	}

	DOUT("");
}

void Graphic_Manager::loadAllFonts()
{
	IOUT("Schriftarten werden geladen...")

	if (!arial_Font.loadFromFile("fonts\\arial.ttf"))
	{
	    FOUT("Schriftart Arial konnte nicht geladen werden!")
		loadingFails = true;
	}
	else 
	{
		SOUT("Schriftart Arial wurde erfolgreich geladen!")
	}

	if (!pixel_Font.loadFromFile("fonts\\pixel.otf"))
	{
	    FOUT("Schriftart Pixel konnte nicht geladen werden!")
		loadingFails = true;
	}
	else
	{
		SOUT("Schriftart Pixel wurde erfolgreich geladen!")
	}

	DOUT("")
}

void Graphic_Manager::initAnimations()
{
	warterAniSprite.setTexture(text_Water_Ani);
	warterAniSprite.setTextureRect(sf::IntRect(0,0,16,16));
	warterAniSprite.setScale(SCALE,SCALE);
	waterAnimationTick = 0;

	lavaAniSprite.setTexture(text_Lava_Ani);
	lavaAniSprite.setTextureRect(sf::IntRect(0,0,16,16));
	lavaAniSprite.setScale(SCALE,SCALE);
	lavaAnimationTick = 0;
}

const sf::Texture &Graphic_Manager::getTilesetTexture(Graphic_Manager::tileset tilemap) const
{
	switch(tilemap)
	{
	case Overworld:
		return text_Overworld_Tileset;
	case People:
		return text_People_Tileset;
	case Baum_Dungeon:
		return text_Baum_Dungeon_Tileset;
	case Burg_Dungeon:
		return text_Burg_Dungeon_Tileset;
	case Kerker_Dungeon:
		return text_Kerker_Dungeon_Tileset;
	case Lava_Dungeon:
		return text_Lava_Dungeon_Tileset;
	case Schloss_Dungeon:
		return text_Schloss_Dungeon_Tileset;
	case Turm_Dungeon:
		return text_Turm_Dungeon_Tileset;
	case Wasser_Dungeon:
		return text_Wasser_Dungeon_Tileset;
	case Wüste_Dungeon:
		return text_Wüste_Dungeon_Tileset;
	case Haus:
		return text_Haus_Tileset;
	case Old_Overworld:
		return text_Old_Overworld_Tileset;
	default:
		FOUT("Tileset-Texture <" << tilemap << "> konnte nicht gefunden weden!")
		return text_Overworld_Tileset;
	}
}

sf::Texture Graphic_Manager::getTilesetTextureCopy(Graphic_Manager::tileset tilemap)
{
	switch(tilemap)
	{
	case Overworld:
		return text_Overworld_Tileset;
	case People:
		return text_People_Tileset;
	case Baum_Dungeon:
		return text_Baum_Dungeon_Tileset;
	case Burg_Dungeon:
		return text_Burg_Dungeon_Tileset;
	case Kerker_Dungeon:
		return text_Kerker_Dungeon_Tileset;
	case Lava_Dungeon:
		return text_Lava_Dungeon_Tileset;
	case Schloss_Dungeon:
		return text_Schloss_Dungeon_Tileset;
	case Turm_Dungeon:
		return text_Turm_Dungeon_Tileset;
	case Wasser_Dungeon:
		return text_Wasser_Dungeon_Tileset;
	case Wüste_Dungeon:
		return text_Wüste_Dungeon_Tileset;
	case Haus:
		return text_Haus_Tileset;
	case Old_Overworld:
		return text_Old_Overworld_Tileset;
	default:
		FOUT("Tileset-Texture <" << tilemap << "> konnte nicht gefunden weden!")
		return text_Overworld_Tileset;
	}
}

const sf::Font &Graphic_Manager::getFont(Graphic_Manager::font font) const
{
	switch(font)
	{
	case Arial:
		return arial_Font;
		break;
	case Pixel:
		return pixel_Font;
		break;
	default:
		return arial_Font;
		break;
	}
}

const sf::Sprite Graphic_Manager::getAnimationSprite(animation ani) const
{
	switch(ani)
	{
	case Water_Animation:
		return warterAniSprite;
		break;
	case Lava_Animation:
		return lavaAniSprite;
		break;
	default:
		return warterAniSprite;
		break;
	}
}

const sf::Texture &Graphic_Manager::getTexture(Graphic_Manager::texture texture) const
{
	switch(texture)
	{
	case Explosion:
		return text_Explosion;
		break;
	default:
		return text_Explosion;
		break;
	}
}

bool Graphic_Manager::isLoadedCorrect()
{
	return !loadingFails;
}

sf::IntRect Graphic_Manager::getIntRect(Graphic_Manager::tileset tileMap, int tile)
{
	int sizeX = getTilesetTexture(tileMap).getSize().x / TILESIZE; 

	return sf::IntRect((tile%sizeX)*TILESIZE,(tile/sizeX)*TILESIZE,TILESIZE,TILESIZE);
}

void Graphic_Manager::updateAnimations()
{
	if(warterTickClock.getElapsedTime().asMilliseconds() > 100)
	{
		if(waterAnimationTick < 21) waterAnimationTick++;
		else waterAnimationTick = 0;

		warterAniSprite.setTextureRect(sf::IntRect(waterAnimationTick*16,0,16,16));
	
		warterTickClock.restart();
	}

	if(lavaTickClock.getElapsedTime().asMilliseconds() > 400)
	{
		if(lavaAnimationTick < 3) lavaAnimationTick++;
		else lavaAnimationTick = 0;

		lavaAniSprite.setTextureRect(sf::IntRect(lavaAnimationTick*16,0,16,16));
	
		lavaTickClock.restart();
	}
}

Graphic_Manager::~Graphic_Manager(void)
{
	DELOUT("Graphic_Manager wurde geloescht!\t(" << this << ")")
}
