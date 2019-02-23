#include "common.h"



void Tetris::start()
{
	WelcomeMenu();
	int flag = get_choice();
	goto_choice(flag);
	_getch();
	closegraph();
}
int  Tetris::get_choice()
{
	MOUSEMSG m;
	int flag = 0;
	while (TRUE)
	{
		m = GetMouseMsg();
		switch (m.uMsg)
		{
		case WM_LBUTTONDOWN:													// opération de la souris 
			if (m.x >= 268 && m.x <= 392 && m.y >= 268 && m.y <= 312)
			{
				flag = 1;														//jouer
				break;
			}
			else if (m.x >= 268 && m.x <= 392 && m.y >= 328 && m.y <= 370)
			{
				flag = 2;														//Scores
				break;
			}
			else if (m.x > 268 && m.x <= 392 && m.y >= 388 && m.y <= 430)
			{
				flag = 3;														//quitter

				break;
			}

		}
		if (flag != 0) break;
		
	}
	return flag;

}
void Tetris::play_game()
{
	game_board_init();
	/* commencer le jeux*/
	NewGame();
	CMD  c;
	while (1)
	{

		c = GetCmd();											    //recevoir la commande
		DispatchCmd(c);												// exécuter le commande
		/* ouvrir un fenêtre pour assurer quitter*/
		if (c == CMD_QUIT)
		{
			HWND wnd = GetHWnd();
			if (MessageBox(wnd, "Est-ce que vous voulez quitter?", "Bonjour!", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
				Quit();
		}

	}
}
void Tetris::goto_choice(int flag)
{
	if (flag == 1) play_game();
	else if (flag == 2)  top_score();
	else if (flag == 3) 	Quit();
}
void Tetris::WelcomeMenu()
{
	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Welcome_menu_1.jpg");
	SetWorkingImage(&img);
	SetWorkingImage();
	putimage(0, 0, &img);
}
void Tetris::game_board_init()
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
void Tetris::reset_game_area()
{
	
	setfillcolor(BLACK);
	bar((WIDTH + 1)*SIZE, SIZE, (2 * WIDTH + 1)*SIZE - 1, (HEIGHT + 1)*SIZE - 1);
	ZeroMemory(game_area, WIDTH*HEIGHT);
}
void Tetris::NewGame()
{
	reset_game_area();
	/* obtenir le bloc prochain */
	NewBlock();
}
void Tetris::GameOver()
{
	HWND wnd = GetHWnd();
	writefile(user);
	score_list(user);
	if (MessageBox(wnd, "GAME OVER \nJouer encore une fois?", "GAME OVER", MB_YESNO | MB_ICONQUESTION) == IDYES)
	{

		NewGame();
		score = 0;
		showScore();
	}
	else
		Quit();


}
void Tetris::Quit()
{
	closegraph();
	exit(0);
}		
CMD  Tetris::GetCmd()
{
	while (1)
	{
		// le bloc va tomber chaque 0.5s si il y a aucune opération 
		DWORD newtime = GetTickCount();

		if (newtime - oldtime >= 600 - level * 50)
		{
			oldtime = newtime;
			return CMD_DOWN;
		}
		if (_kbhit())								// check the command
		{
			switch (_getch())
			{
			case 'W':
			case 'w':  return CMD_ROTATE;
			case 'A':
			case 'a':  return CMD_LEFT;
			case 'S':
			case 's':  return CMD_DOWN;
			case 'D':
			case 'd':  return CMD_RIGHT;
			case 'P':
			case 'p':  return CMD_STOP;
			case 27:   return CMD_QUIT;
			case ' ':  return CMD_SINK;
			case 0:									//speciale
			case 0xE0:								//speciale
				switch (_getch())					//recevoir le commande prochain
				{
				case 72:	return CMD_ROTATE;      //up
				case 75:	return CMD_LEFT;        //left
				case 77:	return CMD_RIGHT;       //right
				case 80:	return CMD_DOWN;        //down
				}
			}

		}
		Sleep(20);
	}
}
void Tetris::DispatchCmd(CMD _cmd)
{
	Block block;
	switch (_cmd)
	{
	case CMD_ROTATE:	block.OnRotate();		break;
	case CMD_LEFT:		block.OnLeft();		break;
	case CMD_RIGHT:		block.OnRight();		break;
	case CMD_DOWN:		block.OnDown();		break;
	case CMD_SINK:      block.OnSink();       break;
	case CMD_STOP:      DisplayPause(); break;
	case CMD_QUIT:		break;
	}
}
void Tetris::NewBlock()
{
	Block block;
	/* générer la configuaration de nouveau block*/
	g_NextBlock.id = rand() % 7;
	g_NextBlock.dir = rand() % 4;
	g_NextBlock.x = 11;
	g_NextBlock.y = 21;
	/*passer la configuration de nouveau block au block current*/
	g_CurBlock.id = g_NextBlock.id, g_NextBlock.id = rand() % 7;
	g_CurBlock.dir = g_NextBlock.dir, g_NextBlock.dir = rand() % 4;
	g_CurBlock.x = 3;
	g_CurBlock.y = 24;

	WORD c = g_blocks[g_CurBlock.id].dir[g_CurBlock.dir];

	while ((c & 0X000F) == 0)
	{
		--g_CurBlock.y;
		c >>= 4;
	}

	/* créer un bloc */
	block.DrawBlock(g_CurBlock,SHOW);

	/* créer le bloc prochain */
	setfillcolor(BLACK);
	bar(440, 20, 519, 99);
	block.DrawBlock(g_NextBlock,SHOW);

	/* les blocs tombent automatiquement */
	oldtime = GetTickCount();

}
void Tetris::writefile(Score_list &S)
{


	FILE *fp;
	fopen_s(&fp, "data_storage\\ScoreTop.rec", "ab");
	if (!fp)
	{
		printf("file cannot be opened");
		exit(1);
	}
	do
	{
		InputBox(S.name, 10, "Votre prénom");					// ajouter le prénom de joueur 
		S.grades = score;
		fwrite(&S, sizeof(S), 1, fp);


		break;
	} while (1);
	fclose(fp);

}
int	 Tetris::readfile(Score_list &S)
{
	int x = 78, y = 70, z = 455, i = 1;
	char ch_grades[5], ch[1], dot = '.';
	FILE *fp;
	fopen_s(&fp, "data_storage\\ScoreTop.rec", "rb");
	if (!fp)
	{
		printf("file cannot be opened");
		exit(1);
	}
	settextstyle(33, 0, "Calibri");
	setbkmode(TRANSPARENT);

	while (fread(&S, sizeof(S), 1, fp) == 1)
	{
		sprintf_s(ch, sizeof(ch), "%d", i);
		outtextxy(x - 22, y, ch);
		outtextxy(x - 11, y, dot);
		outtextxy(x, y, S.name);
		sprintf_s(ch_grades, sizeof(ch_grades), "%g", S.grades);
		outtextxy(z, y, ch_grades);
		y += 35;
		i++;
	}
	fclose(fp);
	return 0;
}
void Tetris::score_list(Score_list &S)
{
	Score_list a[12];
	FILE *fp;
	int length;


	int i, j;
	fopen_s(&fp, "data_storage\\ScoreTop.rec", "rb");

	for (i = 1; fread(&S, sizeof(S), 1, fp) == 1 && i <= 11; i++)
	{

		a[i] = S;
	}

	length = i - 1;

	for (i = 2; i <= length; i++)
		if (a[i].grades < a[i - 1].grades)
		{
			a[0] = a[i];
			a[i] = a[i - 1];
			for (j = i - 2; a[0].grades < a[j].grades; j--)
				a[j + 1] = a[j];
			a[j + 1] = a[0];
		}


	fclose(fp);

	fopen_s(&fp, "data_storage\\ScoreTop.rec", "wb");
	int b = 0;
	for (i = length; b < 10 && i >= 1; i--, b++)
	{
		fwrite(&a[i], sizeof(a[i]), 1, fp);
	}
	fclose(fp);
}
void Tetris::top_score()
{
	initgraph(640, 480);
	IMAGE img(640, 480);
	loadimage(&img, "Pic\\Scoretop.jpg");
	SetWorkingImage(&img);
	Score_list S;
	readfile(S);
	SetWorkingImage();
	putimage(0, 0, &img);
	MOUSEMSG p;
	int flag = 0;
	while (TRUE) {
		p = GetMouseMsg();
		switch (p.uMsg)
		{
		case(WM_LBUTTONDOWN):
			if (p.x >= 520 && p.x <= 582 && p.y >= 14 && p.y <= 46)
			{
				WelcomeMenu(); flag = 1;
			}
			break;
		}if (flag != 0) break;
	}
}
void Tetris::showScore() {
	char str[10];
	setfillcolor(BLACK);
	rectangle(449, 299, 576, 326);
	bar(450, 300, 575, 325);
	setcolor(WHITE);
	settextstyle(25, 0, "Calibri");
	sprintf_s(str, sizeof(str), "score:%d", score);
	outtextxy(450, 300, str);
}
void Tetris::showLevel()
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
void Tetris::DisplayPause()
{
	while (_getch() != 'p'&&_getch() != 'P')
		;
}



