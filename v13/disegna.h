#ifndef DISEGNA_H
#define DISEGNA_H
#include "tronco.h"
#include "rana.h"
#include "auto.h"
#include "logo.h"
#include "menuIniziale.h"
#include "inizializza.h"
#define OLDPOSDIM 8

// TODO: si possono accorpare le strutture per gli sprite basta inserire un campo row e uno col
typedef struct {
    char sprite[2][9]; // 2 righe - 9 char  
} SpriteTronco;

typedef struct {
		char sprite[2][4]; // 2righe - 4 char
}SpriteAuto;

typedef struct {
		char sprite[2][8]; // 2righe - 7 char + terminatore
}SpriteCamion;

void avviaDrawProcess(int pipe_fd[2]);
void drawProcess(int* pipe_fd);
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData);

//rana
void aggiornaPosizioneRana(PipeData *pipeData,PipeData *old_pos,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]);
void printFrog(int x,int y);
void pulisciRanaInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaRanaInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH]);
// tronchi
void aggiornaPosizioneTronco(PipeData *pipeData,PipeData *old_pos,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]);
void disegnaTronco(int row, int col);
void stampaTroncoInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH]);
void pulisciTroncoInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]);
//auto
void aggiornaPosizioneAuto(PipeData *pipeData,PipeData *old_pos,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]);
void pulisciAutoInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaAutoInMatrice(int row,int col, ScreenCell (*screenMatrix)[WIDTH]);
#endif
