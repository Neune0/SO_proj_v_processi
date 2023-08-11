#ifndef DISEGNA_H
#define DISEGNA_H
#include "tronco.h"
#include "rana.h"
#include "auto.h"
#include "logo.h"
#include "menuIniziale.h"

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

// TODO: da rendere tipo, risparmio caratteri in dichiarazioni funzioni
struct ScreenCell{
	char ch;
	int color;
	bool is_changed;
};


void inizializzaOldPos(struct PipeData *old_pos,int length);
void avviaDrawProcess(int pipe_fd[2]);
void drawProcess(int* pipe_fd);
void inizializzaMatriceSchermo(struct ScreenCell (*screenMatrix)[WIDTH]);

//rana
void aggiornaPosizioneRana(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void printFrog(int x,int y);
void pulisciRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH]);
// tronchi
void aggiornaPosizioneOggetto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);

void disegnaTronco(int row, int col);
void stampaTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH]);
void pulisciTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);

//auto
void aggiornaPosizioneAuto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);
															

void pulisciAutoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaAutoInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]);


void stampaMatrice(struct ScreenCell (*screenMatrix)[WIDTH]);
void stampaMatriceStatica(struct ScreenCell (*staticScreenMatrix)[WIDTH]);




#endif
