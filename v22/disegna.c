#include "disegna.h"
// per far partire il processo che si occupa di disegnare
void avviaDrawProcess(int pipe_fd[2]) {
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (draw_pid == 0) {
        // Processo "disegna"
        //close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe, TODO: è davvero utile?
        drawProcess(pipe_fd);
        exit(0); // TODO: è davvero utile?
    }
}

// processo che si occupa di disegnare lo schermo
void drawProcess(int* pipe_fd) {
		
	PipeData pipeData; // struttura per leggere la pipe
	
	PipeData old_pos [OLDPOSDIM]; // array di strutture che contiene dati oggetti al passo precedente; rana, tronchi x3, auto x4, ...
	inizializzaOldPos(old_pos,OLDPOSDIM);
	
	PipeData old_pos_proiettili[MAXNPROIETTILI];
	inizializzaOldPosProiettili(old_pos_proiettili,MAXNPROIETTILI);
	int contatore_proiettili_in_gioco=0;
	pid_t array_pid_proiettili[MAXNPROIETTILI];
	
	// definizione sprite utili
	char *sprite_rana[RANA_ROWS] = {"\\./","/-\\"};
	char *sprite_tronco[TRONCO_ROWS] = {"~ ~ ~ ~ ~", " ~ ~ ~ ~ "};
	char *sprite_auto[AUTO_ROWS] = {"xxxx", "oooo"};
	char *sprite_camion[CAMION_ROWS] = {"xxxxxxx", "ooooooo"};
	char *sprite_proiettile[PROIETTILE_ROWS]={"^","|"};
	
	Sprite ranaSprite = {RANA_ROWS, RANA_COLS, sprite_rana, RANA};
  Sprite troncoSprite = {TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI};
  Sprite autoSprite = {AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO};
  Sprite camionSprite = {CAMION_ROWS, CAMION_COLS, sprite_camion, AUTO};
  Sprite proiettileSprite = {PROIETTILE_ROWS, PROIETTILE_COLS, sprite_proiettile, PROIETTILE};
  
	
  ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
	inizializzaMatriceSchermo(screenMatrix, staticScreenMatrix); // inizializza entrambe le matrici
	
	
  while (1) {
  	read(pipe_fd[0], &pipeData, sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
    // test zona
    mvprintw(36,2,"                                                                                                                           ");
    mvprintw(36,2,"pid proiettili: ");
    for(int i=0;i<MAXNPROIETTILI;i++){
    	mvprintw(36,18+(i*11),"pid%d: %d",i,array_pid_proiettili[i]);
    }
    mvprintw(37,110,"proiettili in gioco: %d",contatore_proiettili_in_gioco);
    // fine test zona
    switch(pipeData.type){
    	case 'X':
    		aggiornaPosizioneOggetto(&pipeData, &old_pos[0], screenMatrix, staticScreenMatrix, &ranaSprite);
    		mvprintw(1,110,"                                    ");
    		mvprintw(1,110,"RANA tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
			case 'T':
        aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix,staticScreenMatrix, &troncoSprite);
        mvprintw(2+pipeData.id,110,"                                    ");
    		mvprintw(2+pipeData.id,110,"TRONCO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
      case 'A':
      	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &autoSprite);
      	mvprintw(2+pipeData.id,110,"                                    ");
    		mvprintw(2+pipeData.id,110,"AUTO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
        
     case 'C':  // legge il camion da pipe e aggiorna posizione
      	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &camionSprite);
      	mvprintw(2+pipeData.id,110,"                                    ");
    		mvprintw(2+pipeData.id,110,"CAMION tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
      case 'c':
      	// NON disegnare il camion per un certo tempo, quando esce dallo schermo
     		break;
      case 'S':
      	//proiettile sparato da utente
      	// avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
      	if(contatore_proiettili_in_gioco<MAXNPROIETTILI){ // se si hanno ancora munizioni
      		// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
      		array_pid_proiettili[id_proiettile_disponibile(array_pid_proiettili)]=avviaProiettile(pipe_fd, &pipeData,id_proiettile_disponibile(array_pid_proiettili));
      		contatore_proiettili_in_gioco++;
      		
      	}
      	// fa partire il processo proiettile se il numero di proiettili in gioco è minore di 10
      	break;
      case 'P':
      	// nuove coordinate proiettile
      	// se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	mvprintw(15+pipeData.id,110,"                                    ");
    		mvprintw(15+pipeData.id,110,"PROIETTILE tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
      	if(pipeData.y<0){
      		mvprintw(25+pipeData.id,110,"                                    ");
    			mvprintw(25+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);
      		kill(array_pid_proiettili[pipeData.id], SIGKILL);
      		array_pid_proiettili[pipeData.id]=0;
      		contatore_proiettili_in_gioco--;
      		pulisciSpriteInMatrice(old_pos_proiettili[pipeData.id].y, old_pos_proiettili[pipeData.id].x, &proiettileSprite, screenMatrix, staticScreenMatrix);
      	}
      	else{
      		aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileSprite);
      	}
      	break;
      default:
        break;
    }
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
    refresh(); // Aggiorna la finestra
	}
  return;  
}
int id_proiettile_disponibile(pid_t *array_pid_proiettili){
	for(int i=0;i<MAXNPROIETTILI;i++){
		if(array_pid_proiettili[i]==0)return i;
	}
	return -1;
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
