#include "NPC.h"

#include "NPC_Manager.h"

#include "Animation_Manager.h"
#include "Text_Particle.h"
#include "Quest_Manager.h"
#include "Map_Manager.h"
#include "Dialog_Manager.h"
#include "Message_Manager.h"

NPC::NPC(int spawnPosX,int spawnPosY)
{
	NEWOUT("Neuer NPC wurde erstellt! (" << this << ")")

	this->spawnPosX = spawnPosX;
	this->spawnPosY = spawnPosY;

	mapPosX = spawnPosX;
	mapPosY = spawnPosY;

	m_collisionbox.mapPosX = spawnPosX;
	m_collisionbox.mapPosY = spawnPosY;
	m_collisionbox.boundNPC = this;
	m_collisionbox.boundEnemy = nullptr;
	m_collisionbox.boundObj = nullptr;
	m_collisionbox.height = ZOOMSIZE;
	m_collisionbox.width = ZOOMSIZE;
	m_collisionbox.type = NPC_Collisionbox;

	npcSprite.setPosition(0,0);
	npcSprite.setScale(SCALE,SCALE);

	active = false;
}

void NPC::interact(interactType interact_Type)
{
	switch(interact_Type)
	{
	case Collision_Interact:
		Message_Manager::getMe().showShortHintMsg("Q - Reden");
		break;
	case Key_Interact:
		Dialog_Manager::getMe().doDialog(dialogID);
		break;
	case Explosion_Interact:
		npcSprite.setColor(sf::Color(25,25,25));
	case Arrow_Interact:
	case Hit_Interact:
	case Boomerang_Interact:
		Particle* textParticle = Particle_Manager::getMe().addParticle(Particle_Manager::ParticleType::Text,10,sf::Color::Red,0,10,npcSprite.getPosition().x,npcSprite.getPosition().y-7);
		if(textParticle != nullptr)
		{
			((Text_Particle*)textParticle)->setString("@%#?");
			((Text_Particle*)textParticle)->setFont(Graphic_Manager::font::Pixel);
			((Text_Particle*)textParticle)->changeMode(Text_Particle::Angry);
		}
		break;
	}
}

void NPC::update()
{
	npcSprite.setPosition(Map_Manager::getMe().getMapPosX() + mapPosX,Map_Manager::getMe().getMapPosY() + mapPosY);
}

void NPC::draw(sf::RenderWindow& mainWindow)
{
	mainWindow.draw(npcSprite);

	if(Map_Manager::getMe().isDrawingFrames())
	{
		sf::RectangleShape box;

		box.setFillColor(sf::Color(255,255,0,150));
		box.setPosition(Map_Manager::getMe().getMapPosX() + m_collisionbox.mapPosX,Map_Manager::getMe().getMapPosY() + m_collisionbox.mapPosY);
		box.setSize(sf::Vector2f(m_collisionbox.width,m_collisionbox.height));

		mainWindow.draw(box);
	}
}

collisionbox *NPC::getCollisionbox()
{
	return &m_collisionbox;
}

bool NPC::loadNPC(std::string path)
{
	std::ifstream ifs;
	ifs.open(path.c_str(), std::ios::in);

	if(ifs.good())
	{
		std::list<fullCommand> fullCommandsList;
		
		char c_dummy[500];

		while(!ifs.eof())
		{
			ifs.getline(c_dummy,500,'{');
			ifs.getline(c_dummy,200,'}');

			fullCommand newCommand;

			newCommand.command = c_dummy;
	
			ifs.getline(c_dummy,200,'\n');
			newCommand.content = c_dummy;

			fullCommandsList.push_back(newCommand);
		}

		for(auto cursor : fullCommandsList)
		{
			if(cursor.command == "ID")
			{
				this->ID = atoi(cursor.content.c_str());
			}
			else if(cursor.command == "name")
			{
				this->name = cursor.content;
			}
			else if(cursor.command == "tileset")
			{
				this->tileset = Graphic_Manager::tileset(atoi(cursor.content.c_str()));
			}
			else if(cursor.command == "cutSizeX")
			{
				this->cutSizeX = atoi(cursor.content.c_str());
			}
			else if(cursor.command == "cutSizeY")
			{
				this->cutSizeY = atoi(cursor.content.c_str());
			}
			else if(cursor.command == "cutPosX")
			{
				this->cutPosX = atoi(cursor.content.c_str()) * TILESIZE;
			}
			else if(cursor.command == "cutPosY")
			{
				this->cutPosY = atoi(cursor.content.c_str()) * TILESIZE;
			}
			else if(cursor.command == "interactable")
			{
				this->interactable = bool(atoi(cursor.content.c_str()));
			}
			else if(cursor.command == "dialogID")
			{
				this->dialogID = atoi(cursor.content.c_str());
			}
			else
			{
				WOUT("Skript-Warnung! Befehl <" << cursor.command << "> ist unbekannt! Datei: " << path);
			}
		}
	}
	else
	{
		FOUT("NPC-Datei <" << path << "> konnte nicht geöffnet werden!");
		return false;
	}

	npcSprite.setTexture(Graphic_Manager::getMe().getTilesetTexture(tileset));
	npcSprite.setTextureRect(sf::IntRect(cutPosX,cutPosY,cutSizeX,cutSizeY));

	m_collisionbox.width = cutSizeX*SCALE;
	m_collisionbox.height = cutSizeY*SCALE;

	return true;
}

void NPC::setActiv(bool activ)
{
	if(activ)
	{
		NPC_Manager::getMe().addActivNPC(this);
		this->active = true;
	}
	else
	{
		NPC_Manager::getMe().removeActivNPC(this);
		this->active = false;
	}
}

std::string NPC::getMySaveData()
{
	std::string fullData;

	fullData += std::to_string(this->active) + ",";
	fullData += std::to_string(this->mapPosX) + ",";
	fullData += std::to_string(this->mapPosY) + ",";
	fullData += std::to_string(this->interactable);

	return fullData;
}

sf::Vector2i NPC::getNPCSize()
{
	sf::Vector2i size;

	size.x = npcSprite.getGlobalBounds().width;
	size.y = npcSprite.getGlobalBounds().height;

	return size;
}

bool NPC::loadMySaveData(std::string data)
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
	std::string str_interactable = data.substr(0,pos);

	if(bool(atoi(str_active.c_str())) != this->active)
		setActiv(bool(atoi(str_active.c_str())));

	tp(atoi(str_mapPosX.c_str()),atoi(str_mapPosY.c_str()));

	this->interactable = atoi(str_interactable.c_str());

	return true;
}

NPC::~NPC(void)
{
	DELOUT("Ein NPC wurde geloescht!\t(" << this << ")")
}

