#pragma once
#include <graphics.h>
#include "Score_list.h"
#include "Block.h"
#include "CMD.h"
#include "Board.h"

#pragma comment(lib,"Winmm.lib")						// utilisation de Winmm.lib
										// contrôle de temp pour charge opération 
class Tetris
{
public :
	void GameOver();										// Gameover
	void NewBlock();										// créer un nouveau bloc 	
	void start();											// afficher la page d'acceuil
	Board board;
	Block block;
	Tetris();
	~Tetris();
	
	
private:
	void WelcomeMenu();                                     // le menu au debut pour commencer le jeu
	void Quit();											// quitter
	CMD  GetCmd();											// obtenir les commandes de clavier
	void DispatchCmd(CMD _cmd);								// distribuer les commandes
	void NewGame();											// commencer un NewGame
	void writefile(Score_list &P);                          // écrire les scores
	int  readfile(Score_list &P);                           // lire les scores 
	void score_list(Score_list &P);                         // lister les scores
	void top_score();                                       // disposer la liste des scores
	void DisplayPause();                                    // Pause
	int  get_choice();										// recevoir le choice de joueur par le souris 
	void play_game();										// Comencer à jouer 
	void goto_choice(int flag);								// prendre l'action selon le chois de joueur
    Score_list user;										// définir user pour stocker le score et nom de joueur
	DWORD  oldtime;
};


