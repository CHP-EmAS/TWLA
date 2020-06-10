#pragma once

#include <list>
#include "Defines.h"

class moveable
{
public:

	moveable(void);

	virtual collisionbox* move(int dir);

	int update_Collisionbox_List();
	void clear_Collisionbox_List();
	void teleport_Out_Of_Collisionbox();

	virtual bool tp(int newMapPosX, int newMapPosY);

	float getPosX();
	float getPosY();

	float getMapPosX();
	float getMapPosY();

	~moveable(void);

protected:

	std::list<collisionbox*> collisionbox_List;

	collisionbox m_collisionbox;
	int cBoxPosX;
	int cBoxPosY;

	int collisionbox_Update_Range;
	int movespeed;

	bool canMove;
	bool canCollide;

	short lastMovingDir;

	float mapPosX;
	float mapPosY;
private:
	
	float lastUpdatePositionX;
	float lastUpdatePositionY;
};

