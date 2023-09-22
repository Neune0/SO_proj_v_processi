#include "collisioni.h"

TipoCollisione checkCollisioni(int startRow,int maxRows,int startCol,int maxCols,Schermo* schermo,PipeData* pipeData){
	int row=0;
	int col=0;
	for (int i = 0; i < maxRows; i++) {
  	for (int j = 0; j < maxCols; j++) {
    	row = startRow + i;
      col = startCol + j;
      
      switch(pipeData->type){
				case 'X': // rana
					switch(schermo->screenMatrix[row][col].tipo){
						case AUTO_OBJ:
							return RANA_AUTO;
							break;
						case CAMION_OBJ:
							return RANA_CAMION;
						case FIUME_OBJ:
							return RANA_FIUME;
						case TRONCO_OBJ:
							return RANA_TRONCO;
						case N_OBJ:
							return RANA_NEMICO;
						case TANA_OPEN_OBJ:
							return RANA_TANA_APERTA;
							break;
						case TANA_CLOSE_OBJ:
							return RANA_TANA_CHIUSA;
							break;
						case PN_OBJ:
							return RANA_PROIETTILE_NEMICO;
							break;
						default:
							break;
					}		
					break;
				case 'T': // tronco
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							return TRONCO_RANA;
						default:
							break;
					}		
					break;
				case 'A': // auto
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							return AUTO_RANA;
						default:
							break;
					}			
					break;
				case 'C': // camion
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							return CAMION_RANA;
						default:
							break;
					}			
					break;
				case 'P': // proiettile amico
								
					break;
				case 'p': // proiettile nemico
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							return PROIETTILE_NEMICO_RANA;
						default:
							break;
					}			
					break;
				default:
					break;
			}
    }
  }
  return NO_COLLISIONE;

}

void gestisciCollisione(TipoCollisione collisione){
	printCollisione(collisione); // per debug
	// switch su collisione:
	switch (collisione) {
        case RANA_AUTO:
        case RANA_CAMION:
        case RANA_FIUME:
        case RANA_NEMICO:
        case RANA_TANA_CHIUSA:
        case AUTO_RANA:
        case CAMION_RANA:
        	// termina rana
        	break;
        case RANA_PROIETTILE_NEMICO:
        case PROIETTILE_NEMICO_RANA:
        	// termina rana e termina proiettile nemico
					break;
        case RANA_TRONCO:
        case TRONCO_RANA:
            // la rana si attacca al tronco
            break;
        case RANA_TANA_APERTA:
            // la rana vince la manche
            break;
        default:
            break;
    }
}
