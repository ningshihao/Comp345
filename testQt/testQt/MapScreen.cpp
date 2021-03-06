/*
Team #8

Authors:
Nicholas Burdet
Nathan Lacasse
Shihao Ning
Alexi Grondin

Course: COMP 345

Final Project Delivery

Map Screen cpp file
*/


#include <fstream>
#include "MapScreen.h"
#include <QDebug>
#include <string>
#include <iostream>
#include <math.h>
#include <sstream>
using namespace std;

//This structure, Search, is used for the searching of the map to discover
//if a path from start to end exists.
struct Search
{
	bool northSearched = false;
	bool eastSearched = false;
	bool southSearched = false;
	bool westSearched = false;
	string spaceCameFrom = "";
	bool explored = false;
	bool deadEnd = false;
};

MapScreen::MapScreen(void)
{

}
//For the matter of simplicity of coding and not using vectors,
//we will assume map sizes cannot exceed 100x100.
void MapScreen::initMap(int x, int y, string n)
{
	//error checking not done for size constraints
	if (x < 1 || x>100 || y < 1 || y>100)
	{
		cout << "Error in Map Screen size generation";
		exit(1);
	}
	else
	{
		mapName = n;
		maxX = x;
		maxY = y;
		currentX = 0;
		currentY = 0;
		startX = 0;
		startY = 0;
		endX = x-1;
		endY = y-1;
	}
}

int MapScreen::getCurrentX()
{
	return currentX;
};

int MapScreen::getCurrentY()
{
	return currentY;
};

int MapScreen::getStartX()
{
	return startX;
};

int MapScreen::getStartY()
{
	return startY;
};

int MapScreen::getEndX()
{
	return endX;
};

int MapScreen::getEndY()
{
	return endY;
};

int MapScreen::getMaxX()
{
	return maxX;
};

int MapScreen::getMaxY()
{
	return maxY;
};

void MapScreen::setCurrentX(int x)
{
	currentX = x;
};

void MapScreen::setCurrentY(int y)
{
	currentY = y;
};

void MapScreen::setStartX(int x)
{
	startX = x;
};

void MapScreen::setStartY(int y)
{
	startY = y;
};

void MapScreen::setEndX(int x)
{
	endX = x;
};

void MapScreen::setEndY(int y)
{
	endY = y;
};

bool MapScreen::isPassable()
{
	return spaces[currentX][currentY].passable;
};

bool MapScreen::isPassable(int x, int y)
{
	return spaces[x][y].passable;
};

void MapScreen::setPassable(int x, int y, bool pass)
{
	spaces[x][y].passable = pass;
};

bool MapScreen::isOccupied(int x, int y)
{
	return spaces[x][y].occupied;
};

void MapScreen::setOccupied(int x, int y, bool pass)
{
	spaces[x][y].occupied = pass;
};

int MapScreen::getNumberOfDistinctNPCs()
{
	return numberOfDistinctNPCs;
};

int MapScreen::getNumberOfNPCs()
{
	return numberOfNPCs;
};

int MapScreen::getId()
{
	return mapId;
}

string MapScreen::getName()
{
	return mapName;
}

void MapScreen::setId(int i)
{
	mapId = i;
}

void MapScreen::setName(string n)
{
	mapName = n;
}

int MapScreen::getCampaignId()
{
	return campaignId;
}

string MapScreen::getCampaignName()
{
	return campaignName;
}

void MapScreen::setCampaignId(int i)
{
	campaignId = i;
}

void MapScreen::setCampaignName(string n)
{
	campaignName = n;
}
/*************************************************************************************************************
Basic explantion for how the checkExit algorithm works.
Using a depth first algorithm, the code will establish a search array (of type search) that
will allow us to keep track of where we have searched in the map, where we have come from, etc.

The algorithm checks goes (in this order) north, west, south or east to check if a passable space exists.

If a space exists, the search algorithm moves to that space and keeps checking, while leaving behind in that
array space a trail of where it came from, so it knows which was to go back when it hits a dead end.

If the search comes to a space, and excluding the direction it came in, it finds that all 3 other directions
have either been visited OR are not passable, it marks the current space as a dead end and
the search goes back to the previous space in the trail.

If the search comes back to the beginning and finds that all 4 directions do not yield a suitable path, the
algorithm returns false.

Once the algorithm finds the exit space, it returns true and stops looking.

NOTE: This will not ensure that the fastest path is discovered. Only that at least 1 path exists.

There are more fine details about the execution of this algorithm, but the above explanation is enough to
get the jist of it.
***************************************************************************************************************/
bool MapScreen::checkExit()
{
	bool endFound = false;
	bool searching = true;
	int cX = startX;
	int cY = startY;
	Search searchArray[100][100];

	while (endFound == false)
	{
		searching = true;
		if ((searchArray[cX][cY].northSearched == false) && (searching == true) && (searchArray[cX][cY].spaceCameFrom != "North"))
		{
			//Check if north space exists
			if ((cY - 1) >= 0)
			{
				//Check if north space is passable or hasn't been explored
				if ((this->isPassable(cX, (cY - 1))) && (searchArray[cX][cY - 1].explored == false) && (searchArray[cX][cY - 1].deadEnd == false))
				{
					if (this->getEndX() == cX && this->getEndY() == cY - 1)
					{
						endFound = true;
					}
					searchArray[cX][cY].explored = true;
					searchArray[cX][cY].northSearched = true;
					cY--;
					searchArray[cX][cY].spaceCameFrom = "South";
					searching = false;
				}
				else
				{
					searchArray[cX][cY].northSearched = true;
				}

			}
			else
			{
				searchArray[cX][cY].northSearched = true;
			}
		}
		if ((searchArray[cX][cY].westSearched == false) && (searching == true) && (searchArray[cX][cY].spaceCameFrom != "West"))
		{
			//Check if east space exists
			if ((cX - 1) >= 0)
			{
				//Check if east space is passable or explored
				if ((this->isPassable((cX - 1), cY)) && (searchArray[cX - 1][cY].explored == false) && (searchArray[cX - 1][cY].deadEnd == false))
				{
					if (this->getEndX() == cX - 1 && this->getEndY() == cY)
					{
						endFound = true;
					}
					searchArray[cX][cY].explored = true;
					searchArray[cX][cY].westSearched = true;
					cX--;
					searchArray[cX][cY].spaceCameFrom = "East";
					searching = false;
				}
				else
				{
					searchArray[cX][cY].westSearched = true;
				}

			}
			else
			{
				searchArray[cX][cY].westSearched = true;
			}
		}
		if ((searchArray[cX][cY].southSearched == false) && (searching == true) && (searchArray[cX][cY].spaceCameFrom != "South"))
		{
			//Check if east space exists
			if ((cY + 1) <= maxY - 1)
			{
				//Check if east space is passable or explored
				if ((this->isPassable(cX, (cY + 1))) && (searchArray[cX][cY + 1].explored == false) && (searchArray[cX][cY + 1].deadEnd == false))
				{
					if (this->getEndX() == cX && this->getEndY() == cY + 1)
					{
						endFound = true;
					}
					searchArray[cX][cY].explored = true;
					searchArray[cX][cY].southSearched = true;
					cY++;
					searchArray[cX][cY].spaceCameFrom = "North";
					searching = false;
				}
				else
				{
					searchArray[cX][cY].southSearched = true;
				}

			}
			else
			{
				searchArray[cX][cY].southSearched = true;
			}
		}
		if ((searchArray[cX][cY].eastSearched == false) && (searching == true) && (searchArray[cX][cY].spaceCameFrom != "East"))
		{
			//Check if east space exists
			if ((cX + 1) <= maxX - 1)
			{
				//Check if east space is passable
				if ((this->isPassable((cX + 1), cY)) && (searchArray[cX + 1][cY].explored == false) && (searchArray[cX + 1][cY].deadEnd == false))
				{
					if (this->getEndX() == cX + 1 && this->getEndY() == cY)
					{
						endFound = true;
					}
					searchArray[cX][cY].explored = true;
					searchArray[cX][cY].eastSearched = true;
					cX++;
					searchArray[cX][cY].spaceCameFrom = "West";
					searching = false;
				}
				else
				{
					searchArray[cX][cY].eastSearched = true;
				}

			}
			else
			{
				searchArray[cX][cY].eastSearched = true;
			}
		}
		if (searching == true)
		{
			if (searchArray[cX][cY].spaceCameFrom.compare("North") == 0)
			{
				searchArray[cX][cY].deadEnd = true;
				cY--;
			}
			else if (searchArray[cX][cY].spaceCameFrom.compare("East") == 0)
			{
				searchArray[cX][cY].deadEnd = true;
				cX++;
			}
			else if (searchArray[cX][cY].spaceCameFrom.compare("South") == 0)
			{
				searchArray[cX][cY].deadEnd = true;
				cY++;
			}
			else if (searchArray[cX][cY].spaceCameFrom.compare("West") == 0)
			{
				searchArray[cX][cY].deadEnd = true;
				cX--;
			}
			else
			{
				return false;
			}
		}
	}
	return true;
};

void MapScreen::saveToFile()
{
	ofstream output;
	string extension = "Maps/";
	string fName = extension;
	fName.append(campaignName);
	fName.append(to_string(campaignId));
	fName.append("m");
	fName.append(to_string(mapId));
	fName.append(mapName);

	output.open(fName.append(".txt"));
	
	output << "campaignName" << " " << campaignName << " " << "campaignId" << " " << campaignId << " " << "mapId" << " " << mapId << " " << "mapName" << " " << mapName << endl;
	output << "currentX" << " " << currentX << endl;
	output << "currentY" << " " << currentY << endl;
	output << "startX" << " " << startX << endl;
	output << "startY" << " " << startY << endl;
	output << "endX" << " " << endX << endl;
	output << "endY" << " " << endY << endl;
	output << "maxX" << " " << maxX << endl;
	output << "maxY" << " " << maxY << endl;

	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			output << "space" << " " << x << " " << y << " " << spaces[x][y].passable << endl;
		}
	}

	output << "numberOfNPCs" << " " << numberOfNPCs << endl;

	int count = 0;
	int index = 0;
	while (count < numberOfNPCs)
	{
		if (characterEntities[index].getName() != "NULL")
		{
			output << "npcID" << " " << characterEntities[index].getId() << " " << "xPos" << " " << characterEntities[index].getX() << " " << "yPos" << " " << characterEntities[index].getY() << " " << characterEntities[index].getType() << endl;
			count++;
		}
		index++;
	}

	output << "numberOfItems" << " " << numberOfItems << endl;

	count = 0;
	index = 0;
	while (count < numberOfItems)
	{
		if (mapItems[index].itemName != "NULL")
		{
			output << "itemX" << " " << mapItems[index].itemX << " " << "itemY" << " " << mapItems[index].itemY << " " << "itemID" << " " << mapItems[index].itemID << endl; 
			count++;
		}
		index++;
	}

	output.close();
}


void MapScreen::loadFromFile(string filename)
{
	vector<string> itemsFromFile;

	ifstream itemInput("Resources/items.txt");

	for (string itemline; getline(itemInput, itemline);)
	{
		itemsFromFile.push_back(itemline);
	}

	itemInput.close();

	ifstream input;
	input.open(filename, ios::in);

	string type;

	input >> type >> campaignName >> type >> campaignId >> type >> mapId >> type >> mapName;
	input >> type >> currentX;
	input >> type >> currentY;
	input >> type >> startX;
	input >> type >> startY;
	input >> type >> endX;
	input >> type >> endY;
	input >> type >> maxX;
	input >> type >> maxY;

	int spaceX, spaceY;
	int pass;
	for (int x = 0; x < maxX; x++)
	{
		for (int y = 0; y < maxY; y++)
		{
			input >> type >> spaceX >> spaceY >> pass;
			if (pass == 1)
			{
				spaces[spaceX][spaceY].passable = true;
			}
			else
			{
				spaces[spaceX][spaceY].passable = false;
			}
		}
	}

	input >> type >> numberOfNPCs;

	int i, xP, yP;

	string nature;

	for (int x = 0; x < numberOfNPCs; x++)
	{
		input >> type >> i >> type >> xP >> type >> yP >> nature;
		characterEntities[x] = characterTable[i];
		characterEntities[x].setX(xP);
		characterEntities[x].setY(yP);
		characterEntities[x].setType(nature); //This can be changed later
		spaces[xP][yP].occupied = true;
		characterEntities[x].npcUniqueID = x;
	}

	
	input >> type >> i;
	string itemText;
	int itemID;
	
	for (int x = 0; x < i; x++)
	{
		input >> type >> xP >> type >> yP >> type >> itemID;
		itemText = itemsFromFile[itemID];
		addItem(xP, yP, itemText);
	}
	
	input.close();
}

//This method will load all DISTINCT NPCs into an identity table, so in the case of
//adding or retrieving them, it won't go into the .txt file every time.
void MapScreen::loadNPCs(void)
{
	vector<string> itemsFromFile;
	ifstream itemInput("Resources/items.txt");

	for (string itemline; getline(itemInput, itemline);)
	{
		itemsFromFile.push_back(itemline);
	}

	itemInput.close();
	ifstream input("npc.txt");

	string type;

	int id;
	string name;
	int level;
	string image;
	string subtype;

	int str;
	int dex;
	int con;
	int itl;
	int wis;
	int cha;

	int wdice;
	int wdam;
	int dbon;
	int abon;
	int ran;

	int abilList[6];
	int itemNum;
	string itemList;

	input >> type >> id >> type >> name >> type >> level >> type >> image >> subtype >> type >> str >> type >> dex >> type >> con >> type >> itl >> type >> wis >> type >> cha >> type >> wdice >> type >> wdam >> type >> dbon >> type >> abon >> type >> ran >> type >> itemNum >> itemList;
	abilList[0] = str;
	abilList[1] = itl;
	abilList[2] = wis;
	abilList[3] = dex;
	abilList[4] = con;
	abilList[5] = cha;
	if (!input.eof())
	{
		characterTable[id] = character(id, name, level, image, subtype, abilList);
		characterTable[id].setNumberOfWeaponDice(wdice);
		characterTable[id].setWeaponDice(wdam);
		characterTable[id].setWeaponDamageBonus(dbon);
		characterTable[id].setWeaponAttackBonus(abon);
		characterTable[id].setWeaponRange(ran);

		std::istringstream idS(itemList);
		string sID;
		int iID;
		string iText = "";
		for (int a = 0; a < itemNum; a++)
		{
			std::getline(idS, sID, '|');
			iID = atoi(sID.c_str());
			iText = itemsFromFile[iID];
			iText.erase(std::remove(iText.begin(), iText.end(), '\r'), iText.end());
			iText.erase(std::remove(iText.begin(), iText.end(), '\n'), iText.end());
			characterTable[id].addNpcItem(iText);
		}
		numberOfDistinctNPCs++;
	}

	while (!input.eof())
	{
		input >> type >> id >> type >> name >> type >> level >> type >> image >> subtype >> type >> str >> type >> dex >> type >> con >> type >> itl >> type >> wis >> type >> cha >> type >> wdice >> type >> wdam >> type >> dbon >> type >> abon >> type >> ran >> type >> itemNum >> itemList;
		abilList[0] = str;
		abilList[1] = itl;
		abilList[2] = wis;
		abilList[3] = dex;
		abilList[4] = con;
		abilList[5] = cha;
		characterTable[id] = character(id, name, level, image, subtype, abilList);
		characterTable[id].setNumberOfWeaponDice(wdice);
		characterTable[id].setWeaponDice(wdam);
		characterTable[id].setWeaponDamageBonus(dbon);
		characterTable[id].setWeaponAttackBonus(abon);
		characterTable[id].setWeaponRange(ran);

		std::istringstream idS(itemList);
		string sID;
		int iID;
		string iText = "";
		for (int a = 0; a < itemNum; a++)
		{
			std::getline(idS, sID, '|');
			iID = atoi(sID.c_str());
			iText = itemsFromFile[iID];
			iText.erase(std::remove(iText.begin(), iText.end(), '\r'), iText.end());
			iText.erase(std::remove(iText.begin(), iText.end(), '\n'), iText.end());
			characterTable[id].addNpcItem(iText);
		}
		numberOfDistinctNPCs++;
	}

	input.close();
}

//This method adds an NPC onto the map, with its ID and position and adds them
//to the entity table
//NOTE: This is not for creating new NPCs
bool MapScreen::addNPC(int id, int xPos, int yPos, string npcType)
{
	if (numberOfNPCs >= 100)
	{
		return false;
	}
	numberOfNPCs++;

	int i = 0;
	while (characterEntities[i].getName() != "NULL")
	{
		i++;
	}

	characterEntities[i] = characterTable[id];
	characterEntities[i].setX(xPos);
	characterEntities[i].setY(yPos);
	characterEntities[i].setType(npcType);
	spaces[xPos][yPos].occupied = true;

	return true;
}

//This method removes the NPC at the location on the map given and
//removes them from the entity table. This method is only called if
//the widget wants to change the contents of the space containing an NPC
void MapScreen::removeNPC(int xPos, int yPos)
{
	int index = 0;
	int count = 0;
	bool found = false;


	for(int i = 0; i < numberOfNPCs+numberOfRemovedNPCs; i++)
	{
		if (characterEntities[i].getName() != "NULL")
		{
			if (characterEntities[i].getX() == xPos && characterEntities[i].getY() == yPos)
			{
				found = true;
				characterEntities[i] = character(); //Fills with empty character
			}
		}
	}

	if (found)
	{
		numberOfRemovedNPCs++;
		numberOfNPCs--;
		spaces[xPos][yPos].occupied = false;
	}
	if (numberOfNPCs < 0)
	{
		numberOfNPCs = 0; //Potentially useless
	}
}

//Generates the filename for this map to pass along to campaign or whatnot
string MapScreen::getFilename()
{
	string fName;
	fName = campaignName;
	fName.append(to_string(campaignId));
	fName.append("m");
	fName.append(to_string(mapId));
	fName.append(mapName);
	fName.append(".txt");

	return fName;
}

//with the npcID (their position in the array, not their distinct id) you can give a point
//On the map (player or otherwise) and npcs will do their best to move as close to it as possible.
bool MapScreen::npcMovement(int npcID, int destX, int destY)
{
	int beginX = characterEntities[npcID].getX();
	int beginY = characterEntities[npcID].getY();

	if (characterEntities[npcID].getName() != "NULL")
	{
		int npcX = characterEntities[npcID].getX();
		int npcY = characterEntities[npcID].getY();

		struct moveArray
		{
			int xSpace;
			int ySpace;
			int stepsTaken = -1;
			bool checked = false;
			double distance = 1000;
		};

		vector<moveArray> pathfinding;

		moveArray mv;
		moveArray checkMoves[100][100];
		mv.xSpace = npcX;
		mv.ySpace = npcY;
		mv.stepsTaken = 0;

		checkMoves[npcX][npcY].stepsTaken = 0;
		pathfinding.push_back(mv);
		//Goes through each step
		for (int i = 1; i <= characterEntities[npcID].getMoveSpeed(); i++)
		{
			//Goes through the pathfinding array
			for (int index = 0; index < pathfinding.size(); index++)
			{
				//If it sees a space with current steps - 1, we will check the adjacent squares if it is -1
				if (pathfinding[index].stepsTaken == (i - 1))
				{
					//up
					if (!(spaces[pathfinding[index].xSpace][pathfinding[index].ySpace - 1].occupied) && spaces[pathfinding[index].xSpace][pathfinding[index].ySpace - 1].passable && (pathfinding[index].ySpace) > 0)
					{
						if (checkMoves[pathfinding[index].xSpace][pathfinding[index].ySpace - 1].stepsTaken < 0)
						{
							checkMoves[pathfinding[index].xSpace][pathfinding[index].ySpace-1].stepsTaken = i;
							mv.xSpace = pathfinding[index].xSpace;
							mv.ySpace = pathfinding[index].ySpace - 1;
							mv.stepsTaken = i;
							pathfinding.push_back(mv);
						}
					}
					
					//down
					if (!(spaces[pathfinding[index].xSpace][pathfinding[index].ySpace + 1].occupied) && spaces[pathfinding[index].xSpace][pathfinding[index].ySpace + 1].passable && (pathfinding[index].ySpace) < (maxY - 1))
					{
						if (checkMoves[pathfinding[index].xSpace][pathfinding[index].ySpace + 1].stepsTaken < 0)
						{
							checkMoves[pathfinding[index].xSpace][pathfinding[index].ySpace+1].stepsTaken = i;
							mv.xSpace = pathfinding[index].xSpace;
							mv.ySpace = pathfinding[index].ySpace + 1;
							mv.stepsTaken = i;
							pathfinding.push_back(mv);
						}
					}
					
					//left
					if (!(spaces[pathfinding[index].xSpace - 1][pathfinding[index].ySpace].occupied) && spaces[pathfinding[index].xSpace - 1][pathfinding[index].ySpace].passable && (pathfinding[index].xSpace) > 0)
					{
						if (checkMoves[pathfinding[index].xSpace-1][pathfinding[index].ySpace].stepsTaken < 0)
						{
							checkMoves[pathfinding[index].xSpace-1][pathfinding[index].ySpace].stepsTaken = i;
							mv.xSpace = pathfinding[index].xSpace - 1;
							mv.ySpace = pathfinding[index].ySpace;
							mv.stepsTaken = i;
							pathfinding.push_back(mv);
						}
					}

					//right
					if (!(spaces[pathfinding[index].xSpace + 1][pathfinding[index].ySpace].occupied) && spaces[pathfinding[index].xSpace + 1][pathfinding[index].ySpace].passable && (pathfinding[index].xSpace) < (maxX - 1))
					{
						if (checkMoves[pathfinding[index].xSpace + 1][pathfinding[index].ySpace].stepsTaken < 0)
						{
							checkMoves[pathfinding[index].xSpace + 1][pathfinding[index].ySpace].stepsTaken = i;
							mv.xSpace = pathfinding[index].xSpace + 1;
							mv.ySpace = pathfinding[index].ySpace;
							mv.stepsTaken = i;
							pathfinding.push_back(mv);
						}
					}
				}
			}
		}

		moveArray closestSpace;
		closestSpace.stepsTaken = 1000;
		//Now to determine which space is closest to the player
		for (int index = 0; index < pathfinding.size(); index++)
		{
			double length = sqrt((destX - pathfinding[index].xSpace)*(destX - pathfinding[index].xSpace) + (destY - pathfinding[index].ySpace)*(destY - pathfinding[index].ySpace));
			
			if (length < closestSpace.distance && length != 0)
			{
				closestSpace.stepsTaken = pathfinding[index].stepsTaken;
				closestSpace.xSpace = pathfinding[index].xSpace;
				closestSpace.ySpace = pathfinding[index].ySpace;
				closestSpace.distance = length;
			}
			else if(length == closestSpace.distance)
			{
				if (pathfinding[index].stepsTaken < closestSpace.stepsTaken)
				{
					closestSpace.stepsTaken = pathfinding[index].stepsTaken;
					closestSpace.xSpace = pathfinding[index].xSpace;
					closestSpace.ySpace = pathfinding[index].ySpace;
				}
			}
		}

		spaces[characterEntities[npcID].getX()][characterEntities[npcID].getY()].occupied = false;
		spaces[closestSpace.xSpace][closestSpace.ySpace].occupied = true;
		characterEntities[npcID].moveTo(closestSpace.xSpace, closestSpace.ySpace);
		if (beginX == characterEntities[npcID].getX() && beginY == characterEntities[npcID].getY())
		{
			return false; //did not move
		}
		else
		{
			return true; //moved
		}
	}

	return false;
}

void MapScreen::loadPlayerCharacter(string filename)
{
	playerCharacter.loadFromFile(filename);
}

//Function for determining attack, it passes the direction and the (starting) point
//at which the space the player is attacking (not the space the player is standing on)
string MapScreen::playerAttack(int sX, int sY, string dir, bool fullAttack)
{
	string combatText = "";

	int foundNPC = -1;
	int numberOfAttacks = 1;
	vector<int> attackBonuses;
	int damageRoll;
	int weaponDice = playerCharacter.getWeaponDice();
	int numberOfWeaponDice = playerCharacter.getNumberOfWeaponDice();
	int damageDone = 0;
	int damageModifier = playerCharacter.getDamageBonus();

	if (fullAttack)
	{
		numberOfAttacks = playerCharacter.getAttackPerRound();
	}
	attackBonuses = playerCharacter.getAttackBonusList();

	int weaponRange = playerCharacter.getWeaponRange();
	bool npcFound = false;
	bool rangeError = false;
	int rangeCheck = 1;

	while (!npcFound && rangeCheck <= weaponRange && !rangeError)
	{
		for (int i = 0; i < numberOfNPCs+numberOfRemovedNPCs; i++)
		{
			if (characterEntities[i].getX() == sX && characterEntities[i].getY() == sY)
			{
				foundNPC = i;
				npcFound = true;
			}
		}

		if (spaces[sX][sY].passable == false)
		{
			foundNPC = -2;
			rangeError = true;
		}

		if (weaponRange > 1)
		{
			if (dir == "up")
			{
				sY--;
				if (sY < 0)
				{
					rangeError = true;
				}
			}
			else if (dir == "down")
			{
				sY++;
				if (sY > maxY-1)
				{
					rangeError = true;
				}
			}
			else if (dir == "left")
			{
				sX--;
				if (sX < 0)
				{
					rangeError = true;
				}
			}
			else if (dir == "right")
			{
				sX++;
				if (sX > maxX-1)
				{
					rangeError = true;
				}
			}
		}
		
		rangeCheck++;
	}
	if (foundNPC == -1)
	{
		return "You attack into an empty space! Nothing happens.";
	}
	else if (foundNPC == -2)
	{
		return "You attack a wall! Nothing happens.";
	}
	else
	{
		//Combat rolls and stuff goes here
		for (int a = 0; a < numberOfAttacks; a++)
		{
			int roll = Dice::roll(1, 20, 0);
			combatText.append(std::to_string(roll));
			damageRoll = (Dice::roll(numberOfWeaponDice, weaponDice, damageModifier));
			if (roll == 1)
			{
				combatText.append(" Crit miss!");
			}
			else if (roll == 20)
			{
				combatText.append(" Crit! ");
				damageRoll = damageRoll * 2;
				damageDone =+ damageRoll;
				combatText.append(std::to_string(damageRoll)).append("dmg! ");
				//Calculate damage here
				characterEntities[foundNPC].takeDamage(damageDone);
				characterEntities[foundNPC].damageTaken = true;
			}
			else
			{
				if ((roll + attackBonuses[a] > characterEntities[foundNPC].getArmorBonus()))
				{
					combatText.append("+");
					combatText.append(std::to_string(attackBonuses[a]));
					combatText.append(" Hit ");
					combatText.append(std::to_string(damageRoll)).append("dmg! ");
					damageDone = +damageRoll;
					//Calculate damage here
					characterEntities[foundNPC].takeDamage(damageDone);
					characterEntities[foundNPC].damageTaken = true;
				}
				else
				{
					combatText.append("+");
					combatText.append(std::to_string(attackBonuses[a]));
					combatText.append(". Miss!");
				}
			}
		}
		
		//NPCs that are friendly will become hostile when attacked
		if (characterEntities[foundNPC].getType() == "friendly")
		{
			characterEntities[foundNPC].setType("hostile");
			combatText.append(" Enemy is now hostile!");
		}
	}
	return combatText;
}

//Function for determining NPC attack. Similar to PC except it checks around themselves
//instead of a single direction.
string MapScreen::npcAttack(int npcID, bool moved)
{
	string combatText = "Nothing";

	int numberOfAttacks = 1;
	vector<int> attackBonuses;
	int damageRoll;
	int weaponDice = characterEntities[npcID].getWeaponDice();
	int numberOfWeaponDice = characterEntities[npcID].getNumberOfWeaponDice();
	int damageDone = 0;
	int damageModifier = characterEntities[npcID].getDamageBonus();

	if (!moved)
	{
		numberOfAttacks = characterEntities[npcID].getAttackPerRound();
	}

	attackBonuses = characterEntities[npcID].getAttackBonusList();

	int npcX = characterEntities[npcID].getX();
	int npcY = characterEntities[npcID].getY();
	bool attack = false;

	for (int i = 1; i <= characterEntities[npcID].getWeaponRange(); i++)
	{
		//Check up
		if (npcX == currentX && (npcY-i) == currentY)
		{
			attack = true;
		}
		//Check down
		if (npcX == currentX && (npcY+i) == currentY)
		{
			attack = true;
		}
		//Check left
		if ((npcX - i) == currentX && npcY == currentY)
		{
			attack = true;
		}
		//Check right
		if ((npcX + i) == currentX && npcY == currentY)
		{
			attack = true;
		}
	}

	if (attack)
	{
		combatText = "NPC ";
		combatText.append(std::to_string(npcID));
		combatText.append(" attacks!");
		int misses = 0;
		int hits = 0;
		int criticalHits = 0;
		damageRoll = (Dice::roll(numberOfWeaponDice, weaponDice, damageModifier));
		//Combat rolls and stuff goes here
		for (int a = 0; a < numberOfAttacks; a++)
		{
			int roll = Dice::roll(1, 20, 0);
			if (roll == 1)
			{
				misses = misses + 1;
			}
			else if (roll == 20)
			{
				damageRoll = damageRoll * 2;
				damageDone = damageDone + damageRoll;
				criticalHits = criticalHits + 1;
			}
			else
			{
				if ((roll + attackBonuses[a] > playerCharacter.getArmorBonus()))
				{
					damageDone = damageDone + damageRoll;
					hits = hits + 1;
				}
				else
				{
					misses = misses + 1;
				}
			}
		}
		playerCharacter.takeDamage(damageDone);
		combatText.append(" Hits:").append(std::to_string(hits)).append(" Misses:").append(std::to_string(misses)).append(" Crits:").append(std::to_string(criticalHits)).append(" Damage:").append(std::to_string(damageDone)).append("dmg").append(" HP:").append(std::to_string(playerCharacter.getCurrentHP())).append("hp");
	}
	return combatText;
}

void MapScreen::addItem(int x, int y, string itemText)
{
	Items tempItem;
	tempItem.itemX = x;
	tempItem.itemY = y;
	tempItem.itemName = itemText;

	std::istringstream idS(itemText);
	string sID;
	std::getline(idS, sID, '|');

	tempItem.itemID = atoi(sID.c_str());

	setOccupied(x, y, true);

	mapItems.push_back(tempItem);
	numberOfItems++;
}

void MapScreen::removeItem(int x, int y)
{
	int index = -1;
	for (int i = 0; i < mapItems.size(); i++)
	{
		if (mapItems[i].itemX == x && mapItems[i].itemY == y)
		{
			index = i;
		}
	}
	if (index != -1)
	{
		mapItems.erase(mapItems.begin()+index);
		numberOfItems--;
		setOccupied(x, y, false);
	}
}

int MapScreen::getNumberOfItems()
{
	return numberOfItems;
}

string MapScreen::viewItems()
{
	string itemList = "";

	for (int i = 0; i < mapItems.size(); i++)
	{
		itemList.append("X: ");
		itemList.append(std::to_string(mapItems[i].itemX));
		itemList.append(" Y: ");
		itemList.append(std::to_string(mapItems[i].itemY));
		itemList.append(" ItemID: ");
		itemList.append(std::to_string(mapItems[i].itemID));
		itemList.append(" Item: ");
		itemList.append(mapItems[i].itemName);
		itemList.append("\n");
	}
	return itemList;
}

//Check if item exists at X,Y
bool MapScreen::checkItem(int x, int y)
{
	bool found = false;
	for (int i = 0; i < mapItems.size(); i++)
	{
		if (mapItems[i].itemX == x && mapItems[i].itemY == y)
		{
			found = true;
		}
	}
	return found;
}

//Get item at X,Y
string MapScreen::getItem(int x, int y)
{
	string foundItem = "";
	for (int i = 0; i < mapItems.size(); i++)
	{
		if (mapItems[i].itemX == x && mapItems[i].itemY == y)
		{
			foundItem = mapItems[i].itemName;
		}
	}
	return foundItem;
}

character MapScreen::getCharacter(int x, int y)
{
	character c = character();
	for (int i = 0; i < numberOfNPCs + numberOfRemovedNPCs; i++)
	{
		if (characterEntities[i].getX() == x && characterEntities[i].getY() == y)
		{
			return characterEntities[i];
		}
	}
	
	return c;
}

string MapScreen::getPlayerString()
{
	string playerString = "";

	playerString.append("Name: ").append(playerCharacter.getName()).append("\n");
	playerString.append("Level: ").append(std::to_string(playerCharacter.getLevel())).append("\n");
	playerString.append("Class: ").append(playerCharacter.getClassName()).append("\n");
	playerString.append("HP: ").append(std::to_string(playerCharacter.getCurrentHP())).append("/").append(std::to_string(playerCharacter.getHP())).append("\n");
	playerString.append("AC: ").append(std::to_string(playerCharacter.getArmorBonus())).append("\n");
	playerString.append("Strength: ").append(std::to_string(playerCharacter.getAbilities().strength+playerCharacter.currentEquipment.equipStrength)).append(" Mod: ").append(std::to_string(playerCharacter.getModifier(playerCharacter.getAbilities().strength + playerCharacter.currentEquipment.equipStrength))).append(" Equip Bonus: ").append(std::to_string(playerCharacter.currentEquipment.equipStrength)).append("\n");
	playerString.append("Dexterity: ").append(std::to_string(playerCharacter.getAbilities().dexterity + playerCharacter.currentEquipment.equipDexterity)).append(" Mod: ").append(std::to_string(playerCharacter.getModifier(playerCharacter.getAbilities().dexterity + playerCharacter.currentEquipment.equipDexterity))).append(" Equip Bonus: ").append(std::to_string(playerCharacter.currentEquipment.equipDexterity)).append("\n");
	playerString.append("Constitution: ").append(std::to_string(playerCharacter.getAbilities().constitution + playerCharacter.currentEquipment.equipConstitution)).append(" Mod: ").append(std::to_string(playerCharacter.getModifier(playerCharacter.getAbilities().constitution + playerCharacter.currentEquipment.equipConstitution))).append(" Equip Bonus: ").append(std::to_string(playerCharacter.currentEquipment.equipConstitution)).append("\n");
	playerString.append("Intelligence: ").append(std::to_string(playerCharacter.getAbilities().intelligence + playerCharacter.currentEquipment.equipIntelligence)).append(" Mod: ").append(std::to_string(playerCharacter.getModifier(playerCharacter.getAbilities().intelligence + playerCharacter.currentEquipment.equipIntelligence))).append(" Equip Bonus: ").append(std::to_string(playerCharacter.currentEquipment.equipIntelligence)).append("\n");
	playerString.append("Wisdom: ").append(std::to_string(playerCharacter.getAbilities().wisdom + playerCharacter.currentEquipment.equipWisdom)).append(" Mod: ").append(std::to_string(playerCharacter.getModifier(playerCharacter.getAbilities().wisdom + playerCharacter.currentEquipment.equipWisdom)).append(" Equip Bonus: ").append(std::to_string(playerCharacter.currentEquipment.equipWisdom))).append("\n");
	playerString.append("Charisma: ").append(std::to_string(playerCharacter.getAbilities().charisma + playerCharacter.currentEquipment.equipCharisma)).append(" Mod: ").append(std::to_string(playerCharacter.getModifier(playerCharacter.getAbilities().charisma + playerCharacter.currentEquipment.equipCharisma))).append(" Equip Bonus: ").append(std::to_string(playerCharacter.currentEquipment.equipCharisma)).append("\n");
	playerString.append("Move Speed: ").append(std::to_string(playerCharacter.getMoveSpeed())).append("\n");
	playerString.append("Initiative: ").append(std::to_string(playerCharacter.getInitiative())).append("\n");
	return playerString;
}

//This is a console window type function (would not legitimately be in the real game)
string MapScreen::viewEnemies()
{
	//Because you don't have enemy data in a real dungeons and dragons game, we are not showing the enemy data on screen.
	//So, in order to appropriate a way to show that stuff is ACTUALLY working, this is just to show off SOME enemy data
	string eStr = "";

	for (int i = 0; i < numberOfNPCs; i++)
	{
		eStr.append("Name: ").append(characterEntities[i].getName());
		eStr.append("  ID: ").append(std::to_string(characterEntities[i].getId()));
		eStr.append("  Level: ").append(std::to_string(characterEntities[i].getLevel()));
		eStr.append("  Current HP: ").append(std::to_string(characterEntities[i].getCurrentHP())).append("/").append(std::to_string(characterEntities[i].getHP()));
		eStr.append("\n");	
	}

	return eStr;
}

void MapScreen::adjustNPCLevel(int levelInc)
{
	for (int i = 0; i < numberOfNPCs; i++)
	{
		characterEntities[i].levelUp(levelInc);
	}
}

//Loot item at X,Y (return item and remove from map)
string MapScreen::lootItems(int x, int y)
{
	string lootedItems = "NULL";
	string tempText;

	if (checkItem(x, y))
	{
		lootedItems = getItem(x, y);
		removeItem(x, y);
	}

	return lootedItems;
}