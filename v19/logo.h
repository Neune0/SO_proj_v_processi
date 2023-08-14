#ifndef LOGO_H
#define LOGO_H
#include "utilities.h"

#define ROWSTARTBOX 2
#define COLSTARTBOX 2
#define ROWENDBOX 35
#define COLENDBOX 102

#define DIMLOGOROW 7
#define DIMLOGOCOL 19

#define DIMTITOLOROW 6
#define DIMTITOLOCOL 46

#define STARTROWLOGO 18
#define STARTCOLLOGO 43

#define STARTROWTITOLO 9
#define STARTCOLTITOLO 29

#define STARTROWBENVENUTO 5
#define STARTCOLBENVENUTO 46

#define STARTROWCONTINUA 31
#define STARTCOLCONTINUA 36
void logo();
void avviaLogo();
void stampaBox();
void stampaLogo(int start_row,int start_col);
void stampaTitolo(int start_row,int start_col);
#endif
