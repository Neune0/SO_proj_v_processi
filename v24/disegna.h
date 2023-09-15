#ifndef DISEGNA_H
#define DISEGNA_H
#include "tronco.h"
#include "rana.h"
#include "auto.h"
#include "logo.h"
#include "menuIniziale.h"
#include "inizializza.h"
#include "proiettile.h"
#include "nemico.h"
#include "proiettileNemico.h"
#include "collisioni.h"


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


int timer(time_t *current_time, time_t *start_time, int max_time);
int id_disponibile(pid_t *array_pid_proiettili,int lunghezza);
void avviaDrawProcess(int pipe_fd[2]);
void drawProcess(int* pipe_fd);
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData);

void aggiornaDirezioneTronchi(PipeData *pipeData, PipeData *old_pos, int *arr_dir_tronchi );


void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite);

void aggiornaHudInMatrice(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH]);

void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]);
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

void cancellaOggetto(PipeData *array_oggetti, Sprite* sprite_oggetto, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], int id_oggetto);


void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]);

void aggiornaGameInfo(GameHUD *gameHud, int tempo, int vite, int punteggio, int livello);

#endif
