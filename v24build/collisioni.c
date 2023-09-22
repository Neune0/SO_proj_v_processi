#include "collisioni.h"

void checkCollisioni(Collisione* collisione,int startRow,int maxRows,int startCol,int maxCols,Schermo* schermo,PipeData* pipeData,int* id_nemici){
	
	collisione->tipoCollisione=NO_COLLISIONE;
	collisione->id=0;
	
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
							collisione->tipoCollisione=RANA_AUTO;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case CAMION_OBJ:
							collisione->tipoCollisione=RANA_CAMION;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case FIUME_OBJ:
							collisione->tipoCollisione=RANA_FIUME;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case TRONCO_OBJ:
							collisione->tipoCollisione=RANA_TRONCO;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case N_OBJ:
							collisione->tipoCollisione=RANA_NEMICO;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case TANA_OPEN_OBJ:
							collisione->tipoCollisione=RANA_TANA_APERTA;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case TANA_CLOSE_OBJ:
							collisione->tipoCollisione=RANA_TANA_CHIUSA;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						case PN_OBJ:
							collisione->tipoCollisione=RANA_PROIETTILE_NEMICO;
							collisione->id=schermo->screenMatrix[row][col].id; 
							break;
						default:
							break;
					}		
					break;
				case 'T': // tronco
					// SE IL TRONCO È IN REALTA UN NEMICO OVVERO IL SUO ID STA IN ID_NEMICI
					
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							if(pipeData->id == id_nemici[0] || pipeData->id == id_nemici[1] || pipeData->id == id_nemici[2]){
								// il tronco è un nemico
								collisione->tipoCollisione=NEMICO_RANA;
								collisione->id=pipeData->id; 
							}else{
								collisione->tipoCollisione=TRONCO_RANA;
								collisione->id=pipeData->id; 
							}
							break;
						case P_OBJ:
							if(pipeData->id == id_nemici[0] || pipeData->id == id_nemici[1] || pipeData->id == id_nemici[2]){
								// il tronco è un nemico in realtà
								collisione->tipoCollisione=NEMICO_PROIETTILE_AMICO;
								collisione->id=pipeData->id; 
							}
							break;
							
						default:
							break;
					}		
					break;
				case 'A': // auto
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							collisione->tipoCollisione=AUTO_RANA;
							collisione->id=pipeData->id; 
						default:
							break;
					}			
					break;
				case 'C': // camion
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							collisione->tipoCollisione=CAMION_RANA;
							collisione->id=pipeData->id; 
						default:
							break;
					}			
					break;
				case 'P': // proiettile amico
					switch(schermo->screenMatrix[row][col].tipo){
						case N_OBJ:
							collisione->tipoCollisione=PROIETTILE_AMICO_NEMICO;
							collisione->id=pipeData->id; 
						default:
							break;
					}			
					break;
				case 'p': // proiettile nemico
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							collisione->tipoCollisione=PROIETTILE_NEMICO_RANA;
							collisione->id=pipeData->id; 
						default:
							break;
					}			
					break;
				default:
					break;
			}
    }
  }
  return;
}

void gestisciCollisione(Collisione* collisione, GameData* gameData, int* pipe_fd){
	printCollisione(collisione->tipoCollisione); // per debug
	// switch su collisione:
	switch (collisione->tipoCollisione) {
        case RANA_AUTO:
        case RANA_CAMION:
        case RANA_FIUME:
        case RANA_NEMICO:
        case RANA_TANA_CHIUSA:
        case AUTO_RANA:
        case CAMION_RANA:
        case NEMICO_RANA:
        	// termina rana
        	gameData->pids.pidRana= resetRana(pipe_fd,gameData->pipeRana_fd, gameData->pids.pidRana);
        	break;
        case RANA_PROIETTILE_NEMICO:
        case PROIETTILE_NEMICO_RANA:
        	// termina rana e termina proiettile nemico
        	gameData->pids.pidRana= resetRana(pipe_fd,gameData->pipeRana_fd, gameData->pids.pidRana);
        	uccidiProiettileNemico(gameData->pids.pidProiettiliNemici,collisione->id);
					break;
        case RANA_TRONCO:
        case TRONCO_RANA:
            // la rana si attacca al tronco
            break;
        case RANA_TANA_APERTA:
            // la rana vince la manche
            break;
         case NEMICO_PROIETTILE_AMICO:
         case PROIETTILE_AMICO_NEMICO:
         	// termina nemico e rispristina sprite del tronco
         	break;
        default:
            break;
    }
}
