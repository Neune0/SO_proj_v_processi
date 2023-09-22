#ifndef DEBUG_H
#define DEBUG_H
#include "utilities.h"

void aggiornaGameInfo(GameHUD *gameHud, int tempo, int vite, int punteggio, int livello);
void aggiornaHudInMatrice(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaGameHud(GameHUD *gameHud);
#endif
