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
  
  // vettore di sprite degli oggetti di gioco, usato per rilevazione delle collisioni
  Sprite spriteOggetto[4];
  spriteOggetto[RANA_SPRITE] = ranaSprite;
  spriteOggetto[TRONCO_SPRITE] = troncoSprite;
  spriteOggetto[AUTO_SPRITE] = autoSprite;
  spriteOggetto[CAMION_SPRITE] = camionSprite;
  	
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
		  	// aggiorna la posizione ma NON disegna il camion per un certo tempo, quando esce dallo schermo
		  	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &camionSprite);
		  	pulisciSpriteInMatrice(old_pos[pipeData.id].y, old_pos[pipeData.id].x, &camionSprite, screenMatrix, staticScreenMatrix);
		 		break;
	  	case 'S':
		  	// fa partire il processo proiettile se il numero di proiettili in gioco è minore di 10
		  	break;
	  	case 'P':
		  	// nuove coordinate proiettile
	  		break;
	  	default:
  			break;
		}//end switch-case
    
    //----collisioni rana------
		bool frogCollision = false;
		
		frogCollision = collisioneRana(old_pos, spriteOggetto);
		
		if(frogCollision){beep(); }	
    		
    
    
    
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
//----------------------------------------COLLISIONI----------------------------
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
//-----------------------------------------------------------------------------
// Controlla se la Rana collide con uno degli oggetti in gioco (Rana == old_pos[0])
bool collisioneRana( PipeData *old_pos, Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	
  bool collision=false;
   
	for(int i=1; i<OLDPOSDIM; i++){ // per ogni oggetto di gioco
 		if(i<4) // check collisione con Tronchi
 		{ 
 			collision = checkCollisione(rana, &old_pos[i], &array_sprite[RANA_SPRITE], &array_sprite[TRONCO_SPRITE]);
	 	}
	 	if(i>=4){							// check collisione con auto/camion
	 		if(old_pos[i].type=='A')
	 		{
 				collision = checkCollisione(rana, &old_pos[i], &array_sprite[RANA_SPRITE], &array_sprite[AUTO_SPRITE]);
			}else if (old_pos[i].type=='C')
			{
				collision = checkCollisione(rana, &old_pos[i], &array_sprite[RANA_SPRITE], &array_sprite[CAMION_SPRITE]);
	 		}
	 	}
	 	if(collision) break; //se rileva collisione ferma il ciclo e ed esce
	}
 	return collision;
}

//------------------------------------------------
//controlla se value è compreso tra valori min e max
bool isBetween (int value, int min_value, int max_value){
	
	if((value >= min_value) && (value <= max_value)){
		return true;
	}
	return false;
}

