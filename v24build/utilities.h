#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>

#define WIDTH 104
#define HEIGHT 37
#define RANA 1
#define TRONCHI 2
#define SFONDO 3
#define FIUME 4
#define TANE 5
#define PRATO 6
#define STRADA 7
#define STRISCIA 8
#define MARCIAPIEDE 9
#define AUTO 10
#define MENUBOX 11
#define PROIETTILE 12

#define OLDPOSDIM 12
#define MAXNPROIETTILI 3
#define MAXNNEMICI 3
#define MAXNPROIETTILINEMICI MAXNPROIETTILI

#define RANA_ROWS 2
#define RANA_COLS 3

#define TRONCO_ROWS 2
#define TRONCO_COLS 9

#define AUTO_ROWS 2
#define AUTO_COLS 4

#define CAMION_ROWS 2
#define CAMION_COLS 7

#define PROIETTILE_ROWS 2
#define PROIETTILE_COLS 1

#define TANA_ROWS 3
#define TANA_COLS 11

typedef enum 
{
	RANA_SPRITE, TRONCO_SPRITE, AUTO_SPRITE, CAMION_SPRITE, PROIETTILE_SPRITE, PROIETTILE_NEMICO_SPRITE, NEMICO_SPRITE,OPEN,CLOSE
}TipoSprite;


// Definizione della struttura dati per le coordinate (x, y) e per il tipo
typedef struct{
    int x;
    int y;
    char type;
    int id;
}PipeData;

typedef struct{
	PipeData general[OLDPOSDIM];
	PipeData proiettili[MAXNPROIETTILI];
	PipeData proiettiliNemici[MAXNPROIETTILINEMICI];
}OldPos;

typedef struct{
	char ch;
	int color;
	bool is_changed;
	int id;
	char tipo;
}ScreenCell;


typedef enum{TANA_OPENED, TANA_CLOSED } TanaStatus;

typedef struct {
	PipeData info;
	TanaStatus status;
}Tana;


typedef struct{
	int max_row;
	int max_col;
	char **sprite;
	int color;
}Sprite;

typedef struct {
	int tempo;
	int vite;
	int punteggio;
	int livello;
} GameInfo;		// struttura per le info di gioco

typedef struct {
	GameInfo gameInfo;
	char score_hud[WIDTH] ;		// stringa con livello e punteggio
	int start_x_hud ;					// dove inizia e finisce la Hud
	int end_x_hud ;
	int score_hud_y;					// righe per le info di gioco
	char life_hud[WIDTH]; 		//stringa per vite e tempo
	int start_x_life_hud;
	int end_x_life_hud; 			// posizione di fine della stringa, dipenderà dalla lunghezza di life_hud
	int life_hud_y;
	int time_hud_y;
	
} GameHUD;			// struttura con le posizioni della HUD.

typedef struct{
	pid_t pidRana;
	pid_t pidProiettili[MAXNPROIETTILI];
	pid_t pidProiettiliNemici[MAXNPROIETTILI];
	pid_t pidNemici[3];
	pid_t pidTronchi[3];
	pid_t pidVeicoli[8];
}Pids;

typedef struct{
	ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
}Schermo;

typedef struct{
	int contP;
	int contPN;
	int contN;
}Cont;

typedef struct{
	PipeData pipeData; // struttura per leggere la pipe
	
	Sprite sprites[9]; // le sprite degli oggetti
	
	Schermo schermo; // lo schermo
	
	Pids pids; // tutti i pid dei processi figli
	
	Cont contatori; // contatori per oggetti
	
	Tana tane[5]; // oggetti tana
	
	GameHUD gameHud;	// viene inizializzato nalla fuunz. inizializzaMatriceSchermo()
	
	OldPos oldPos; // dati oggetti al passo precedente
	
	int arrayDirTronchi[4]; // vettore per registrare la direzione di chiascun tronco
	int pipeRana_fd [2];
}GameData;

typedef enum{
	TANA_OPEN_OBJ, 
	TANA_CLOSE_OBJ,
	FIUME_OBJ,
	PRATO_OBJ,
	STRADA_OBJ,
	MARCIAPIEDE_OBJ,
	PN_OBJ,
	AUTO_OBJ,
	CAMION_OBJ,
	TRONCO_OBJ,
	N_OBJ,
	P_OBJ,
	RANA_OBJ,
	SFONDO_OBJ
}TipoObj;

void inizializzaNcurses();
void creaPipe(int pipe_fd[2]);
void inizializzaColorazione();
void uccidiProcesso( pid_t *array_pid, int id_processo);
int id_disponibile(pid_t *array_pid, int lunghezza);
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData);
#endif // UTILITIES_H

