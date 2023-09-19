#include "schermo.h"
void aggiorna(GameData* gameData,int* pipe_fd){

	switch(gameData->pipeData.type){
    	case 'X': // rana
    		aggiornaOggetto(&gameData, &(gameData->old_pos->general), RANA_SPRITE);
        break;
			case 'T': // tronco
        aggiornaOggetto(&gameData, &(gameData->old_pos->general), TRONCOSPRITE);
        
        //aggiornaDirezioneTronchi( &pipeData, &old_pos[pipeData.id], arrayDirTronchi); // da controllare
        break;
      case 'A': // auto
      	aggiornaOggetto(&gameData, &(gameData->old_pos->general), AUTOSPRITE);
        break;
     case 'C':  // camion
      	aggiornaOggetto(&gameData, &(gameData->old_pos->general), CAMIONSPRITE);
        break;
      case 'S':
      	//proiettile sparato da utente avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
      	if(gameData->cont.contP<MAXNPROIETTILI)	// se si hanno ancora munizioni
      	{ 
      		// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
      		int id = id_disponibile(pids.pidProiettili,MAXNPROIETTILI);
      		
      		gameData->pids.pidProiettili[id]=avviaProiettile(pipe_fd, &(gameData.pipeData), id);
      		
      		gameData->cont.contP++;
      	}
      	break;
      	//// SONO QUI
      case 'n':
      	if(contatore_nemici_in_gioco<MAXNNEMICI)  // se non si è raggiunto il numero massimo di nemici
      	{ 
      		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
      		int id = id_disponibile(pids.pidNemici,MAXNNEMICI);
	      	pids.pidNemici[id]=avviaNemico(pipe_fd,&pipeData, id);
  				contatore_nemici_in_gioco++;
      	}
      	break;
      case 's':
      	// proiettile nemico sparato
      	if(contatore_proiettili_nemici_in_gioco<MAXNPROIETTILINEMICI) // se non si è raggiunto il numero massimo di nemici
      	{ 
      		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
      		int id = id_disponibile(pids.pidProiettiliNemici,MAXNPROIETTILINEMICI);
      		pids.pidProiettiliNemici[id]= avviaProiettileNemico(pipe_fd, &pipeData, id);
      		contatore_proiettili_nemici_in_gioco++;
      	}
      	break;
      case 'P':
      	// nuove coordinate proiettile se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	if(pipeData.y<0){
      		uccidiProiettile( pids.pidProiettili, pipeData.id); // uccide il processo proiettile
      		cancellaOggetto(old_pos_proiettili, &proiettileSprite, screenMatrix, staticScreenMatrix, pipeData.id);
      		contatore_proiettili_in_gioco--;
      	}
      	else{
      		aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileSprite);
      	}
      	break;
      case 'p':
      	// nuove coordinate proiettile nemico se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	if(pipeData.y>30){
      		uccidiProiettileNemico( pids.pidProiettiliNemici, pipeData.id); // uccide il processo proiettile
      		cancellaOggetto(old_pos_proiettili_nemici, &proiettileNemicoSprite, screenMatrix, staticScreenMatrix, pipeData.id);
      		contatore_proiettili_nemici_in_gioco--;
      	}
      	else{
      		aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili_nemici[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileNemicoSprite);
      	}
      	break;
      	case 'Z':
      		beep();
      		// in pausa
      		// funzione che visualizza menu // exit del menu -> intero con scelta // gli vanno passati tutti i pid che deve mettere in pausa
      		//int scelta = pausa(pidRana);
      		stampaRefreshMatrice(screenMatrix);
      		break;
      default:
        break;
    }

	return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaOggetto(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite) {
		PipeData* datiNuovi = &(gameData->pipeData); // i dati nuovi passati in pipe
		PipeData* datiVecchi = &(old_pos[tipoSprite]); // dati al passo precedentes
		
    // se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y) {
    		
        pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), &(gameData->schermo));
        
        stampaSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), &(gameData->schermo));
        
        aggiornaOldPos(datiVecchi, datiNuovi);
    }
}
void stampaSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo) {
    int startRow=datiVecchi->y;
    int startCol=datiVecchi->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;

    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            int row = startRow + i;
            int col = startCol + j;

            schermo->screenMatrix[row][col].ch = sprite->sprite[i][j];
            schermo->screenMatrix[row][col].color = sprite->color;
            schermo->screenMatrix[row][col].is_changed = true;
        }
    }
}
void pulisciSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo) {
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
            }
        }
    }
}
//--------------------------------------------Stampa Puntuale----------------------------------------------------------------------
void stampaMatrice( ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
		// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
		// il flag sara TRUE solo se il carattere è stato modificato
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
			// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE il flag sara TRUE solo se il carattere è stato modificato
							
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attroff(COLOR_PAIR(screenMatrix[i][j].color));
			screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			
		}
	}
	return;
}
// cancella la sprite dell'oggetto dalle matrici e lo 'disattiva' (type = ' ')
void cancellaOggetto(PipeData *array_oggetti, Sprite* sprite_oggetto, 
											ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], int id_oggetto)
{
	if(id_oggetto >= 0) // se l'id è un indice di array valido
	{
		pulisciSpriteInMatrice(array_oggetti[id_oggetto].y, array_oggetti[id_oggetto].x, sprite_oggetto, screenMatrix, staticScreenMatrix);
		array_oggetti[id_oggetto].type = ' ';
		array_oggetti[id_oggetto].x = 0;
		array_oggetti[id_oggetto].y = 0;
	}
}
