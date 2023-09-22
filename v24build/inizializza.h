#ifndef INIZIALIZZA_H
#define INIZIALIZZA_H
#include "utilities.h"
void inizializzaSchermo(GameData* gameData);
void inizializzaOldPos(OldPos *old_pos);
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaStrada(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaPrato(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaTane(GameData* gameData);
void inizializzaHUD(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH]);
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

void inizializzaGameHUD(GameHUD *gameHud);
void inizializzaOggettiTane(Tana* tane);
void inizializzaSprites(Sprite* sprites);
void inizializza(GameData* gameData);
void avviaProcessiBase(int* pipe_fd,Pids* pids,int* pipeRana_fd);
Sprite inizializzaSprite(int rows, int cols, char **sprite, int color);

#endif
