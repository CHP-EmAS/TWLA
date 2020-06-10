#pragma once

#include "Defines.h"

#include "InteractivObject.h"


class Collision_System : public Singleton<Collision_System>
{

friend class Singleton<Collision_System>;

public:
	/*Prüft ob ein Bereich oder eine Hitbox mit einer anderen Hitbox kollidiert.
	Die erste Hitbox die kollidiert wird zurück gegeben*/
	collisionbox *isCollide(int posX, int posY, int width, int height);
	collisionbox *isCollide(const collisionbox* cBox);

	/*Prüft ob ein Bereich oder eine Hitbox mit einer anderen Hitbox aus der cBoxList Liste kollidiert.
	Die erste Hitbox die kollidiert wird zurück gegeben*/
	collisionbox *isCollide(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height);
	collisionbox *isCollide(std::list<collisionbox*> &cBoxList,const collisionbox* cBox);
	
	//Prüft eine Kollision zwischen zwei Hitboxen
	bool boxCollision(collisionbox *box1,collisionbox *box2);
	
	//Prüft ob ein Quadrat mit einem anderen Quadrat überlappt
	bool intersection(sf::IntRect rect1,sf::IntRect rect2);

	//Speichert alle Hitboxen in die Liste cBoxList, die mit einem Bererich kollidieren
	void get_colliding_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height);
	
	//Speichert alle normalen Hitboxen in die Liste cBoxList, die mit einem Bererich kollidieren
	void get_colliding_Normal_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height);
	
	//Speichert alle Objekt-Hitboxen(Spieler, NPC, Objekte) in die Liste cBoxList, die mit einem Bererich kollidieren
	void get_colliding_Interactiv_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height);
	
	//Speichert alle Objekt-Hitboxen(Spieler, NPC, Objekte) in die Liste cBoxList, die mit einer anderen Hitbox kollidieren
	void get_colliding_Interactiv_Collisionboxes(std::list<collisionbox*> &cBoxList,const collisionbox* cBox);
	
	//Speichert alle NPC-Hitboxen in die Liste cBoxList, die mit einem Bererich kollidieren
	void get_colliding_NPC_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height);
	
	//Speichert alle Interaktive-Objekt-Hitboxen in die Liste cBoxList, die mit einem Bererich kollidieren
	void get_colliding_Obj_Collisionboxes(std::list<collisionbox*> &cBoxList,int posX, int posY, int width, int height);

	//gibt die Kollisionsabfragen pro Sekunde zurück
	int getCollisionCounter();
	
	//startet die Kollisionsabfragen pro Sekunde neu
	void resetCollisionCounter();
	
	//Berechnet den Abstand zwischen zwei Positionen
	int distance(sf::Vector2i pos1,sf::Vector2i pos2);
	float distance(sf::Vector2f pos1,sf::Vector2f pos2);

	~Collision_System(void);
private:
	Collision_System(void);

	collisionbox nullCollisionbox;
	int collisionCheckCounter;
};

