#ifndef DISEGNA_H
#define DISEGNA_H
#include "tronco.h"
#include "rana.h"
#include "auto.h"
#define RANA 1
#define TRONCHI 2
#define SFONDO 3
#define FIUME 4
#define TANE 5
#define PRATO 6
#define STRADA 7
#define STRISCIA 8
#define MARCIAPIEDE 9

//							 Sprite per i Tronchi
typedef struct {
    char sprite[2][9]; // 2 righe - 9 char  
} SpriteTronco;

typedef struct {
		char sprite[2][5]; // 2righe - 4 char + terminatore
}SpriteAuto;

typedef struct {
		char sprite[2][8]; // 2righe - 7 char + terminatore
}SpriteCamion;

struct ScreenCell{
	char ch;
	int color;
};

void drawProcess(int* pipe_fd);
void printFrog(int x,int y);
void disegnaTronco(int row, int col);
void stampaTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH]);
void pulisciTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaMatrice(struct ScreenCell (*screenMatrix)[WIDTH]);

void inizializzaMatriceSchermo(struct ScreenCell (*screenMatrix)[WIDTH]);
void pulisciRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH]);

void aggiornaPosizioneAuto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);
															

void pulisciAutoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaAutoInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaPosizioneOggetto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);
#endif
