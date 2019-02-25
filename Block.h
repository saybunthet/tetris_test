#pragma once
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>  
#include <graphics.h>
#include "Board.h"
#include "DRAW.h"


#pragma comment(lib,"Winmm.lib")						// utilisation de Winmm.lib

struct BlockInfo
{
	byte id;		// ID
	char x, y;		// coordonnee
	byte dir : 2;	// 4 directions(codé en 2 bits)
};
struct Block_stuct
{
	WORD dir[4];	//couleurs des bloc
	COLORREF  color;	//structure des blocs
};
class Block: public Board
{
public:
	Block();
	~Block();
	void OnRotate();										// rotation
	void OnLeft();											// bouger vers le gauche 
	void OnRight();											// bouger vers le droite
	void OnDown();											// bouger vers le bas
	void OnSink();                                          // aller tout droite au dessous 
	bool CheckBlock(BlockInfo _block);						// assurer que le bloc peut être mis 
	void DrawBlock(BlockInfo _block, DRAW _draw);			// dessiner un bloc 
	BlockInfo g_CurBlock, g_NextBlock;;
	Block_stuct g_blocks[7];

};

