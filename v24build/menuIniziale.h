#ifndef MENUINIZIALE_H
#define MENUINIZIALE_H
#include "utilities.h"

#define ROWSTARTBOX 2
#define COLSTARTBOX 2
#define ROWENDBOX 35
#define COLENDBOX 102

#define DIMLOGOROW 7
#define DIMLOGOCOL 19

#define STARTROWLOGOMENU 16
#define STARTCOLLOGOMENU 17

#define DIMOP 21
#define NUMOP 6

#define STARTROWOP 9
#define STARTCOLOP 59

#define STARTROWBLUBOX 7
#define STARTCOLBLUBOX 53

#define ENDROWBLUBOX 32
#define ENDCOLBLUBOX 85


void menuIniziale(); // il menu iniziale
int avviaMenuIniziale(); // per avviare il menu iniziale (fork part)
void stampaBoxMenu(); // per la box attorno al menu
void stampaLogoMenu(int start_row,int start_col);
void stampaOpzione(int i,int sottolineato);
void boxMenu(int start_row,int start_col,int end_row,int end_col);
#endif
