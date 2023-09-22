#ifndef COLLISIONI_H
#define COLLISIONI_H
#include "utilities.h"
#include "debug.h"
#include "rana.h"
#include "proiettileNemico.h"
void checkCollisioni(Collisione* collisione,int startRow,int maxRows,int startCol,int maxCols,Schermo* schermo,PipeData* pipeData);
void gestisciCollisione(Collisione* collisione, GameData* gameData, int* pipe_fd);
#endif
