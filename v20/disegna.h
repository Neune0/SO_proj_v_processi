#ifndef DISEGNA_H
#define DISEGNA_H
#include "tronco.h"
#include "rana.h"
#include "auto.h"
#include "logo.h"
#include "menuIniziale.h"
#include "inizializza.h"
#include "proiettile.h"
#define OLDPOSDIM 12

#define RANA_SPRITE {{'\\', '.', '/'}, {'/', '-', '\\'}}
#define TRONCO_SPRITE {{"~ ~ ~ ~ ~"}, {" ~ ~ ~ ~ "}}
#define AUTO_SPRITE {{'x', 'x', 'x', 'x'}, {'o', 'o', 'o', 'o'}}
#define CAMION_SPRITE {{'x', 'x', 'x', 'x', 'x', 'x', 'x'}, {'o', 'o', 'o', 'o', 'o', 'o', 'o'}}



#define RANA_ROWS 2
#define RANA_COLS 3
#define TRONCO_ROWS 2
#define TRONCO_COLS 9
#define AUTO_ROWS 2
#define AUTO_COLS 4
#define CAMION_ROWS 2
#define CAMION_COLS 7



typedef struct{
	int max_row;
	int max_col;
	char **sprite;
	int color;
}Sprite;

void avviaDrawProcess(int pipe_fd[2]);
void drawProcess(int* pipe_fd);
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData);

void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite);
void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]);
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

bool isBetween (int value, int min_value, int max_value);
bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2);


#endif
