#ifndef DISEGNA_H
#define DISEGNA_H
#include "tronco.h"
#include "rana.h"
#include "auto.h"
#include "logo.h"
#include "menuIniziale.h"
#include "inizializza.h"
#include "proiettile.h"
#include "nemico.h"
#include "proiettileNemico.h"
#include "collisioni.h"
#include "debug.h"
#include "hud.h"
#include "schermo.h"

void avviaDrawProcess(int pipe_fd[2]);
void drawProcess(int* pipe_fd);

#endif
