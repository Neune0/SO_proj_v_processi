#ifndef SCHERMO_H
#define SCHERMO_H
#include "utilities.h"
#include "collisioni.h"
void aggiorna(GameData* gameData,int* pipe_fd);
void cancellaOggetto(GameData* gameData, PipeData *old_pos, TipoSprite tipoSprite);
void aggiornaOggetto(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite,int* pipe_fd);

void stampaSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo,PipeData* pipeData,GameData* gameData,int* pipe_fd);
void pulisciSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo);

void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]);
void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]);
#endif
