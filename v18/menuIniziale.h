#ifndef MENUINIZIALE_H
#define MENUINIZIALE_H
#include "utilities.h"
#define ROWSTARTBOX 2
#define COLSTARTBOX 2
#define ROWENDBOX 35
#define COLENDBOX 102
#define DIMLOGOROW 7
#define DIMLOGOCOL 19
#define STARTROWLOGOMENU 7
#define STARTCOLLOGOMENU 43
#define DIMOP 20
#define NUMOP 4
#define STARTCOLOP 42
#define STARTROWOP 17
void menuIniziale(); // il menu iniziale
int avviaMenuIniziale(); // per avviare il menu iniziale (fork part)
void stampaBoxMenu(); // per la box attorno al menu
void stampaLogoMenu(int start_row,int start_col);
void stampaOpzione(int i,int sottolineato);
#endif
