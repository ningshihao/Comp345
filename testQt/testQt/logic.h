/*
Author: Nicholas Burdet
Id: 29613773
Course: COMP 345
Assignment 2 Part 3: Map

Logic header file
*/

#ifndef LOGIC_H
#define LOGIC_H

#include <QPoint>
#include <QWidget>
#include "MapScreen.h"
#include "playerLog.h"
#include "Campaign.h"
#include <string>
#include <QMessageBox>
#include <vector>
#include "character.h"
#include <QMainWindow>
#include "dice.h"

using namespace std;

class logic : public QWidget
{
	Q_OBJECT;
	public:
		logic(QWidget *parent = 0, QMainWindow *mw = 0);
		void initialize(int w, int h, string n);
		void newCampaign(string cName, int cId);
		void loadCampaign(string filename);
		void loadJustCampaign();
		void loadPlayerCharacter(string filename);
		void setResolution(int res);
		void loadMap(string filename);
		void newMap();
		bool previousMap();
		bool nextMap();
		void resetMap(int w, int h);
		int getWidth();
		int getHeight();
		string getFilename();
		void closeWindow();
		void setEditmode(bool editM);
		int checkResolution(int x, int y);
	protected:
		void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
		void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;
		void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;
		void resizeEvent(QResizeEvent *event) Q_DECL_OVERRIDE;
		void leaveEvent(QEvent * event) Q_DECL_OVERRIDE;
		void changeEvent(QEvent * event) Q_DECL_OVERRIDE;
		void focusOutEvent(QFocusEvent * event) Q_DECL_OVERRIDE;
		void enterEvent(QEvent * event) Q_DECL_OVERRIDE;
	private:
		QPoint lastPoint = QPoint(0,0);
		QImage image;
		QMessageBox message;
		QFont font;

		//flag to invoke fullscreen painter and initialization (also incase contents of screen disappear)
		bool start = true;
		bool editMode = true;

		MapScreen ms;
		campaign Campaign;
		int resolution = 50;

		//flag each time users clicks on screen to invoke mouse event
		bool clicked = false;

		int currentX;
		int currentY;
		QPixmap currentTile = QPixmap("C:/Users/Nick/Desktop/Images/grass.png");
		
		//This will be the mode checking for tiles
		int mode = 1; 
		
		//This will be in coupling with the mode, the determining factor for the NPC
		int npcId = 0;

		//flags to indicate the user is drawing entrance or exit
		bool drawStart = false; 
		bool drawEnd = false;
		bool drawNPC = false;
		bool movePlayer = false;
		bool replacePlayer = false;

		//flag to check path status
		bool checkStatus = true;

		//stores if path exists here, default map path exists
		bool path = true;

		bool textChange = false;

		//values to hold previous entrance or exit when painting new ones to fill in old spaces
		int oldStartX;
		int oldStartY;
		int oldEndX;
		int oldEndY;

		int oldPlayerX;
		int oldPlayerY;
		string replace = "";

		int wid;
		int hei;

		//this section will hold some variables pertaining to gameplay ONLY, ignore for non-gameplay related stuff
		int playerSteps = 0;
		bool gameSession = false;


		playerLog combatLog;
		bool playerTurn = true;
		bool playerMove = false;

		bool playerAttacking = false;
		bool fullAttack = false;

		int npcTurn = -1;
		QMainWindow *mainWindow;
		bool mapStart = true;

		vector<int> turnOrder;

		QPixmap grass = QPixmap("Images/grass.png");
		QPixmap dirt = QPixmap("Images/dirt.jpg");
		QPixmap entranceDoor = QPixmap("Images/start.png");
		QPixmap exitDoor = QPixmap("Images/end.png");
		QPixmap checkButton = QPixmap("Images/button.png");
		QPixmap errorButton = QPixmap("Images/redbutton.jpg");
		QPixmap saveButton = QPixmap("Images/save.png");
		QPixmap playerImage = QPixmap("Resources/player.png");

		QPixmap orc = QPixmap("Images/orc.png");
		QPixmap ogre = QPixmap("Images/ogre.png");
		QPixmap minotaur = QPixmap("Images/minotaur.png");
		QPixmap logBackground = QPixmap("Images/log.png");

		QPixmap background = QPixmap("Images/background.jpg");
		QPixmap item = QPixmap("Images/item.png");
		QPixmap chest = QPixmap("Images/chest.png");
};

#endif
