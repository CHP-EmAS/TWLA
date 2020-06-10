#include "Particle_Manager.h"

#include "Circle_Particle.h"
#include "Rectangle_Particle.h"
#include "Sprite_Particle.h"
#include "Text_Particle.h"
#include "Item_List.h"
#include "Map_Manager.h"

Particle_Manager::Particle_Manager(void)
{
	NEWOUT("Neuer Particle_Manager wurde erstellet! (" << this << ")");

	maxParticleAmount = 300;
}

Particle *Particle_Manager::addParticle(ParticleType type,int size,sf::Color color,unsigned int ms_showTime,unsigned int ms_delayTime,int posX,int posY,int speedX, int speedY,bool relativToMap,bool rotate)
{
	if(particleList.size() + itemParticleList.size() < maxParticleAmount && Screen_Manager::getMe().getShowParticle())
	{
		Particle *newShape;

		switch(type)
		{
		case Circle:
		{
			newShape = new Circle_Particle(size,color,ms_showTime,ms_delayTime,posX,posY,speedX,speedY,relativToMap,rotate);
			break;
		}
		case Rectangle:
			newShape = new Rectangle_Particle(size,color,ms_showTime,ms_delayTime,posX,posY,speedX,speedY,relativToMap,rotate);
			break;
		case Text:
			newShape = new Text_Particle(size,color,ms_showTime,ms_delayTime,posX,posY,speedX,speedY,relativToMap,rotate);
			break;
		}

		particleList.push_back(newShape);

		return newShape;
	}

	return nullptr;
}

Particle *Particle_Manager::addParticle(sf::Sprite particleSprite,unsigned int ms_showTime,unsigned int ms_delayTime,int speedX, int speedY, bool relativToMap, bool rotate)
{
	if(particleList.size() + itemParticleList.size() < maxParticleAmount && Screen_Manager::getMe().getShowParticle())
	{
		Sprite_Particle *newParticle = new Sprite_Particle(particleSprite,ms_showTime,ms_delayTime,speedX,speedY,relativToMap,rotate);
		particleList.push_back(newParticle);

		return newParticle;
	}

	return nullptr;
}

bool Particle_Manager::spawnItemParticle(int itemNumber,int scale,int mapPosX,int mapPosY,int despawnTime_Sec)
{
	itemParticle *newItemParticle = new itemParticle();

	newItemParticle->despawnTime_Sec = despawnTime_Sec;
	newItemParticle->itemNumber = itemNumber;
	newItemParticle->mapPosX = mapPosX;
	newItemParticle->mapPosY = mapPosY;
	newItemParticle->scale = scale;

	Graphic_Manager::tileset itemTileset = Item_List::getMe().getItemTileset(itemNumber);
	newItemParticle->itemParticleSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(itemTileset));
	newItemParticle->itemParticleSprite.setTextureRect(Graphic_Manager::getMe().getIntRect(itemTileset,Item_List::getMe().getItemTile(itemNumber)));
	newItemParticle->itemParticleSprite.setPosition(sf::Vector2f(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(mapPosX,mapPosY))));
	newItemParticle->itemParticleSprite.setScale(scale,scale);

	newItemParticle->aniTick = 0;
	newItemParticle->aniMode = true;

	itemParticleList.push_back(newItemParticle);

	newItemParticle->aniClock.restart();
	newItemParticle->despawnClock.restart();

	return true;
}

int Particle_Manager::isCollidingWithItemParticle(collisionbox* box)
{
	sf::FloatRect cBox(sf::Vector2f(Map_Manager::getMe().convert_MapPos_to_Pos(sf::Vector2i(box->mapPosX,box->mapPosY))),sf::Vector2f(box->width,box->height));

	std::list<itemParticle*>::iterator itemCursor;

	for(itemCursor = itemParticleList.begin(); itemCursor != itemParticleList.end(); ++itemCursor)
	{
		if((*itemCursor)->itemParticleSprite.getGlobalBounds().intersects(cBox))
		{
			int item = (*itemCursor)->itemNumber;

			delete (*itemCursor);

			itemParticleList.erase(itemCursor);
			
			return item;
		}
	}

	return -1;
}

void Particle_Manager::drawParticle(sf::RenderWindow &mainWindow)
{
	std::list<Particle*>::iterator cursor;

	for(cursor = particleList.begin(); cursor != particleList.end(); ++cursor)
	{
		if(!(*cursor)->isParticleComplete())
		{
			(*cursor)->draw(mainWindow);
		}
	}

	for(auto itemCursor : itemParticleList)
	{
		mainWindow.draw(itemCursor->itemParticleSprite);
	}
}

void Particle_Manager::updateParticle()
{
	std::list<Particle*>::iterator cursor;

	for(cursor = particleList.begin(); cursor != particleList.end(); ++cursor)
	{
		if(!(*cursor)->isParticleComplete())
		{
			(*cursor)->update();
		}
		else
		{
			try
			{
				delete (*cursor);

				cursor = particleList.erase(cursor);

				if(cursor == particleList.end()) 
					break;
			}
			catch (const std::exception& e) 
			{
				FOUT(e.what())
			}
		}
	}

	std::list<itemParticle*>::iterator itemCursor;

	for(itemCursor = itemParticleList.begin(); itemCursor != itemParticleList.end(); ++itemCursor)
	{
		if((*itemCursor)->aniClock.getElapsedTime().asMilliseconds() >= 100)
		{
			if((*itemCursor)->aniMode)
			{
				(*itemCursor)->aniTick++;
		
				if((*itemCursor)->aniTick >= 4)
					(*itemCursor)->aniMode = false;
			}
			else
			{
				(*itemCursor)->aniTick--;

				if((*itemCursor)->aniTick <= 0)
					(*itemCursor)->aniMode = true;
			}

			(*itemCursor)->aniClock.restart();
		}

		if((*itemCursor)->despawnClock.getElapsedTime().asSeconds() >= (*itemCursor)->despawnTime_Sec)
		{
			int alpha = (*itemCursor)->itemParticleSprite.getColor().a;

			if(alpha > 0)
			{
				(*itemCursor)->itemParticleSprite.setColor(sf::Color(255,255,255,alpha-1));
			}
			else
			{
				try
				{
					delete (*itemCursor);

					itemCursor = itemParticleList.erase(itemCursor);

					if(itemCursor == itemParticleList.end()) 
						break;
				}
				catch (const std::exception& e) 
				{
					FOUT(e.what())
				}
			}
		}

		(*itemCursor)->itemParticleSprite.setPosition((*itemCursor)->mapPosX + Map_Manager::getMe().getMapPosX(),(*itemCursor)->mapPosY + Map_Manager::getMe().getMapPosY() - (*itemCursor)->aniTick);
	}
}

void Particle_Manager::setMaxParticleAmount(unsigned int amount)
{
	maxParticleAmount = amount;
	DOUT("---------------------------------------------------------------------")
}

void Particle_Manager::clearParticleList()
{
	std::list<Particle*>::iterator cursor;

	for(cursor = particleList.begin(); cursor != particleList.end(); ++cursor)
	{
		delete (*cursor);
	}

	particleList.clear();

	std::list<itemParticle*>::iterator itemCursor;

	for(itemCursor = itemParticleList.begin(); itemCursor != itemParticleList.end(); ++itemCursor)
	{
		delete (*itemCursor);
	}

	itemParticleList.clear();
}

int Particle_Manager::getActivParticleAmount()
{
	return particleList.size();
}

Particle_Manager::~Particle_Manager(void)
{
	clearParticleList();

	DELOUT("Particle_Manager wurde geloescht!\t(" << this << ")");
}
