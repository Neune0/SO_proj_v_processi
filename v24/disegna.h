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

/*
#define OLDPOSDIM 12
#define MAXNPROIETTILI 3
#define MAXNNEMICI 3
#define MAXNPROIETTILINEMICI MAXNPROIETTILI

typedef enum 
{
	RANA_SPRITE, TRONCO_SPRITE, AUTO_SPRITE, CAMION_SPRITE, PROIETTILE_SPRITE, PROIETTILE_NEMICO_SPRITE, NEMICO_SPRITE
}TipoSprite;
/**/

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
/*
typedef enum{OPEN, CLOSED } TanaStatus;

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
/**/


int id_disponibile(pid_t *array_pid_proiettili,int lunghezza);
void avviaDrawProcess(int pipe_fd[2]);
void drawProcess(int* pipe_fd);
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData);

void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite);
void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]);
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);
//------------------------------------------------------------COLLISIONI----------------------------
/*
bool collisioneRana( PipeData *old_pos, Sprite *array_sprite);
bool collisioneAuto( PipeData *old_pos, Sprite *array_sprite);
bool collisioneProiettiliNemici( PipeData *old_pos, PipeData *old_pos_proiettiliNemici ,Sprite *array_sprite);
bool collisioneTaneAperte( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite);
bool collisioneTaneChiuse( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite);
bool collisioneAutoProiettili( PipeData *old_pos, PipeData * array_proiettili, Sprite *array_sprite);



bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2);
bool isBetween (int value, int min_value, int max_value);
/**/
#endif
