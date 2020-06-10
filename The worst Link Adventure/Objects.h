#pragma once
#include "InteractivObject.h"

class BreakAbyss : public InteractivObject
{
public:
	BreakAbyss(int x, int y,short ID);

	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~BreakAbyss(void);
private:
	bool broken;
	sf::Clock breakClock;
};

class Bush : public InteractivObject
{
public:
	Bush(int x, int y,short ID,short type);

	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	void drop();

	~Bush(void);
private:
	bool broken;
	short type;
};

class Button : public InteractivObject
{
public:
	Button(int x, int y,short ID, std::string pressCommand, std::string releaseCommand, bool pressed, int type);

	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Button(void);
private:
	std::string pressCommand;
	std::string releaseCommand;

	bool pressed;
	int type;

	sf::Clock idleClock;
};

class Cactus : public InteractivObject
{
public:
	Cactus(int x, int y,short ID,float damage);

	void interact(interactType interact_Type);
	void draw(sf::RenderWindow &mainWindow,float x,float y);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Cactus(void);
private:
	float damage;
	float live;
};

class Chest : public InteractivObject
{
public:
	Chest(int x, int y,short ID, int keyItemNumber,int itemNumber);

	void interact(interactType interact_Type);
	void open(int key);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Chest(void);
private:
	int keyNumber;
	bool isOpen;
	int item;
};

class CommandDoor : public InteractivObject
{
public:
	CommandDoor(int x, int y,short ID,std::list<std::string> requiredCommands,std::list<std::string> checkedCommands);
	void interact(interactType interact_Type);

	void executeObjectCommand(std::string command);

	void open();
	void close();

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~CommandDoor(void);
private:
	std::map<std::string,bool> requiredCommands;
};

class Door : public InteractivObject
{
public:
	Door(int x, int y,short ID, int keyNumber, std::string portWorld, int portSpawnPointNumber);

	void interact(interactType interact_Type);
	void open(int key);

	~Door(void);
private:
	int keyNumber;
	bool isOpen;

	std::string portWorld;
	int portSpawnPointNumber;
};

class FragmentInserter : public InteractivObject
{
public:
	FragmentInserter(int x, int y,short ID);
	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~FragmentInserter(void);
private:
	bool inserted;
};

class GlobalWall : public InteractivObject
{
public:
	GlobalWall(int x, int y,short ID,bool open);
	void interact(interactType interact_Type);

	void executeObjectCommand(std::string command);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	void setOpen(bool open);

	~GlobalWall(void);
private:
	bool open;
};

class Grabable_Tile : public InteractivObject
{
public:
	Grabable_Tile(int posX, int posY,short ID, Graphic_Manager::tileset tileset, int tile);

	void interact(interactType interact_Type);

	~Grabable_Tile(void);
private:

};

class Jar : public InteractivObject
{
public:
	Jar(int x, int y,short ID,int item);

	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Jar(void);
private:
	bool broken;
	int item;
};

class KeyStone : public InteractivObject
{
public:
	KeyStone(int x, int y,short ID,short type,int keyNumber);
	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~KeyStone(void);
private:
	short type;
	bool open;

	int keyNumber;
};

class Portal : public InteractivObject
{
public:
	Portal(int x, int y,short ID, std::string portWorld, int portSpawnPointNumber);

	void draw(sf::RenderWindow &mainWindow);
	void interact(interactType interact_Type);
	void teleport();

	~Portal(void);
private:

	std::string portWorld;
	int portSpawnPointNumber;
};

class Secret_Bush : public InteractivObject
{
public:
	Secret_Bush(int x, int y,short ID, std::string portWorld, int portSpawnPointNumber);

	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Secret_Bush(void);
private:
	std::string portWorld;
	int portSpawnPointNumber;

	bool broken;
};

class Sign : public InteractivObject
{
public:
	Sign(int x, int y,short ID, std::string signMessage,sf::Color textColor,short type);

	void interact(interactType interact_Type);

	~Sign(void);
private:
	std::string signMessage;
	sf::Color textColor;
};

class Stone : public InteractivObject
{
public:
	Stone(int x, int y,short ID, short type);

	void interact(interactType interact_Type);

	std::string getMySaveData();
	bool loadMySaveData(std::string data);

	~Stone(void);
private:
	short type;
	bool broken;
};

class Teleporter : public InteractivObject
{
public:
	Teleporter(int x, int y,short ID, int portSpawnPointNumber);

	void draw(sf::RenderWindow &mainWindow);
	void interact(interactType interact_Type);
	void teleport();

	~Teleporter(void);
private:

	int portSpawnPointNumber;
};

class Trigger : public InteractivObject
{
public:
	Trigger(int x, int y,short ID, std::list<std::string> commands, bool multiTrigger, interactType trigger_Type, std::string hintMsg);

	void interact(interactType interact_Type);

	~Trigger(void);
private:
	std::list<std::string> commands;
	bool multiTrigger;

	interactType triggerType;
	std::string hintMsg;
};