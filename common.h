#pragma once
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>  
#include <graphics.h>
#include "Score_list.h"
#include "Block.h"
#include "Tetris.h"
#include "CMD.h"
#include "DRAW.h"


#pragma comment(lib,"Winmm.lib")						// utilisation de Winmm.lib
 

#define	WIDTH	10										// la region de jeu est de 10*22 unites
#define	HEIGHT	22	                       
#define	SIZE	20										// chaque unite est de 20 pixels


/*définir le score et le niveau de difficulté*/
int score = 0;
int level = 1;
BYTE game_area[WIDTH][HEIGHT] = { 0 };
DWORD  oldtime;											// contrôle de temp pour charge opération 
Score_list user;										// définir user pour stocker le score et nom de joueur
/*
il y a 7 types des bloc
0x0f00 = [0000 1111 0000 0000]:	I horizontale
0x4444=[0100 0100 0100 0100]: I verticale
*/
Block_stuct g_blocks[7] = {
{ 0x0F00, 0x4444, 0x0F00, 0x4444, RED },				// I
{ 0x0660, 0x0660, 0x0660, 0x0660, BLUE },				// O
{ 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA },			// L
{ 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW },				// L oppose
{ 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN },				// Z
{ 0x0360, 0x4620, 0x0360, 0x4620, GREEN },				// Z oppose
{ 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN } };			// T

BlockInfo g_CurBlock, g_NextBlock;