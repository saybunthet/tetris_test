
#include <conio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>  
#include "Block.h"
#include "Tetris.h"


Block::Block()
{   		
	g_blocks[0] = { 0x0F00, 0x4444, 0x0F00, 0x4444, RED };				// I
	g_blocks[1] = { 0x0660, 0x0660, 0x0660, 0x0660, BLUE };				// O
	g_blocks[2] = { 0x4460, 0x02E0, 0x0622, 0x0740, MAGENTA };			// L
	g_blocks[3] = { 0x2260, 0x0E20, 0x0644, 0x0470, YELLOW };			// L oppose
	g_blocks[4] = { 0x0C60, 0x2640, 0x0C60, 0x2640, CYAN };				// Z
	g_blocks[5] = { 0x0360, 0x4620, 0x0360, 0x4620, GREEN };			// Z oppose
	g_blocks[6] = { 0x4E00, 0x4C40, 0x0E40, 0x4640, BROWN };			// T

}
Block::~Block()
{
}


void Block::OnRotate()
{

	int distance;
	BlockInfo temp = g_CurBlock;
	temp.dir++;					 if (CheckBlock(temp)) { distance = 0; goto rotate; }		// rotation sans bouger
	temp.x = g_CurBlock.x - 1;   if (CheckBlock(temp)) { distance = -1; goto rotate; }		// bouger à gauche pour un unité et après tourner 
	temp.x = g_CurBlock.x + 1;   if (CheckBlock(temp)) { distance = 1; goto rotate; }		// bouger à droite pour un unité et après tourner
	temp.x = g_CurBlock.x - 2;   if (CheckBlock(temp)) { distance = -2; goto rotate; }		// bouger à gauche pour 2 unités et après tourner
	temp.x = g_CurBlock.x + 2;   if (CheckBlock(temp)) { distance = 2; goto rotate; }		// bouger à droite pour 2 unités et après tourner
	return;

	//rotation:tuer le bloc à l'instant et créer un nouveau bloc(qui est 'tourné') 
rotate:
	DrawBlock(g_CurBlock, HIDE);
	++g_CurBlock.dir;
	g_CurBlock.x += distance;
	DrawBlock(g_CurBlock,SHOW);

}
void Block::OnLeft()
{
	BlockInfo temp = g_CurBlock;
	--temp.x;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE);
		--g_CurBlock.x;
		DrawBlock(g_CurBlock, SHOW);
	}
}
void Block::OnRight()
{
	BlockInfo temp = g_CurBlock;
	++temp.x;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE);
		++g_CurBlock.x;
		DrawBlock(g_CurBlock, SHOW);
	}
}
void Block::OnDown()
{
	BlockInfo temp = g_CurBlock;
	--temp.y;

	if (CheckBlock(temp))
	{
		DrawBlock(g_CurBlock, HIDE);
		--g_CurBlock.y;
		DrawBlock(g_CurBlock, SHOW);
	}
	else OnSink();
}
void Block::OnSink()
{
	Tetris tetris;
	int i, x, y;
	int count = 0;
	/* continuer à bouger au dessous */
	DrawBlock(g_CurBlock, HIDE);
	BlockInfo temp = g_CurBlock;
	--temp.y;
	while (CheckBlock(temp))									//assurer que le bloc peut bouger 
	{
		--g_CurBlock.y;
		--temp.y;
	}
	DrawBlock(g_CurBlock, FIX);									//continue à bouger au dessus si possible

	/* mettre le bloc sur la place finale */
	WORD b = g_blocks[g_CurBlock.id].dir[g_CurBlock.dir];
	for (i = 0; i < 16; i++)
	{
		if (b & 0x8000)
		{
			if (g_CurBlock.y - i / 4 >= HEIGHT)
			{
				tetris.GameOver();										// si la place finale est trop haut - GAMEOVER
				return;
			}
			else
				game_area[g_CurBlock.x + i % 4][g_CurBlock.y - i / 4] = 1;
		}

		b <<= 1;
	}

	/* confirmer est-ce que on doit supprimer un ou plusieurs lignes*/
	int row[4] = { 0 };
	bool bRow = false;
	for (y = g_CurBlock.y; y >= max(g_CurBlock.y - 3, 0); y--)
	{
		i = 0;
		for (x = 0; x < WIDTH; x++)
			if (game_area[x][y] == 1)
				i++;
		if (i == WIDTH)
		{
			bRow = true;
			row[g_CurBlock.y - y] = 1;						// marquer le ligne
			setfillcolor(WHITE);
			bar(220, (HEIGHT - y - 1)*SIZE + SIZE / 2 - 2 + 20, WIDTH * SIZE - 1 + 220, (HEIGHT - y - 1) * SIZE + SIZE / 2 + 2 + 20);
		}
	}

	if (bRow)
	{
		Sleep(200);
		/* supprimer le ligne marqué */
		IMAGE img;
		for (i = 0; i < 4; i++)
		{
			if (row[i])
			{
				count++;
				for (y = g_CurBlock.y - i + 1; y < HEIGHT; y++)
					for (x = 0; x < WIDTH; x++)
					{
						game_area[x][y - 1] = game_area[x][y];
						game_area[x][y] = 0;
					}
				getimage(&img, 220, 20, WIDTH * SIZE, (HEIGHT - (g_CurBlock.y - i + 1)) * SIZE);
				putimage(220, SIZE + 20, &img);
			}
		}
		/* calculer le score et le niveaux de difficulté*/
		score += count * 10;
		showScore();
		level = score / 100 + 1;					          //le niveau est incrémenté tanque tout les 100 points de score 
		showLevel();
	}
	tetris.NewBlock();												  // créer un nouveau bloc
}
bool Block::CheckBlock(BlockInfo _block)
{
	WORD b = g_blocks[_block.id].dir[_block.dir];
	int x, y;

	for (int i = 0; i < 16; i++)
	{
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if ((x < 0) || (x >= WIDTH) || (y < 0))						//trop gauche ou trop droite
				return false;
			if ((y < HEIGHT) && (game_area[x][y]))					//trop haut
				return false;

		}
		b <<= 1;
	}
	return true;
}
void Block::DrawBlock(BlockInfo _block, DRAW _draw)
{
	WORD b = g_blocks[_block.id].dir[_block.dir];
	int x, y;

	int color = BLACK;
	switch (_draw)
	{
	case SHOW: color = g_blocks[_block.id].color; break;
	case HIDE: color = BLACK;	break;
	case FIX: color = g_blocks[_block.id].color / 3; break;
	}
	setfillcolor(color);

	for (int i = 0; i < 16; i++)                                        //scanner une région de £´*£´ et dessiner le bloc 
	{
		if (b & 0x8000)
		{
			x = _block.x + i % 4;
			y = _block.y - i / 4;
			if (y < HEIGHT)
			{
				if (_draw != HIDE)
				{
					bar3d(x*SIZE + 2 + 220, (HEIGHT - y - 1)*SIZE + 2 + 20, (x + 1)*SIZE - 4 + 220, (HEIGHT - y)*SIZE - 4 + 20, 3, true);
				}
				else
					bar(x*SIZE + 220, (HEIGHT - y - 1)*SIZE + 20, (x + 1)*SIZE - 1 + 220, (HEIGHT - y)*SIZE - 1 + 20);

			}
		}
		b <<= 1;
	}

}