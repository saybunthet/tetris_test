#pragma once
#include <graphics.h>
#pragma comment(lib,"Winmm.lib")						// utilisation de Winmm.lib

#define	WIDTH	10										// la region de jeu est de 10*22 unites
#define	HEIGHT	22	                       
#define	SIZE	20										// chaque unite est de 20 pixels

class Board
{
public:
	Board();
	~Board();
	void game_board_init();								    // initialisation de game board
	void reset_game_area();									// clear game area and put it in black 
	void showScore();                                       // afficher le score en temps réel
	void showLevel();                                       // afficher le niveau du jeu
	int getLevel();
	int getScore();
	void setLevel(int l);
	void setScore(int s);
	BYTE game_area[WIDTH][HEIGHT];
protected:
	int score;
	int level;

		
};

