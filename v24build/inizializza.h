#ifndef INIZIALIZZA_H
#define INIZIALIZZA_H
#include "utilities.h"
void inizializzaSchermo(GameHUD *gameHud, Schermo* schermo);
void inizializzaOldPos(PipeData *old_pos);
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaStrada(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaPrato(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaTane(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaHUD(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH]);
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

void inizializzaGameHUD(GameHUD *gameHud);
void inizializzaTane(Tane* tane);
void inizializzaSprite(Sprite* sprites);
void inizializza(GameData* gameData);
void avviaProcessiBase(int* pipe_fd,Pids* pids,int* pipeRana_fd);

#endif
