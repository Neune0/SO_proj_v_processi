#include "schermo.h"
void aggiorna(GameData* gameData,int* pipe_fd, int* id_nemici){
	mvprintw(35,0,"                                                                          ");
	mvprintw(35,0,"id primo tronco: %d, id secondo tronco: %d, id terzo tronco: %d",gameData->oldPos.general[1].id,gameData->oldPos.general[2].id,gameData->oldPos.general[3].id);
	mvprintw(36,0,"                                                                          ");
	mvprintw(36,0,"id primo nemico: %d, id secondo nemico: %d, id terzo nemico: %d",id_nemici[0],id_nemici[1],id_nemici[2]);
	refresh();
	switch(gameData->pipeData.type){
    	case 'X': // rana
    		aggiornaOggetto(gameData, gameData->oldPos.general, RANA_SPRITE,pipe_fd);
        break; 
			case 'T': // tronco
				if(gameData->pipeData.id == id_nemici[0] || gameData->pipeData.id == id_nemici[1] || gameData->pipeData.id == id_nemici[2]){
					aggiornaOggetto(gameData, gameData->oldPos.general, NEMICO_SPRITE,pipe_fd);
				}
				else{
					aggiornaOggetto(gameData, gameData->oldPos.general, TRONCO_SPRITE,pipe_fd);
				}
        
        //aggiornaDirezioneTronchi( &pipeData, &old_pos[pipeData.id], arrayDirTronchi); // da controllare
        break;
      case 'A': // auto
      	aggiornaOggetto(gameData, gameData->oldPos.general, AUTO_SPRITE,pipe_fd);
        break;
     case 'C':  // camion
      	aggiornaOggetto(gameData, gameData->oldPos.general, CAMION_SPRITE,pipe_fd);
        break;
      case 'S':
      	//proiettile sparato da utente avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
      	if(gameData->contatori.contP<MAXNPROIETTILI)	// se si hanno ancora munizioni
      	{ 
      		// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
      		int id = id_disponibile(gameData->pids.pidProiettili,MAXNPROIETTILI);
      		
      		gameData->pids.pidProiettili[id]=avviaProiettile(pipe_fd, &(gameData->pipeData), id);
      		
      		gameData->contatori.contP++;
      	}
      	break;
      case 'n':
      mvprintw(34,0,"                                              ");
      	mvprintw(34,0,"%d != %d && %d != %d && %d != %d",gameData->pipeData.id,id_nemici[0], gameData->pipeData.id,id_nemici[1] ,gameData->pipeData.id,id_nemici[2]);
      	refresh();
      	if(gameData->pipeData.id!=id_nemici[0] && gameData->pipeData.id!=id_nemici[1] && gameData->pipeData.id!=id_nemici[2]){
		    	if(gameData->contatori.contN<MAXNNEMICI)  // se non si è raggiunto il numero massimo di nemici
		    	{ 
		    		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
		    		//int id = id_disponibile(gameData->pids.pidNemici,MAXNNEMICI);
			    	int id = gameData->pipeData.id; // questo sarà l'id del nemico che è uguale all'id del tronco che l'ha generato
			    	gameData->pids.pidNemici[id]=avviaNemico(pipe_fd, id);
						// devo comunicare a disegna che la prossima volta che leggera T in pipe e l'id sarà uguale all'id che ho qui allora dovra disegnare il tronco in un modo diverso ed anche il tipo in matrice dovra essere diverso
						// uso un array tronchi nemici di id
						id_nemici[gameData->contatori.contN]=id;
						gameData->contatori.contN++;		
		    	}
      	}
      	break;
      case 's':
      	// LE COORDINATE DI LANCIO DEVONO ESSERE QUELLE CENTRALI DEL TRONCO HA ID = A ID IN PIPEDATA, PRENDERE DA OLD POS TRONCHI
      	// proiettile nemico sparato
      	if(gameData->contatori.contPN<MAXNPROIETTILINEMICI) // se non si è raggiunto il numero massimo di nemici
      	{ 
      		// pipeData.id = id del tronco che ha sparato
      		
      		// trova old pos tronco con id = pipeData.id
      		PipeData* oldPosPipeData = &(gameData->oldPos.general[gameData->pipeData.id]); // questa è la old pos che devo passare
      		oldPosPipeData->x+=4;
      		
      		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
      		int id = id_disponibile(gameData->pids.pidProiettiliNemici,MAXNPROIETTILINEMICI);
      		
      		gameData->pids.pidProiettiliNemici[id]= avviaProiettileNemico(pipe_fd, oldPosPipeData, id);
      		gameData->contatori.contPN++;
      	}
      	break;
      case 'P':
      	// nuove coordinate proiettile se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	if(gameData->pipeData.y<0){
      		uccidiProiettile( gameData->pids.pidProiettili, gameData->pipeData.id); // uccide il processo proiettile
      		// da fare cancella oggetto
      		cancellaOggetto(gameData,gameData->oldPos.proiettili, PROIETTILE_SPRITE);
      		gameData->contatori.contP--;
      	}
      	else{
      		aggiornaOggetto(gameData, gameData->oldPos.proiettili, PROIETTILE_SPRITE,pipe_fd);
      	}
      	break;
      case 'p':
      	// nuove coordinate proiettile nemico se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	if(gameData->pipeData.y>30){
      		uccidiProiettileNemico( gameData->pids.pidProiettiliNemici, gameData->pipeData.id); // uccide il processo proiettile
      		// da fare cancella oggetto
      		cancellaOggetto(gameData,gameData->oldPos.proiettiliNemici, PROIETTILE_NEMICO_SPRITE);
      		gameData->contatori.contPN--;
      	}
      	else{
      		aggiornaOggetto(gameData, gameData->oldPos.proiettiliNemici, PROIETTILE_NEMICO_SPRITE,pipe_fd);
      	}
      	break;
      	case 'Z':
      		beep();
      		// in pausa
      		// funzione che visualizza menu // exit del menu -> intero con scelta // gli vanno passati tutti i pid che deve mettere in pausa
      		//int scelta = pausa(pidRana);
      		stampaRefreshMatrice(gameData->schermo.screenMatrix);
      		break;
      		
      default:
        break;
    }

	return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaOggetto(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite, int* pipe_fd) { //ok
		PipeData* datiNuovi = &(gameData->pipeData); // i dati nuovi passati in pipe
		PipeData* datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes
		
    // se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y) {
    		
        pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), &(gameData->schermo));
        
        stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), &(gameData->schermo), &(gameData->pipeData), gameData, pipe_fd);
        
        aggiornaOldPos(datiVecchi, datiNuovi);
    }
}
void stampaSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo, PipeData* pipeData,GameData* gameData,int *pipe_fd) { // ok
    int startRow=datiVecchi->y;
    int startCol=datiVecchi->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    int row=0, col=0;
    // funizone che controlla se ci saranno collisioni restituisce il tipo di collisine, se non c'è collisione restituisce NO_COLLSIONE
    // se ci sono più collisioni allora serve una policy su quale restituire
    // gli si passa row iniziale e finale + col inziale e fiinale + schermo per matrice dinamica + pipeData
    Collisione collisione;
    checkCollisioni(&collisione,startRow,maxRows,startCol,maxCols,schermo,pipeData);
    
    // qui switch su tipo collisione
    //if(tipoCollisione!=NO_COLLISIONE){
    	gestisciCollisione(&collisione,gameData,pipe_fd);
    //}
    //else{}
    	for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            row = startRow + i;
            col = startCol + j;
						
						
						// devo fare lo switch sul tipo per assegnare il tipo
						switch(pipeData->type){
							case 'X': // rana
								schermo->screenMatrix[row][col].tipo = RANA_OBJ;
								break;
							case 'T': // tronco
								schermo->screenMatrix[row][col].tipo = TRONCO_OBJ;
								break;
							case 'A': // auto
								schermo->screenMatrix[row][col].tipo = AUTO_OBJ;
								break;
							case 'C': // camion
								schermo->screenMatrix[row][col].tipo = CAMION_OBJ;
								break;
							case 'P': // proiettile amico
								schermo->screenMatrix[row][col].tipo = P_OBJ;
								break;
							case 'p': // proiettile nemico
								schermo->screenMatrix[row][col].tipo = PN_OBJ;
								break;
							default:
								break;
						}
						
						// l'id da scrivere è quello in pipeData
					
            schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
            schermo->screenMatrix[row][col].color = sprite->color;
            schermo->screenMatrix[row][col].is_changed = true;
            schermo->screenMatrix[row][col].id=pipeData->id;
        }
    }
    
    
    
    
    
    
}
void pulisciSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo) { // ok
    int row=datiVecchi->y;
    int col=datiVecchi->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    if (row != -1) {
        for (int i = row; i < row + maxRows; i++) {
            for (int j = col; j < col + maxCols; j++) {
                schermo->screenMatrix[i][j].ch = schermo->staticScreenMatrix[i][j].ch;
                schermo->screenMatrix[i][j].color = schermo->staticScreenMatrix[i][j].color;
                schermo->screenMatrix[i][j].is_changed = true;
                schermo->screenMatrix[i][j].tipo = schermo->staticScreenMatrix[i][j].tipo;
                schermo->screenMatrix[i][j].id = schermo->staticScreenMatrix[i][j].id;
            }
        }
    }
}
//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]){//ok
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
		// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE (è stato modificato)
			if(screenMatrix[i][j].is_changed){							
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				mvaddch(i, j, screenMatrix[i][j].ch);
				attroff(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			}
		}
	}
	return;
}
void stampaRefreshMatrice( ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attroff(COLOR_PAIR(screenMatrix[i][j].color));
			screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
		}
	}
	return;
}
// cancella la sprite dell'oggetto dalle matrici e lo 'disattiva' (type = ' ')
void cancellaOggetto(GameData* gameData, PipeData *old_pos, TipoSprite tipoSprite){
	PipeData* datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedente
	int id= gameData->pipeData.id;
	if(id >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(datiVecchi,&(gameData->sprites[tipoSprite]), &(gameData->schermo));
		datiVecchi[id].type = ' ';
		datiVecchi[id].x = 0;
		datiVecchi[id].y = 0;
	}
	return;
}
