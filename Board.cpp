
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>  
#include "Board.h"



Board::Board()
{
	score = 0;
	level = 1;
	for (int i = 0; i < WIDTH; i++)
	{
		for (int j=0;j<HEIGHT;j++)
			game_area[i][j] = 0;
	}
	
}


Board::~Board()
{
}

void Board::game_board_init()
{
	initgraph(640, 480);

	HWND hwnd = GetHWnd();
	SetWindowText(hwnd, "Tetris");												// Titre de jeu 

	/* ouvrir la musique du background*/
	mciSendString("open music\\change.mp3 alia mymusic", NULL, 0, NULL);
	mciSendString("play music\\change.mp3 repeat", NULL, 0, NULL);


	srand((unsigned)time(NULL));												// pour fabriquer un chiffre random 
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\1-110501012153.jpg");
	SetWorkingImage(&img);
	showScore();
	showLevel();

	/*region pour afficher le guide du jeur*/
	settextstyle(35, 0, "Calibri");
	setbkmode(TRANSPARENT);
	outtextxy(50, 50, "GUIDE");
	settextstyle(20, 0, "Calibri");
	outtextxy(55, 135, "up-rotate");
	outtextxy(55, 155, "left-move left");
	outtextxy(55, 175, "down-move down");
	outtextxy(55, 195, "right-move right");
	outtextxy(55, 215, "space-to the bottom");
	outtextxy(55, 235, "ESC-quit");
	settextstyle(22, 0, "Calibri");
	outtextxy(60, 300, "P-Pause the game");



	/*définir la région pour jouer*/
	rectangle((WIDTH + 1)*SIZE - 1, SIZE - 1, (2 * WIDTH + 1)*SIZE, (HEIGHT + 1)*SIZE);
	rectangle(439, 19, 520, 100);
	SetWorkingImage();
	putimage(0, 0, &img);

}
void Board::reset_game_area()
{

	setfillcolor(BLACK);
	bar((WIDTH + 1)*SIZE, SIZE, (2 * WIDTH + 1)*SIZE - 1, (HEIGHT + 1)*SIZE - 1);
	ZeroMemory(game_area, WIDTH*HEIGHT);
}
void Board::showScore() {
	char str[10];
	setfillcolor(BLACK);
	rectangle(449, 299, 576, 326);
	bar(450, 300, 575, 325);
	setcolor(WHITE);
	settextstyle(25, 0, "Calibri");
	sprintf_s(str, sizeof(str), "score:%d", score);
	outtextxy(450, 300, str);
}
void Board::showLevel()
{
	char str[10];

	setfillcolor(BLACK);

	rectangle(449, 249, 576, 276);

	bar(450, 250, 575, 275);

	setcolor(WHITE);

	settextstyle(25, 0, "Calibri");

	sprintf_s(str, sizeof(str), "Level:%d", level);

	outtextxy(450, 250, str);

}
int Board::getLevel()
{
	return level;
}
int Board::getScore()
{
	return score;
}
void Board::setLevel(int l)
{
	level = l;
}
void Board::setScore(int s)
{
	score = s;
}