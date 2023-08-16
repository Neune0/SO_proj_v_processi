#include "disegna.h"
// per far partire il processo che si occupa di disegnare
void avviaDrawProcess(int pipe_fd[2]) {
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (draw_pid == 0) {
        // Processo "disegna"
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe, TODO: è davvero utile?
        drawProcess(pipe_fd);
        exit(0); // TODO: è davvero utile?
    }
}

// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
		
	PipeData pipeData; // struttura per leggere la pipe
	PipeData old_pos [OLDPOSDIM]; // array di strutture che contiene dati oggetti al passo precedente; rana, tronchi x3, auto x4, ...
	inizializzaOldPos(old_pos,OLDPOSDIM);
	
	// definizione sprite utili
	char *sprite_rana[RANA_ROWS] = {"\\./","/-\\"};
	char *sprite_tronco[TRONCO_ROWS] = {"~ ~ ~ ~ ~", " ~ ~ ~ ~ "};
	char *sprite_auto[AUTO_ROWS] = {"xxxx", "oooo"};
	char *sprite_camion[CAMION_ROWS] = {"xxxxxxx", "ooooooo"};
	
	Sprite ranaSprite = {RANA_ROWS, RANA_COLS, sprite_rana, RANA};
  	Sprite troncoSprite = {TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI};
	Sprite autoSprite = {AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO};
  	Sprite camionSprite = {CAMION_ROWS, CAMION_COLS, sprite_camion, AUTO};
  	
  	ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
	inizializzaMatriceSchermo(screenMatrix, staticScreenMatrix); // inizializza entrambe le matrici
	
	
 	while (1) {
  		read(pipe_fd[0], &pipeData, sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
        
		switch(pipeData.type){
			case 'X':
				aggiornaPosizioneOggetto(&pipeData, &old_pos[0], screenMatrix, staticScreenMatrix, &ranaSprite);
				
		    	break;
			case 'T':
		    	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix,staticScreenMatrix, &troncoSprite);
		    	break;
	  		case 'A':
		  		aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &autoSprite);
		    	break;
		    
		 	case 'C':  // legge il camion da pipe e aggiorna posizione
		  		aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &camionSprite);
		    	break;
		  	case 'c':
		  	// NON disegnare il camion per un certo tempo, quando esce dallo schermo
		 		break;
		  	case 'S':
		  	//proiettile sparato da utente
		  	// avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
		  		avviaProiettile(pipe_fd, &pipeData); 
		  		beep();
		  	// fa partire il processo proiettile se il numero di proiettili in gioco è minore di 10
		  	break;
		  	case 'P':
		  	// nuove coordinate proiettile
		  		break;
		  	default:
	    		break;
		}//end switch-case
    
		bool frogCollision = false;
		//----collisioni rana------
	  	for(int i=1; i<OLDPOSDIM; i++){
			//check_1 = checkCollisione(&old_pos[i], &old_pos[0], &ranaSprite);
	 		if(i<4){
	 			frogCollision =checkCollisione(&old_pos[0], &old_pos[i], &ranaSprite, &troncoSprite);
		 	}
		 	if(i>=4){							// check su auto/camion
		 		if(old_pos[i].type=='A')
		 		{
	 				frogCollision = checkCollisione(&old_pos[0], &old_pos[i], &ranaSprite, &autoSprite);
				}else if (old_pos[i].type=='C')
				{
					frogCollision = checkCollisione(&old_pos[0], &old_pos[i], &ranaSprite, &camionSprite);
		 		}
		 	}
			 // se collisione, fai beep
			 if(frogCollision){beep(); }
		}
		
    		
    
    
    
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
    	refresh(); // Aggiorna la finestra
	}//end while
  return;  
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
// per aggiornamento della posizione al passo precedente degli oggetti dinamici
void aggiornaOldPos(PipeData *old_pos,PipeData *pipeData){
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
    old_pos->type=pipeData->type;
    return;
}
//--------------------------------------------AGGIORNAMENTO OGGETTI IN MATRICE--------------------------------
void aggiornaPosizioneOggetto(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH], Sprite *sprite) {
    // se le coordinate sono cambiate, pulisci l'area vecchia e stampa il nuovo sprite
    if (pipeData->x != old_pos->x || pipeData->y != old_pos->y) {
        pulisciSpriteInMatrice(old_pos->y, old_pos->x, sprite, screenMatrix, staticScreenMatrix);
        stampaSpriteInMatrice(pipeData->y, pipeData->x, sprite, screenMatrix);
        aggiornaOldPos(old_pos, pipeData);
    }
}
//----------------------------------------------------------------
void stampaSpriteInMatrice(int startRow, int startCol, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH]) {
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;

    for (int i = 0; i < maxRows; i++) {
        for (int j = 0; j < maxCols; j++) {
            int row = startRow + i;
            int col = startCol + j;

            screenMatrix[row][col].ch = sprite->sprite[i][j];
            screenMatrix[row][col].color = sprite->color;
            screenMatrix[row][col].is_changed = true;
        }
    }
}
//----------------------------------------------------------------
void pulisciSpriteInMatrice(int row, int col, Sprite* sprite, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]) {
    int maxRows = sprite->max_row;
    int maxCols = sprite->max_col;
    
    if (row != -1) {
        for (int i = row; i < row + maxRows; i++) {
            for (int j = col; j < col + maxCols; j++) {
                screenMatrix[i][j].ch = staticScreenMatrix[i][j].ch;
                screenMatrix[i][j].color = staticScreenMatrix[i][j].color;
                screenMatrix[i][j].is_changed = true;
            }
        }
    }
}
//--------------------------------------------------------------------
// Ritorna TRUE se oggetto_1 entra nel perimetro di oggetto_2 
bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2)
{
	// stabilisce coordinate massime per entrambi gli oggetti 
	int obj1_maxX, obj1_maxY, obj2_maxX, obj2_maxY; 
	obj1_maxX = (object_1->x) + (sprite_1->max_col);
	obj1_maxY = (object_1->y) +1;
	obj2_maxX = (object_2->x) + (sprite_2->max_col);
	obj2_maxY = (object_2->y) +1;									//assumendo che tutti gli oggetti siano alti 2row
	
	bool checkUP, checkDOWN, checkLEFT,checkRIGHT;
	checkUP = checkDOWN = checkLEFT = checkRIGHT = false;
	// controlla se i lati di object_1 sono dentro object_2, controllo lato per lato
	checkUP = isBetween(object_1->y, object_2->y, obj2_maxY); 
	checkDOWN =  isBetween(obj1_maxY, object_2->y, obj2_maxY);
	checkLEFT = isBetween(object_1->x, object_2->x, obj2_maxX);
	checkRIGHT = isBetween(obj1_maxX, object_2->x, obj2_maxX);
	
	if((checkUP || checkDOWN)&&(checkLEFT || checkRIGHT))
	{
		return true;
	}
	
	return false;
}

bool collisione()
{
	/* chiama checkCollisione (obj1,obj2) 
	   e 			checkCollisione (obj2,obj1) */
	   return false;
}

//------------------------------------------------
//controlla se value è compreso tra valori min e max
bool isBetween (int value, int min_value, int max_value){
	
	if((value >= min_value) && (value <= max_value)){
		return true;
	}
	return false;
}

