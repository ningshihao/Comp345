/*
Team #8

Authors:
Nicholas Burdet
Nathan Lacasse
Shihao Ning
Alexi Grondin

Course: COMP 345

Final Project Delivery

MapScreen header file

*/

#pragma once
#include "character.h"
#include <vector>
#include "dice.h"

//Occupied refers to whether an entity (item/NPC/character) is in the space
//Passable refers to if an entity can occupy or pass through the space
struct Space
{
	bool passable = true;
	bool occupied = false;
};


struct Items
{
	int itemX;
	int itemY;
	int itemID;
	string itemName = "NULL";
};

struct Chests
{
	int chestX;
	int chestY;
	vector<string> items;
};

class MapScreen
{
public:
	MapScreen(void);
	void initMap(int x, int y, string n);
	int getCurrentX();
	int getCurrentY();
	int getStartX();
	int getStartY();
	int getEndX();
	int getEndY();
	void setCurrentX(int x);
	void setCurrentY(int y);
	void setStartX(int x);
	void setStartY(int y);
	void setEndX(int x);
	void setEndY(int y);
	bool isPassable();
	bool isPassable(int x, int y);
	bool isOccupied(int x, int y);
	void setOccupied(int x, int y, bool pass);
	void setPassable(int x, int y, bool pass);
	int getNumberOfDistinctNPCs();
	int getNumberOfNPCs();
	bool checkExit();
	int getMaxX();
	int getMaxY();
	int getId();
	string getName();
	void setId(int i);
	void setName(string n);
	int getCampaignId();
	string getCampaignName();
	void setCampaignId(int i);
	void setCampaignName(string n);
	void saveToFile();
	void loadFromFile(string filename);
	void loadNPCs();
	bool addNPC(int id, int xPos, int yPos, string npcType);
	void removeNPC(int xPos, int yPos);
	character characterTable[10]; //This is a table that holds each individual different type of NPC (for reference)
	character characterEntities[100]; //This is a table of NPCS that exist on the map
	character playerCharacter;
	string getFilename();
	bool npcMovement(int npcID, int destX, int destY);
	void loadPlayerCharacter(string filename);
	string playerAttack(int sX, int sY, string dir, bool fullAttack);
	string npcAttack(int npcID, bool moved);
	void addItem(int x, int y, string itemText);
	void removeItem(int x, int y);
	int getNumberOfItems();
	vector<Items> mapItems;
	vector<Chests> mapChests;
	string viewItems();
	string lootItems(int x, int y);
	bool checkItem(int x, int y);
	string getItem(int x, int y);
	character getCharacter(int x, int y);
	string getPlayerString();
	string viewEnemies();
	void adjustNPCLevel(int levelInc);

private:
	//current X and Y can be used for the player location (?)
	//currently unused, to be used in later iterations
	int currentX;
	int currentY;

	Space spaces[100][100];
	//entrance and exit coordinates
	int startX;
	int startY;
	int endX;
	int endY;
	
	//map size in x and y
	int maxX;
	int maxY;
	
	//Number of NPCs on the map
	int numberOfNPCs = 0;
	int numberOfDistinctNPCs = 0; //This is just for id tracking
	int numberOfRemovedNPCs = 0; //This is a dumb workaround for not refactoring a ton of code

	int numberOfItems = 0;
	int numberOfChests = 0;

	int mapId;
	string mapName;

	//Stores campaignId and Name for backwards referencing, in the case of
	//opening a map, which will prompt the appropriate campaign to load in
	//consequence.
	int campaignId;
	string campaignName;

	
	
};
#pragma once
