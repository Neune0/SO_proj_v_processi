#include "schermo.h"
void aggiorna(GameData* gameData,int* pipe_fd, int* id_nemici,int* id_rana_tronco,int* pos_x_rel){
	
	switch(gameData->pipeData.type){
    	case 'X': // rana
    		mvprintw(35,0,"                                                        ");
    		mvprintw(35,0,"rana x: %d y: %d",gameData->pipeData.x,gameData->pipeData.y);
    		mvprintw(36,0,"                                                        ");
    		mvprintw(36,0,"old rana x: %d y: %d",gameData->oldPos.general[0].x,gameData->oldPos.general[0].y);
    		refresh();
    		// se la rana è sul tronco ed il movimento è orizzontale e non sforo il tronco allora devo fare come sotto con caso tronco che si muove
    		if(*id_rana_tronco!=-1){
    			
    			// se il movimento è orizzonatale
    			if(gameData->oldPos.general[0].x != gameData->pipeData.x){
    				
    				// se non sforo il tronco
    				// mi serve la posizione del tronco su cui è la rana
    				int pos_x_tronco = gameData->oldPos.general[*id_rana_tronco].x;
    				// se nuova posizione della rana quello scritto in pipe è minore di pos x del tronco sto sforando
    				// se nuova posizione della rana è maggiore di pos x tronco + 8 sto sforando
    				int diff = gameData->pipeData.x - pos_x_tronco;
    				if( diff>=0 && diff <=7){
    					// non sforo
    					mvprintw(39,0,"                                   ");
    					mvprintw(39,0,"spostamento orizzontale lecito");
    					refresh();
    					// come spostare la rana 
    					// stampo sprite tronco
    					PipeData aus;
    					aus.x=gameData->pipeData.x;
    					aus.y=gameData->pipeData.y;
    					aus.type=gameData->pipeData.type;
    					aus.id=gameData->pipeData.id;
    					
    					gameData->pipeData.x=gameData->oldPos.general[*id_rana_tronco].x;
    					gameData->pipeData.y=gameData->oldPos.general[*id_rana_tronco].y;
    					gameData->pipeData.type=gameData->oldPos.general[*id_rana_tronco].type;
    					gameData->pipeData.id=gameData->oldPos.general[*id_rana_tronco].id;
    					
    					aggiornaOggettoMod(gameData, gameData->oldPos.general, TRONCO_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
    					
    					gameData->pipeData.x=aus.x;
    					gameData->pipeData.y=aus.y;
    					gameData->pipeData.type=aus.type;
    					gameData->pipeData.id=aus.id;
    					
    					// aggiorno la pos_x_rel
    					*pos_x_rel=diff;
    					
    					
    					// stampo sprite rana
    					aggiornaOggettoMod(gameData, gameData->oldPos.general, RANA_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
							
    					
    				}
    				else{
    					// sforo
    					mvprintw(39,0,"                                    ");
    					mvprintw(39,0,"spostamento orizzontale NON lecito");
    					refresh();
    					// scrivi in pipe alla rana la sua posizione vecchia
    					// usando pos tronco e pos rana relativa a tronco scrivo in pipe verso la rana le coordinate aggiornate
							PipeData aus_pipe;
							aus_pipe.x= gameData->oldPos.general[*id_rana_tronco].x + *pos_x_rel; // posizione rana assoluta = postronco + posranarelativa
							aus_pipe.y= gameData->oldPos.general[*id_rana_tronco].y; // pos rana assoluta = old pos del tronco
							aus_pipe.type='X';
							aus_pipe.id=0;
						
							write(gameData->pipeRana_fd[1], &aus_pipe,sizeof(PipeData));
    				}
    				
    				
    				
    			}
    			// movimento lungo y
    				if(gameData->oldPos.general[0].y != gameData->pipeData.y){
    					// 3 casi
    					//1. se cade in acqua
    					//2. se atterra su un altro tronco
    					//3. se atterra su una tana chiusa
    					//4. se atterra su una tana aperta
    					// devo chiamare aggiornaOggetto normalmente
    					mvprintw(38,0,"                  ");
    					mvprintw(38,0,"spostamento su y");
    					refresh();
    					*id_rana_tronco=-1;
    					*pos_x_rel=-1;
    					//usleep(3000000);
    					aggiornaOggetto(gameData, gameData->oldPos.general, RANA_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
    				}
    		}
    		else{
    				
    					mvprintw(38,0,"                                ");
    					refresh();
    			// normale aggiornamento la rana non è su un tronco
    			aggiornaOggetto(gameData, gameData->oldPos.general, RANA_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
    		}
    		// se la rana è sul tronco ed il movimento è orizziontale e sforo allora non devo muovere la rana e devo scrivere in pipe alla rana la posizione prima del movimento
    		// se la rana è sul tronco ed il movimento è verticale
    		
    		
        break; 
			case 'T': // tronco
			
				if(gameData->pipeData.id == id_nemici[0] || gameData->pipeData.id == id_nemici[1] || gameData->pipeData.id == id_nemici[2]){
					// allora è un nemico
					aggiornaOggetto(gameData, gameData->oldPos.general, NEMICO_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
				}
				else{
					if(gameData->pipeData.id==*id_rana_tronco){
						mvprintw(34,0,"la rana è sul tronco con id: %d",*id_rana_tronco);
						refresh();
						// allora il tronco trasporta la rana
						// stampo sprite tronco
						aggiornaOggetto(gameData, gameData->oldPos.general, TRONCO_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
						
						// usando pos tronco e pos rana relativa a tronco scrivo in pipe verso la rana le coordinate aggiornate
						PipeData aus_pipe;
						aus_pipe.x= gameData->oldPos.general[*id_rana_tronco].x + *pos_x_rel; // posizione rana assoluta = postronco + posranarelativa
						aus_pipe.y= gameData->oldPos.general[*id_rana_tronco].y; // pos rana assoluta = old pos del tronco
						aus_pipe.type='X';
						aus_pipe.id=0;
						
						write(gameData->pipeRana_fd[1], &aus_pipe,sizeof(PipeData));
						
						// stampo lo sprite della rana
						// salvo per ripristino pipedata
						PipeData aus_pipe2;
						aus_pipe2.x= gameData->pipeData.x;
						aus_pipe2.y= gameData->pipeData.y;
						aus_pipe2.type=gameData->pipeData.type;
						aus_pipe2.id=gameData->pipeData.id;
						
						// modifico gameData.pipeData
						gameData->pipeData.x = aus_pipe.x;
						gameData->pipeData.y = aus_pipe.y;
						gameData->pipeData.type = aus_pipe.type;
						gameData->pipeData.id = aus_pipe.id;
						
						aggiornaOggettoMod(gameData, gameData->oldPos.general, RANA_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
						
						

						// ripristino
						gameData->pipeData.x = aus_pipe2.x;
						gameData->pipeData.y = aus_pipe2.y;
						gameData->pipeData.type = aus_pipe2.type;
						gameData->pipeData.id = aus_pipe2.id;
						
					}
					else{
						// allora è un tronco normale
						aggiornaOggetto(gameData, gameData->oldPos.general, TRONCO_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
					}
				}
        
        //aggiornaDirezioneTronchi( &pipeData, &old_pos[pipeData.id], arrayDirTronchi); // da controllare
        break;
      case 'A': // auto
      	aggiornaOggetto(gameData, gameData->oldPos.general, AUTO_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
        break;
     case 'C':  // camion
      	aggiornaOggetto(gameData, gameData->oldPos.general, CAMION_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
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
      	if(gameData->pipeData.id!=*id_rana_tronco){
      	if(gameData->pipeData.id!=id_nemici[0] && gameData->pipeData.id!=id_nemici[1] && gameData->pipeData.id!=id_nemici[2]){
		    	if(gameData->contatori.contN<MAXNNEMICI)  // se non si è raggiunto il numero massimo di nemici
		    	{ 
		    		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
		    		//int id = id_disponibile(gameData->pids.pidNemici,MAXNNEMICI);
			    	int id = gameData->pipeData.id; // questo sarà l'id del nemico che è uguale all'id del tronco che l'ha generato
			    	gameData->pids.pidNemici[id - 1]=avviaNemico(pipe_fd, id);
						// devo comunicare a disegna che la prossima volta che leggera T in pipe e l'id sarà uguale all'id che ho qui allora dovra disegnare il tronco in un modo diverso ed anche il tipo in matrice dovra essere diverso
						// uso un array tronchi nemici di id
						id_nemici[gameData->contatori.contN]=id;
						gameData->contatori.contN++;		
		    	}
      	}}
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
      	if(gameData->pipeData.y<4){
      		uccidiProiettile( gameData->pids.pidProiettili, gameData->pipeData.id); // uccide il processo proiettile
      		// ucciso processo proiettile e impostato a zero il pid in array pid proiettili
      		
      		cancellaOggetto(gameData,gameData->oldPos.proiettili, PROIETTILE_SPRITE);
      		gameData->contatori.contP--;
      	}
      	else{
      		aggiornaOggetto(gameData, gameData->oldPos.proiettili, PROIETTILE_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
      	}
      	break;
      case 'p':
      	// nuove coordinate proiettile nemico se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	if(gameData->pipeData.y>31){
      		uccidiProiettileNemico( gameData->pids.pidProiettiliNemici, gameData->pipeData.id); // uccide il processo proiettile
      		// da fare cancella oggetto
      		cancellaOggetto(gameData,gameData->oldPos.proiettiliNemici, PROIETTILE_NEMICO_SPRITE);
      		gameData->contatori.contPN--;
      	}
      	else{
      		aggiornaOggetto(gameData, gameData->oldPos.proiettiliNemici, PROIETTILE_NEMICO_SPRITE,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
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
void aggiornaOggetto(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite, int* pipe_fd,int* id_nemici,int* id_rana_tronco, int* pos_x_rel) { //ok
		PipeData* datiNuovi = &(gameData->pipeData); // i dati nuovi passati in pipe
		PipeData* datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes
		
    // se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y) {
    		
        pulisciSpriteInMatrice(datiVecchi, &(gameData->sprites[tipoSprite]), &(gameData->schermo));
        
        stampaSpriteInMatrice(datiNuovi, &(gameData->sprites[tipoSprite]), &(gameData->schermo), &(gameData->pipeData), gameData, pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
        
        
        aggiornaOldPos(datiVecchi, datiNuovi);
    }
}
void aggiornaOggettoMod(GameData* gameData, PipeData* old_pos, TipoSprite tipoSprite, int* pipe_fd,int* id_nemici,int* id_rana_tronco, int* pos_x_rel){
	PipeData* datiNuovi = &(gameData->pipeData); // i dati nuovi passati in pipe
	PipeData* datiVecchi = &(old_pos[gameData->pipeData.id]); // dati al passo precedentes
	// se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (datiNuovi->x != datiVecchi->x || datiNuovi->y != datiVecchi->y) {
    		
        stampaSpriteInMatriceMod(datiNuovi, &(gameData->sprites[tipoSprite]), &(gameData->schermo), &(gameData->pipeData), gameData, pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
        
        
        aggiornaOldPos(datiVecchi, datiNuovi);
    }


}
void stampaSpriteInMatrice(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo, PipeData* pipeData,GameData* gameData,int *pipe_fd,int* id_nemici, int* id_rana_tronco, int* pos_x_rel) { // ok
    int startRow=datiVecchi->y;
    int startCol=datiVecchi->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    int row=0, col=0;
    // funizone che controlla se ci saranno collisioni restituisce il tipo di collisine, se non c'è collisione restituisce NO_COLLSIONE
    // se ci sono più collisioni allora serve una policy su quale restituire
    // gli si passa row iniziale e finale + col inziale e fiinale + schermo per matrice dinamica + pipeData
    Collisione collisione;
    
    checkCollisioni(&collisione,startRow,maxRows,startCol,maxCols,schermo,pipeData,id_nemici);
    
    gestisciCollisione(&collisione,gameData,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
    // dopo questo se ho una collisione rana tronco
    // ho salvato in pos_x_rel la posizione della rana relativa al tronco
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
								if(sprite->sprite[0][4]=='^'){
									schermo->screenMatrix[row][col].tipo = N_OBJ;
								}
								else{
									schermo->screenMatrix[row][col].tipo = TRONCO_OBJ;
								}
								
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
void stampaSpriteInMatriceMod(PipeData* datiVecchi, Sprite* sprite, Schermo* schermo, PipeData* pipeData,GameData* gameData,int *pipe_fd,int* id_nemici, int* id_rana_tronco, int* pos_x_rel) { // ok
    int startRow=datiVecchi->y;
    int startCol=datiVecchi->x;
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    int row=0, col=0;
    // funizone che controlla se ci saranno collisioni restituisce il tipo di collisine, se non c'è collisione restituisce NO_COLLSIONE
    // se ci sono più collisioni allora serve una policy su quale restituire
    // gli si passa row iniziale e finale + col inziale e fiinale + schermo per matrice dinamica + pipeData
    //Collisione collisione;
    
    //checkCollisioni(&collisione,startRow,maxRows,startCol,maxCols,schermo,pipeData,id_nemici);
    
    //gestisciCollisione(&collisione,gameData,pipe_fd,id_nemici,id_rana_tronco,pos_x_rel);
    // dopo questo se ho una collisione rana tronco
    // ho salvato in pos_x_rel la posizione della rana relativa al tronco
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
								if(sprite->sprite[0][4]=='^'){
									schermo->screenMatrix[row][col].tipo = N_OBJ;
								}
								else{
									schermo->screenMatrix[row][col].tipo = TRONCO_OBJ;
								}
								
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
    int maxRows = sprite->max_row; // 2
    int maxCols = sprite->max_col; // 3
    
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

