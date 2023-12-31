#include "collisioni.h"

void checkCollisioni(Collisione* collisione,int startRow,int maxRows,int startCol,int maxCols,Schermo* schermo,PipeData* pipeData,int* id_nemici){
	
	collisione->tipoCollisione=NO_COLLISIONE;
	
	int row=0, col=0;
	bool on_tronco = true;
	
	// controllo collisione rana tronco ovvero la rana sta tutta sul tronco
	if(pipeData->type == 'X'){
		// in pipe c'è la rana
		// scorro matrice per controllare che sia tutta sopra un oggetto tronco
		if(schermo->screenMatrix[startRow][startCol].tipo == TRONCO_OBJ){
			for (int i = 0; i < maxRows; i++) {
				for (int j = 0; j < maxCols; j++) {
					row = startRow + i;
		    	col = startCol + j;
		    	if(schermo->screenMatrix[row][col].tipo != TRONCO_OBJ){
		    		on_tronco = false;
		    	}
  			}
  		}
  		if(on_tronco){
  			// se true allora la rana sta tutta sul tronco
  			
  			collisione->tipoCollisione=RANA_TRONCO; // tipo di collisione
				collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
				collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
				collisione->oggetto_passivo= TRONCO_OBJ; // oggetto che viene colpito
				collisione->id_oggetto_passivo= schermo->screenMatrix[startRow][startCol].id; // id oggetto che viene colpito 
				// in id_oggetto_passivo conservo l'id del tronco sul quale la rana si è poggiata
				return;
  		}
		}
		
	}
	// fine parte controllo collisione intera con tronco
	
	
	
	
	
	
	for (int i = 0; i < maxRows; i++) {
  	for (int j = 0; j < maxCols; j++) {
    	row = startRow + i;
      col = startCol + j;
      
      switch(pipeData->type){
				case 'X': // rana
					switch(schermo->screenMatrix[row][col].tipo){
						case AUTO_OBJ:
							collisione->tipoCollisione=RANA_AUTO; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= AUTO_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito
							return;
							break;
						case CAMION_OBJ:
							collisione->tipoCollisione=RANA_CAMION; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= CAMION_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
							break;
						case FIUME_OBJ:
							collisione->tipoCollisione=RANA_FIUME; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= FIUME_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
							break;
						case N_OBJ:
							collisione->tipoCollisione=RANA_NEMICO; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= N_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito  
							return;
							break;
						case TANA_OPEN_OBJ:
							collisione->tipoCollisione=RANA_TANA_APERTA; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= TANA_OPEN_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
							break;
						case TANA_CLOSE_OBJ:
							collisione->tipoCollisione=RANA_TANA_CHIUSA; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= TANA_CLOSE_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
							break;
						case PN_OBJ:
							collisione->tipoCollisione=RANA_PROIETTILE_NEMICO; // tipo di collisione
							collisione->oggetto_attivo=RANA_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= PN_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
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
								
								collisione->tipoCollisione=NEMICO_RANA; // tipo di collisione
								collisione->oggetto_attivo=N_OBJ; // oggetto che colpisce
								collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
								collisione->oggetto_passivo= RANA_OBJ; // oggetto che viene colpito
								collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito
								return; 
							}
							break;
						case P_OBJ:
							if(pipeData->id == id_nemici[0] || pipeData->id == id_nemici[1] || pipeData->id == id_nemici[2]){
								// il tronco è un nemico in realtà
								collisione->tipoCollisione=NEMICO_PROIETTILE_AMICO; // tipo di collisione
								collisione->oggetto_attivo=N_OBJ; // oggetto che colpisce
								collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
								collisione->oggetto_passivo= P_OBJ; // oggetto che viene colpito
								collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
								return;
							}
							break;
							
						default:
							break;
					}		
					break;
				case 'A': // auto
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							collisione->tipoCollisione=AUTO_RANA; // tipo di collisione
							collisione->oggetto_attivo=AUTO_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= RANA_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
						default:
							break;
					}			
					break;
				case 'C': // camion
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							collisione->tipoCollisione=CAMION_RANA; // tipo di collisione
							collisione->oggetto_attivo=CAMION_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= RANA_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
						default:
							break;
					}			
					break;
				case 'P': // proiettile amico
					switch(schermo->screenMatrix[row][col].tipo){
						case N_OBJ:
							collisione->tipoCollisione=PROIETTILE_AMICO_NEMICO; // tipo di collisione
							collisione->oggetto_attivo=P_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= N_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
							break;
						default:
							break;
					}			
					break;
				case 'p': // proiettile nemico
					switch(schermo->screenMatrix[row][col].tipo){
						case RANA_OBJ:
							collisione->tipoCollisione=PROIETTILE_NEMICO_RANA; // tipo di collisione
							collisione->oggetto_attivo=PN_OBJ; // oggetto che colpisce
							collisione->id_oggetto_attivo=pipeData->id; // id dell'oggetto che colpisce
							collisione->oggetto_passivo= RANA_OBJ; // oggetto che viene colpito
							collisione->id_oggetto_passivo= schermo->screenMatrix[row][col].id; // id oggetto che viene colpito 
							return;
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


// da rifare
void gestisciCollisione(Collisione* collisione, GameData* gameData, int* pipe_fd,int* id_nemici, int* id_tronco_rana, int* pos_x_rel){
	printCollisione(collisione); // per debug
	
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
        	gameData->contatori.vite--;
        	*pos_x_rel=-1;
        	*id_tronco_rana=-1;
        	
        	break;
        case RANA_PROIETTILE_NEMICO:
        	// termina rana e termina proiettile nemico
        	gameData->pids.pidRana= resetRana(pipe_fd,gameData->pipeRana_fd, gameData->pids.pidRana);
        	gameData->contatori.vite--;
        	
        	uccidiProiettileNemico(gameData->pids.pidProiettiliNemici,collisione->id_oggetto_passivo);
        	
        	// cancello l'ggetto proiettile nemico
        	int aus_id=gameData->pipeData.id;
        	
        	// modifico pipeData.id cosi da passare l'id del proiettile nemico
        	gameData->pipeData.id=collisione->id_oggetto_passivo;
        	
        	cancellaOggetto(gameData,gameData->oldPos.proiettiliNemici, PROIETTILE_NEMICO_SPRITE);
        	
        	// ripristino id
        	gameData->pipeData.id=aus_id;
        	
        	// decremento contatore proiettili nemici
        	gameData->contatori.contPN--;
        	*pos_x_rel=-1;
        	*id_tronco_rana=-1;
        	break;
        case PROIETTILE_NEMICO_RANA:
        	// termina rana e termina proiettile nemico
        	gameData->pids.pidRana= resetRana(pipe_fd,gameData->pipeRana_fd, gameData->pids.pidRana);
        	gameData->contatori.vite--;
        	
        	uccidiProiettileNemico(gameData->pids.pidProiettiliNemici,collisione->id_oggetto_attivo);
 					// cancello oggetto proiettile nemico
        	cancellaOggetto(gameData,gameData->oldPos.proiettiliNemici, PROIETTILE_NEMICO_SPRITE);
    
        	gameData->contatori.contPN--;
        	*pos_x_rel=-1;
        	*id_tronco_rana=-1;
        	break;
        case RANA_TRONCO:
        case TRONCO_RANA:
            // la rana si attacca al tronco
            // salvo l'id del tronco sul quale la rana si è poggiata
            *id_tronco_rana= collisione->id_oggetto_passivo;
            // salvo posizione della rana relativa al tronco
            // mi serve posizione assoluta della rana
            int pos_x_rana = gameData->pipeData.x;
            int pos_y_rana = gameData->pipeData.y;
            // mi serve posizione assoluta del tronco su cui la rana è poggiata
            int pos_x_tronco = gameData->oldPos.general[*id_tronco_rana].x;
            int pos_y_tronco = gameData->oldPos.general[*id_tronco_rana].y;
            // calcolo posizione relativa della rana sul tronco
            *pos_x_rel = pos_x_rana - pos_x_tronco;
           
            break;
        case RANA_TANA_APERTA:
            // la rana vince la manche
            // la tana aperta viene chiusa
            gameData->tane[collisione->id_oggetto_passivo].status=TANA_CLOSED;
            gameData->pids.pidRana= resetRana(pipe_fd,gameData->pipeRana_fd, gameData->pids.pidRana);
            *pos_x_rel=-1;
        		*id_tronco_rana=-1;
            break;
         case NEMICO_PROIETTILE_AMICO:
         		// termina nemico
         		killNemico(gameData->pids.pidNemici[collisione->id_oggetto_attivo - 1]); // -1 perchè gli id dei nemici 1-3 mentre l'indice 0-2
         		gameData->pids.pidNemici[collisione->id_oggetto_attivo - 1] = 0;
         		// setta a -1 l'id del nemico nell'array id_nemici
         		//id_nemici[(collisione->id_oggetto_attivo) - 1]=-1;
         		if(id_nemici[0]==collisione->id_oggetto_attivo){id_nemici[0]=-1;}
         		if(id_nemici[1]==collisione->id_oggetto_attivo){id_nemici[1]=-1;}
         		if(id_nemici[2]==collisione->id_oggetto_attivo){id_nemici[2]=-1;}
         		gameData->contatori.contN--; // decremento nemici
         		// termina proiettile amico
         		// uccide il processo proiettile corrispondente all' id passato
         		uccidiProiettile(gameData->pids.pidProiettili,collisione->id_oggetto_passivo);
         		gameData->contatori.contP--;
         		
         		break;
         case PROIETTILE_AMICO_NEMICO:
         		// termina nemico
         		killNemico(gameData->pids.pidNemici[collisione->id_oggetto_passivo - 1]); // -1 perchè gli id dei nemici 1-3 mentre l'indice 0-2
         		gameData->pids.pidNemici[collisione->id_oggetto_passivo - 1] = 0;
         		// setta a -1 l'id del nemico nell'array id_nemici
         		//id_nemici[collisione->id_oggetto_passivo - 1]=-1;
         		if(id_nemici[0]==collisione->id_oggetto_passivo){id_nemici[0]=-1;}
         		if(id_nemici[1]==collisione->id_oggetto_passivo){id_nemici[1]=-1;}
         		if(id_nemici[2]==collisione->id_oggetto_passivo){id_nemici[2]=-1;}
         		gameData->contatori.contN--; // decremento nemici
         		// termina proiettile amico
         		// uccide il processo proiettile corrispondente all' id passato
         		uccidiProiettile(gameData->pids.pidProiettili,collisione->id_oggetto_attivo);
         		gameData->contatori.contP--;
         	break;
        default:
            break;
    }
}
