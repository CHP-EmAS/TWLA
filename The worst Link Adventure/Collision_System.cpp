#include "Collision_System.h"

#include <cmath>

#include "Map_Manager.h"
#include "NPC_Manager.h"
#include "Player_Manager.h"

Collision_System::Collision_System(void)
{
	NEWOUT("Collision System wurde erstellt! (" << this << ")")

	collisionCheckCounter = 0;

	nullCollisionbox.boundNPC = nullptr;
	nullCollisionbox.boundObj = nullptr;
	nullCollisionbox.height = 0;
	nullCollisionbox.width = 0;
	nullCollisionbox.mapPosX = -1;
	nullCollisionbox.mapPosY = -1;
	nullCollisionbox.type = None_Collisionbox;
}


collisionbox *Collision_System::isCollide(int posX, int posY, int width, int height)
{
	int mapPosX = Map_Manager::getMe().getMapPosX();
	int mapPosY = Map_Manager::getMe().getMapPosY();
	Map_Manager *mapManager = &Map_Manager::getMe();

	std::list<collisionbox*>::const_iterator cBoxCursor;

	for(cBoxCursor = mapManager->getCurrentMap().getCollisionBoxes().begin(); cBoxCursor != mapManager->getCurrentMap().getCollisionBoxes().end(); cBoxCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*cBoxCursor)->mapPosX) && posX < (mapPosX + (*cBoxCursor)->mapPosX) + (*cBoxCursor)->width && (posY + height) > (mapPosY + (*cBoxCursor)->mapPosY) && posY < (mapPosY + (*cBoxCursor)->mapPosY) + (*cBoxCursor)->height)
		{
			return *cBoxCursor;
		}
	}

	std::list<InteractivObject*>::const_iterator interObjectCursor;

	for(interObjectCursor = mapManager->getCurrentMap().getObjectList().begin(); interObjectCursor != mapManager->getCurrentMap().getObjectList().end(); interObjectCursor++)
	{
		if((*interObjectCursor)->getCollisionMode())
		{
			collisionbox* box = (*interObjectCursor)->getCollisionBox();

			collisionCheckCounter++;

			if((posX + width) > (mapPosX + box->mapPosX) && posX < (mapPosX + box->mapPosX) + box->width && (posY + height) > (mapPosY + box->mapPosY) && posY < (mapPosY + box->mapPosY) + box->height)
			{
				return box;
			}
		}
	}

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			return *npcCollisionboxesCursor;
		}
	}

	//Player
	collisionbox* playerBox = Player_Manager::getMe().getCollisionbox();

	collisionCheckCounter++;

	if((posX + width) > (mapPosX + playerBox->mapPosX) && posX < (mapPosX + playerBox->mapPosX) + playerBox->width && (posY + height) > (mapPosY + playerBox->mapPosY) && posY < (mapPosY + playerBox->mapPosY) + playerBox->height)
	{
		return playerBox;
	}
	
	//No Collisionbox found
	return &nullCollisionbox;
}

collisionbox *Collision_System::isCollide(const collisionbox* cBox)
{
	int mapPosX = Map_Manager::getMe().getMapPosX();
	int mapPosY = Map_Manager::getMe().getMapPosY();
	Map_Manager *mapManager = &Map_Manager::getMe();

	sf::Vector2i pos = mapManager->convert_MapPos_to_Pos(sf::Vector2i(cBox->mapPosX,cBox->mapPosY));

	int posX = pos.x;
	int posY = pos.y;
	int width = cBox->width;
	int height = cBox->height;

	std::list<collisionbox*>::const_iterator cBoxCursor;

	for(cBoxCursor = mapManager->getCurrentMap().getCollisionBoxes().begin(); cBoxCursor != mapManager->getCurrentMap().getCollisionBoxes().end(); cBoxCursor++)
	{
		if((*cBoxCursor) == cBox) continue;

		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*cBoxCursor)->mapPosX) && posX < (mapPosX + (*cBoxCursor)->mapPosX) + (*cBoxCursor)->width && (posY + height) > (mapPosY + (*cBoxCursor)->mapPosY) && posY < (mapPosY + (*cBoxCursor)->mapPosY) + (*cBoxCursor)->height)
		{
			return *cBoxCursor;
		}
	}

	std::list<InteractivObject*>::const_iterator interObjectCursor;

	for(interObjectCursor = mapManager->getCurrentMap().getObjectList().begin(); interObjectCursor != mapManager->getCurrentMap().getObjectList().end(); interObjectCursor++)
	{
		if((*interObjectCursor)->getCollisionMode())
		{
			collisionbox* box = (*interObjectCursor)->getCollisionBox();

			if(box == cBox) continue;

			collisionCheckCounter++;

			if((posX + width) > (mapPosX + box->mapPosX) && posX < (mapPosX + box->mapPosX) + box->width && (posY + height) > (mapPosY + box->mapPosY) && posY < (mapPosY + box->mapPosY) + box->height)
			{
				return box;
			}
		}
	}

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		if((*npcCollisionboxesCursor) == cBox) continue;

		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			return *npcCollisionboxesCursor;
		}
	}

	//Player
	collisionbox* playerBox = Player_Manager::getMe().getCollisionbox();

	if(playerBox != cBox)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + playerBox->mapPosX) && posX < (mapPosX + playerBox->mapPosX) + playerBox->width && (posY + height) > (mapPosY + playerBox->mapPosY) && posY < (mapPosY + playerBox->mapPosY) + playerBox->height)
		{
			return playerBox;
		}
	}

	return &nullCollisionbox;
}


collisionbox *Collision_System::isCollide(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();
	
	std::list<collisionbox*>::const_iterator cBoxCursor;

	for(cBoxCursor = cBoxList.begin(); cBoxCursor != cBoxList.end(); cBoxCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*cBoxCursor)->mapPosX) && posX < (mapPosX + (*cBoxCursor)->mapPosX) + (*cBoxCursor)->width && (posY + height) > (mapPosY + (*cBoxCursor)->mapPosY) && posY < (mapPosY + (*cBoxCursor)->mapPosY) + (*cBoxCursor)->height)
		{
			return *cBoxCursor;
		}
	}

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			return *npcCollisionboxesCursor;
		}
	}

	//Player
	collisionbox* playerBox = Player_Manager::getMe().getCollisionbox();

	collisionCheckCounter++;

	if((posX + width) > (mapPosX + playerBox->mapPosX) && posX < (mapPosX + playerBox->mapPosX) + playerBox->width && (posY + height) > (mapPosY + playerBox->mapPosY) && posY < (mapPosY + playerBox->mapPosY) + playerBox->height)
	{
		return playerBox;
	}
	
	//No Collisionbox found
	return &nullCollisionbox;
}

collisionbox *Collision_System::isCollide(std::list<collisionbox*> &cBoxList,const collisionbox* cBox)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	sf::Vector2i pos = mapManager->convert_MapPos_to_Pos(sf::Vector2i(cBox->mapPosX,cBox->mapPosY));

	int posX = pos.x;
	int posY = pos.y;
	int width = cBox->width;
	int height = cBox->height;

	std::list<collisionbox*>::const_iterator cBoxCursor;

	for(cBoxCursor = cBoxList.begin(); cBoxCursor != cBoxList.end(); cBoxCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*cBoxCursor)->mapPosX) && posX < (mapPosX + (*cBoxCursor)->mapPosX) + (*cBoxCursor)->width && (posY + height) > (mapPosY + (*cBoxCursor)->mapPosY) && posY < (mapPosY + (*cBoxCursor)->mapPosY) + (*cBoxCursor)->height)
		{
			return *cBoxCursor;
		}
	}

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		collisionCheckCounter++;

		if((*npcCollisionboxesCursor) == cBox) continue;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			return *npcCollisionboxesCursor;
		}
	}

	//Player
	collisionbox* playerBox = Player_Manager::getMe().getCollisionbox();

	if(playerBox != cBox)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + playerBox->mapPosX) && posX < (mapPosX + playerBox->mapPosX) + playerBox->width && (posY + height) > (mapPosY + playerBox->mapPosY) && posY < (mapPosY + playerBox->mapPosY) + playerBox->height)
		{
			return playerBox;
		}
	}

	return &nullCollisionbox;
}


bool Collision_System::boxCollision(collisionbox *box1,collisionbox *box2)
{	
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	sf::Vector2i pos1 = mapManager->convert_MapPos_to_Pos(sf::Vector2i(box1->mapPosX,box1->mapPosY));
	int pos1X = pos1.x;
	int pos1Y = pos1.y;
	int width1 = box1->width;
	int height1 = box1->height;

	sf::Vector2i pos2 = mapManager->convert_MapPos_to_Pos(sf::Vector2i(box2->mapPosX,box2->mapPosY));
	int pos2X = pos2.x;
	int pos2Y = pos2.y;
	int width2 = box2->width;
	int height2 = box2->height;

	if((pos1X + width1) > pos2X && pos1X < pos2X + width2 && (pos1Y + height1) > pos2Y && pos1Y < pos2Y + height2)
	{
		return true;
	}

	return false;
}

bool Collision_System::intersection(sf::IntRect rect1,sf::IntRect rect2)
{
	if((rect1.left + rect1.width) > rect2.left && rect1.left < rect2.left + rect2.width && (rect1.top + rect1.height) > rect2.top && rect1.top < rect2.top + rect2.height)
	{
		return true;
	}

	return false;
}


void Collision_System::get_colliding_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	cBoxList.clear();

	//Normal Hitboxes
	std::list<collisionbox*>::const_iterator cBoxCursor;

	for(cBoxCursor = mapManager->getCurrentMap().getCollisionBoxes().begin(); cBoxCursor != mapManager->getCurrentMap().getCollisionBoxes().end(); cBoxCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*cBoxCursor)->mapPosX) && posX < (mapPosX + (*cBoxCursor)->mapPosX) + (*cBoxCursor)->width && (posY + height) > (mapPosY + (*cBoxCursor)->mapPosY) && posY < (mapPosY + (*cBoxCursor)->mapPosY) + (*cBoxCursor)->height)
		{
			cBoxList.push_back(*cBoxCursor);
		}
	}

	//Interactiv Object
	std::list<InteractivObject*>::const_iterator interObjectCursor;

	for(interObjectCursor = mapManager->getCurrentMap().getObjectList().begin(); interObjectCursor != mapManager->getCurrentMap().getObjectList().end(); interObjectCursor++)
	{
		if((*interObjectCursor)->getCollisionMode())
		{
			collisionbox* box = (*interObjectCursor)->getCollisionBox();

			collisionCheckCounter++;

			if((posX + width) > (mapPosX + box->mapPosX) && posX < (mapPosX + box->mapPosX) + box->width && (posY + height) > (mapPosY + box->mapPosY) && posY < (mapPosY + box->mapPosY) + box->height)
			{
				cBoxList.push_back(box);
			}
		}
	}
}

void Collision_System::get_colliding_Normal_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	cBoxList.clear();

	//Normal Hitboxes
	std::list<collisionbox*>::const_iterator cBoxCursor;

	for(cBoxCursor = mapManager->getCurrentMap().getCollisionBoxes().begin(); cBoxCursor != mapManager->getCurrentMap().getCollisionBoxes().end(); cBoxCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*cBoxCursor)->mapPosX) && posX < (mapPosX + (*cBoxCursor)->mapPosX) + (*cBoxCursor)->width && (posY + height) > (mapPosY + (*cBoxCursor)->mapPosY) && posY < (mapPosY + (*cBoxCursor)->mapPosY) + (*cBoxCursor)->height)
		{
			cBoxList.push_back(*cBoxCursor);
		}
	}
}


void Collision_System::get_colliding_Interactiv_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	cBoxList.clear();

	//Interactiv Object
	std::list<InteractivObject*>::const_iterator interObjectCursor;

	for(interObjectCursor = mapManager->getCurrentMap().getObjectList().begin(); interObjectCursor != mapManager->getCurrentMap().getObjectList().end(); interObjectCursor++)
	{
		if((*interObjectCursor)->getCollisionMode())
		{
			collisionbox* box = (*interObjectCursor)->getCollisionBox();

			collisionCheckCounter++;

			if((posX + width) > (mapPosX + box->mapPosX) && posX < (mapPosX + box->mapPosX) + box->width && (posY + height) > (mapPosY + box->mapPosY) && posY < (mapPosY + box->mapPosY) + box->height)
			{
				cBoxList.push_back(box);
			}
		}
	}

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			cBoxList.push_back(*npcCollisionboxesCursor);
		}
	}

	//Player
	collisionbox* playerBox = Player_Manager::getMe().getCollisionbox();

	collisionCheckCounter++;

	if((posX + width) > (mapPosX + playerBox->mapPosX) && posX < (mapPosX + playerBox->mapPosX) + playerBox->width && (posY + height) > (mapPosY + playerBox->mapPosY) && posY < (mapPosY + playerBox->mapPosY) + playerBox->height)
	{
		cBoxList.push_back(playerBox);
	}
}

void Collision_System::get_colliding_Interactiv_Collisionboxes(std::list<collisionbox*> &cBoxList,const collisionbox *cBox)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	sf::Vector2i pos = mapManager->convert_MapPos_to_Pos(sf::Vector2i(cBox->mapPosX,cBox->mapPosY));

	int posX = pos.x;
	int posY = pos.y;
	int width = cBox->width;
	int height = cBox->height;

	cBoxList.clear();

	//Interactiv Object
	std::list<InteractivObject*>::const_iterator interObjectCursor;

	for(interObjectCursor = mapManager->getCurrentMap().getObjectList().begin(); interObjectCursor != mapManager->getCurrentMap().getObjectList().end(); interObjectCursor++)
	{
		if((*interObjectCursor)->getCollisionMode())
		{
			collisionbox* box = (*interObjectCursor)->getCollisionBox();

			collisionCheckCounter++;

			if((posX + width) > (mapPosX + box->mapPosX) && posX < (mapPosX + box->mapPosX) + box->width && (posY + height) > (mapPosY + box->mapPosY) && posY < (mapPosY + box->mapPosY) + box->height)
			{
				cBoxList.push_back(box);
			}
		}
	}

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			cBoxList.push_back(*npcCollisionboxesCursor);
		}
	}

	//Player
	collisionbox* playerBox = Player_Manager::getMe().getCollisionbox();

	collisionCheckCounter++;

	if((posX + width) > (mapPosX + playerBox->mapPosX) && posX < (mapPosX + playerBox->mapPosX) + playerBox->width && (posY + height) > (mapPosY + playerBox->mapPosY) && posY < (mapPosY + playerBox->mapPosY) + playerBox->height)
	{
		cBoxList.push_back(playerBox);
	}
}


void Collision_System::get_colliding_NPC_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	cBoxList.clear();

	//NPCs
	std::list<collisionbox*>::const_iterator npcCollisionboxesCursor;

	for(npcCollisionboxesCursor = NPC_Manager::getMe().get_Activ_Collisionboxes().begin(); npcCollisionboxesCursor != NPC_Manager::getMe().get_Activ_Collisionboxes().end(); npcCollisionboxesCursor++)
	{
		collisionCheckCounter++;

		if((posX + width) > (mapPosX + (*npcCollisionboxesCursor)->mapPosX) && posX < (mapPosX + (*npcCollisionboxesCursor)->mapPosX) + (*npcCollisionboxesCursor)->width && (posY + height) > (mapPosY + (*npcCollisionboxesCursor)->mapPosY) && posY < (mapPosY + (*npcCollisionboxesCursor)->mapPosY) + (*npcCollisionboxesCursor)->height)
		{
			cBoxList.push_back(*npcCollisionboxesCursor);
		}
	}
}

void Collision_System::get_colliding_Obj_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height)
{
	Map_Manager *mapManager = &Map_Manager::getMe();
	int mapPosX = mapManager->getMapPosX();
	int mapPosY = mapManager->getMapPosY();

	cBoxList.clear();

	//Interactiv Object
	std::list<InteractivObject*>::const_iterator interObjectCursor;

	for(interObjectCursor = mapManager->getCurrentMap().getObjectList().begin(); interObjectCursor != mapManager->getCurrentMap().getObjectList().end(); interObjectCursor++)
	{
		if((*interObjectCursor)->getCollisionMode())
		{
			collisionbox* box = (*interObjectCursor)->getCollisionBox();

			collisionCheckCounter++;

			if((posX + width) > (mapPosX + box->mapPosX) && posX < (mapPosX + box->mapPosX) + box->width && (posY + height) > (mapPosY + box->mapPosY) && posY < (mapPosY + box->mapPosY) + box->height)
			{
				cBoxList.push_back(box);
			}
		}
	}
}


int Collision_System::distance(sf::Vector2i pos1,sf::Vector2i pos2)
{
	int dx = pos1.x - pos2.x;
	int dy = pos1.y - pos2.y;
	return sqrt((dx * dx) + (dy * dy));
}

float Collision_System::distance(sf::Vector2f pos1,sf::Vector2f pos2)
{
	float dx = pos1.x - pos2.x;
	float dy = pos1.y - pos2.y;
	return sqrtf((dx * dx) + (dy * dy));
}


int Collision_System::getCollisionCounter()
{
	return collisionCheckCounter;
}

void Collision_System::resetCollisionCounter()
{
	collisionCheckCounter = 0;
}


Collision_System::~Collision_System(void)
{
	DELOUT("Collision System wurde geloescht!\t(" << this << ")")
}
