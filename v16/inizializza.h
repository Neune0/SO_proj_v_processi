#ifndef INIZIALIZZA_H
#define INIZIALIZZA_H
#include "utilities.h"
void inizializzaMatriceSchermo(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);
void inizializzaOldPos(PipeData *old_pos,int length);
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaStrada(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaPrato(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaTane(ScreenCell (*screenMatrix)[WIDTH]);
void inizializzaHUD(ScreenCell (*screenMatrix)[WIDTH]);
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]);

#endif
