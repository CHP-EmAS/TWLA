#include "moveable.h"

#include "Collision_System.h"
#include "Map_Manager.h"

moveable::moveable(void)
{
	mapPosX = 0; 
	mapPosY = 0;
	
	lastUpdatePositionX = -1000;
	lastUpdatePositionY = -1000;

	movespeed = 1; //standart 1
	lastMovingDir = 0;

	collisionbox_Update_Range = 100;

	canMove = true;

	m_collisionbox.boundNPC = nullptr;
	m_collisionbox.boundObj = nullptr;
	m_collisionbox.height = 0;
	m_collisionbox.width = 0;
	m_collisionbox.mapPosX = 0;
	m_collisionbox.mapPosY = 0;
	m_collisionbox.type = collisionboxType::None_Collisionbox;

	cBoxPosX = 0;
	cBoxPosY = 0;
}

collisionbox* moveable::move(int dir)
{
	if(!canCollide)
	{
		switch(dir)
		{
		case LEFT:
			m_collisionbox.mapPosX -= movespeed;
			mapPosX -= movespeed;
			break;
		case RIGHT:
			m_collisionbox.mapPosX += movespeed;
			mapPosX += movespeed;
			break;
		case UP:
			m_collisionbox.mapPosY -= movespeed;
			mapPosY -= movespeed;
			break;
		case DOWN:
			m_collisionbox.mapPosY += movespeed;
			mapPosY += movespeed;
			break;
		}

		return nullptr;
	}

	if(getMapPosX() + TILESIZE > lastUpdatePositionX  + collisionbox_Update_Range || getMapPosX() + TILESIZE < lastUpdatePositionX - collisionbox_Update_Range)
	{
		update_Collisionbox_List();
	}
	else if(getMapPosY() + TILESIZE > lastUpdatePositionY + collisionbox_Update_Range || getMapPosY() + TILESIZE < lastUpdatePositionY - collisionbox_Update_Range)
	{
		update_Collisionbox_List();
	}

	if(!canMove) return nullptr; 

	bool isColliding = false;
	collisionbox* collision = nullptr;

	lastMovingDir = dir;

	switch(dir)
	{
	case LEFT:
	{
		m_collisionbox.mapPosX -= movespeed;
		collision = Collision_System::getMe().isCollide(collisionbox_List,&m_collisionbox);

		if(collision->type == None_Collisionbox)
		{
			mapPosX -= movespeed;
		}
		else
		{
			isColliding = true;
			m_collisionbox.mapPosX += movespeed;
		}

	break;
	}
	case RIGHT:
	{
		m_collisionbox.mapPosX += movespeed;
		collision = Collision_System::getMe().isCollide(collisionbox_List,&m_collisionbox);
	
		if(collision->type == None_Collisionbox)
		{
			mapPosX += movespeed;
		}
		else
		{
			isColliding = true;
			m_collisionbox.mapPosX -= movespeed;
		}

	break;
	}
	case DOWN:
	{	
		m_collisionbox.mapPosY += movespeed;
		collision = Collision_System::getMe().isCollide(collisionbox_List,&m_collisionbox);

		if(collision->type == None_Collisionbox)
		{
			mapPosY += movespeed;
		}
		else
		{
			isColliding = true;
			m_collisionbox.mapPosY -= movespeed;
		}
		
	break;
	}
	case UP:
	{
		m_collisionbox.mapPosY -= movespeed;
		collision = Collision_System::getMe().isCollide(collisionbox_List,&m_collisionbox);

		if(collision->type == None_Collisionbox)
		{
			mapPosY -= movespeed;
		}
		else
		{
			isColliding = true;
			m_collisionbox.mapPosY += movespeed;
		}

	break;
	}
	}

	if(isColliding) teleport_Out_Of_Collisionbox();

	return collision;
}

int moveable::update_Collisionbox_List()
{
	if(!canCollide) return 0;

	Collision_System::getMe().get_colliding_Collisionboxes(collisionbox_List,getPosX()+TILESIZE-(collisionbox_Update_Range + ZOOMSIZE),getPosY()+TILESIZE-(collisionbox_Update_Range + ZOOMSIZE),(collisionbox_Update_Range + ZOOMSIZE)*2,(collisionbox_Update_Range + ZOOMSIZE)*2);
	lastUpdatePositionX = getMapPosX() + TILESIZE;
	lastUpdatePositionY = getMapPosY() + TILESIZE;

	return collisionbox_List.size();
}

void moveable::clear_Collisionbox_List()
{
	collisionbox_List.clear();
}

void moveable::teleport_Out_Of_Collisionbox()
{
	if(!canCollide) return;

	int tries = 0;

	while(Collision_System::getMe().isCollide(collisionbox_List,&m_collisionbox)->type != None_Collisionbox && tries <= 100)
	{
		switch(lastMovingDir)
		{
		case LEFT:
			mapPosX++;
			break;
		case RIGHT:
			mapPosX--;
			break;
		case DOWN:
			mapPosY--;
			break;
		case UP:
			mapPosY++;
			break;
		}

		m_collisionbox.mapPosX = getMapPosX();
		m_collisionbox.mapPosY = getMapPosY();

		update_Collisionbox_List();

		tries++;
	}

	if(tries>100)
	{
		FOUT("[CRITICAL] - NPC-Teleportations-Fehler!!!!");
	}
}

bool moveable::tp(int newMapPosX, int newMapPosY)
{
	mapPosX = newMapPosX;
	mapPosY = newMapPosY;

	m_collisionbox.mapPosX = getMapPosX();
	m_collisionbox.mapPosY = getMapPosY();

	if(canCollide) update_Collisionbox_List();

	return true;
}

float moveable::getPosX()
{
	return mapPosX + Map_Manager::getMe().getMapPosX() + cBoxPosX;
}

float moveable::getPosY()
{
	return mapPosY + Map_Manager::getMe().getMapPosY() + cBoxPosY;
}

float moveable::getMapPosX()
{
	return mapPosX + cBoxPosX;
}

float moveable::getMapPosY()
{
	return mapPosY + cBoxPosY;
}

moveable::~moveable(void)
{

}
