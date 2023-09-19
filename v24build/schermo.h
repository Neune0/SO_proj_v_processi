#ifndef SCHERMO_H
#define SCHERMO_H
#include "utilities.h"
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite);
void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]);
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);
void cancellaOggetto(PipeData *array_oggetti, Sprite* sprite_oggetto, 
											ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], int id_oggetto);
void aggiorna(GameData* gameData);
#endif
