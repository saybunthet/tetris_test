#pragma once
#include "common.h"


class Tetris
{
public :
	void WelcomeMenu();                                     // le menu au debut pour commencer le jeu
	void game_board_init();								    // initialisation de game board
	void Quit();											// quitter
	void NewGame();											// commencer un NewGame
	void GameOver();										// Gameover
	CMD  GetCmd();											// obtenir les commandes de clavier
	void DispatchCmd(CMD _cmd);								// distribuer les commandes
	void NewBlock();										// créer un nouveau bloc 						

	void writefile(Score_list &P);                          // écrire les scores
	int  readfile(Score_list &P);                           // lire les scores 
	void score_list(Score_list &P);                         // lister les scores
	void top_score();                                       // disposer la liste des scores
	void showScore();                                       // afficher le score en temps réel
	void showLevel();                                       // afficher le niveau du jeu
	void DisplayPause();                                    // Pause
	void start();											// afficher la page d'acceuil
	int  get_choice();										// recevoir le choice de joueur par le souris 
	void play_game();										// Comencer à jouer 
	void goto_choice(int flag);								// prendre l'action selon le chois de joueur
	void reset_game_area();									// clear game area and put it in black 

};


