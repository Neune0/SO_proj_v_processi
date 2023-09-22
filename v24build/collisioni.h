#ifndef COLLISIONI_H
#define COLLISIONI_H
#include "utilities.h"
#include "debug.h"
TipoCollisione checkCollisioni(int startRow,int maxRows,int startCol,int maxCols,Schermo* schermo,PipeData* pipeData);
void gestisciCollisione(TipoCollisione collisione, GameData* gameData, int* pipe_fd);
#endif
