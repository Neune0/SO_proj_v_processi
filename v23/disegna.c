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
	
	PipeData old_pos_proiettili_nemici[MAXNPROIETTILINEMICI];
	inizializzaOldPosProiettili(old_pos_proiettili_nemici,MAXNPROIETTILI);
	int contatore_proiettili_nemici_in_gioco=0;
	pid_t array_pid_proiettili_nemici[MAXNPROIETTILINEMICI];
	
	PipeData old_pos_nemici[3];
	//inizializzaOldPosNemici(old_pos_nemici,3);
	int contatore_nemici_in_gioco=0;
	pid_t array_pid_nemici[3];
	
	// strutture di tane
	Tana tane[5];
	for(int i =0; i<5; i++){
		tane[i].info.y = 6;
		tane[i].info.x = i*22;
		tane[i].info.id = i;
		tane[i].status = 0;
	}
	
	// definizione sprite utili
	char *sprite_rana[RANA_ROWS] = {"\\./","/-\\"};
	char *sprite_tronco[TRONCO_ROWS] = {"~ ~ ~ ~ ~", " ~ ~ ~ ~ "};
	char *sprite_auto[AUTO_ROWS] = {"xxxx", "oooo"};
	char *sprite_camion[CAMION_ROWS] = {"xxxxxxx", "ooooooo"};
	char *sprite_proiettile[PROIETTILE_ROWS]={"^","|"};
	char *sprite_proiettile_nemico[PROIETTILE_ROWS]={"|","v"};
	char *sprite_nemico[2]={" ^ ","/ \\"};
	
	char *sprite_tana_open[TANA_ROWS] = {"           ", "           ", "           "};
	char *sprite_tana_closed[TANA_ROWS] = {"H         H", "H         H", "HHHHHHHHHHH"};
	
	
	Sprite ranaSprite = {RANA_ROWS, RANA_COLS, sprite_rana, RANA};
  Sprite troncoSprite = {TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI};
  Sprite autoSprite = {AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO};
  Sprite camionSprite = {CAMION_ROWS, CAMION_COLS, sprite_camion, AUTO};
  Sprite proiettileSprite = {PROIETTILE_ROWS, PROIETTILE_COLS, sprite_proiettile, PROIETTILE};
  Sprite proiettileNemicoSprite = {PROIETTILE_ROWS,PROIETTILE_COLS,sprite_proiettile_nemico,PROIETTILE};
  Sprite nemicoSprite = {2,3,sprite_nemico,RANA};
						//--------
	Sprite tanaApertaSprite = {TANA_ROWS, TANA_COLS, sprite_tana_open, TANE};
	Sprite tanaChiusaSprite = {TANA_ROWS, TANA_COLS, sprite_tana_closed, TANE};
	Sprite taneSprite [2];
	taneSprite[OPEN] = tanaApertaSprite;
	taneSprite[CLOSED] = tanaChiusaSprite;
	
	
	// vettore di sprite degli oggetti di gioco, usato per rilevazione delle collisioni
	Sprite spriteOggetto[7];
  spriteOggetto[RANA_SPRITE] = ranaSprite;
  spriteOggetto[TRONCO_SPRITE] = troncoSprite;
  spriteOggetto[AUTO_SPRITE] = autoSprite;
  spriteOggetto[CAMION_SPRITE] = camionSprite;
	spriteOggetto[PROIETTILE_SPRITE] = proiettileSprite;
  spriteOggetto[PROIETTILE_NEMICO_SPRITE] = proiettileNemicoSprite;
  spriteOggetto[NEMICO_SPRITE] = nemicoSprite;
	
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
    mvprintw(35,2,"                                                                                                                           ");
    mvprintw(35,2,"pid proiettili nemici: ");
    for(int i=0;i<MAXNPROIETTILINEMICI;i++){
    	mvprintw(35,26+(i*11),"pid%d: %d",i,array_pid_proiettili_nemici[i]);
    }
    mvprintw(34,2,"                                                                                                                           ");
    mvprintw(34,2,"pid nemici: ");
    for(int i=0;i<MAXNNEMICI;i++){
    	mvprintw(34,15+(i*11),"pid%d: %d",i,array_pid_nemici[i]);
    }
    mvprintw(37,110,"                        ");
    mvprintw(37,110,"proiettili in gioco: %d",contatore_proiettili_in_gioco);
    mvprintw(38,110,"                               ");
    mvprintw(38,110,"nemici in gioco: %d",contatore_nemici_in_gioco);
    mvprintw(39,110,"                                  ");
    mvprintw(39,110,"proiettili nemici in gioco: %d",contatore_proiettili_nemici_in_gioco);
    // fine test zona
    switch(pipeData.type){
    	case 'X':
    		aggiornaPosizioneOggetto(&pipeData, &old_pos[0], screenMatrix, staticScreenMatrix, &ranaSprite);
    		mvprintw(0,110,"                                    ");
    		mvprintw(0,110,"RANA tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
			case 'T':
        aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix,staticScreenMatrix, &troncoSprite);
        mvprintw(pipeData.id,110,"                                    ");
    		mvprintw(pipeData.id,110,"TRONCO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
      case 'A':
      	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &autoSprite);
      	mvprintw(pipeData.id,110,"                                    ");
    		mvprintw(pipeData.id,110,"AUTO tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
        
     case 'C':  // legge il camion da pipe e aggiorna posizione
      	aggiornaPosizioneOggetto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix, &camionSprite);
      	mvprintw(pipeData.id,110,"                                    ");
    		mvprintw(pipeData.id,110,"CAMION tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
        break;
      case 'c':
      	// NON disegnare il camion per un certo tempo, quando esce dallo schermo
     		break;
      case 'S':
      	//proiettile sparato da utente
      	// avvia il proiettile con posizione iniziale della rana (o dell oggetto che ha sparato)
      	if(contatore_proiettili_in_gioco<MAXNPROIETTILI){ // se si hanno ancora munizioni
      		// incremento contatore e faccio partire il processo proiettile, salvo il pid del processo
      		array_pid_proiettili[id_disponibile(array_pid_proiettili,MAXNPROIETTILI)]=avviaProiettile(pipe_fd, &pipeData,id_disponibile(array_pid_proiettili,MAXNPROIETTILI));
      		contatore_proiettili_in_gioco++;
      		
      	}
      	// fa partire il processo proiettile se il numero di proiettili in gioco è minore di 10
      	break;
      case 'n':
      	if(contatore_nemici_in_gioco<MAXNNEMICI){ // se non si è raggiunto il numero massimo di nemici
      		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
      		array_pid_nemici[id_disponibile(array_pid_nemici,MAXNNEMICI)]=avviaNemico(pipe_fd,&pipeData,id_disponibile(array_pid_nemici,MAXNNEMICI));
      		contatore_nemici_in_gioco++;
      		
      	}
      	break;
      case 's':
      	// proiettile nemico sparato
      	if(contatore_proiettili_nemici_in_gioco<MAXNPROIETTILINEMICI){ // se non si è raggiunto il numero massimo di nemici
      		// incremento contatore e faccio partire il processo nemico, salvo il pid del processo
      		array_pid_proiettili_nemici[id_disponibile(array_pid_proiettili_nemici,MAXNPROIETTILINEMICI)]=avviaProiettileNemico(pipe_fd, &pipeData,id_disponibile(array_pid_proiettili_nemici,MAXNPROIETTILINEMICI));
      		contatore_proiettili_nemici_in_gioco++;
      	}
      	break;
      case 'P':
      	// nuove coordinate proiettile
      	// se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	mvprintw(13+pipeData.id,110,"                                       ");
    		mvprintw(13+pipeData.id,110,"PROIETTILE tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
      	if(pipeData.y<0){
      		mvprintw(13+pipeData.id,110,"                                    ");
    			mvprintw(13+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);
      		kill(array_pid_proiettili[pipeData.id], SIGKILL);
      		waitpid(array_pid_proiettili[pipeData.id],NULL,0);
      		array_pid_proiettili[pipeData.id]=0;
      		contatore_proiettili_in_gioco--;
      		pulisciSpriteInMatrice(old_pos_proiettili[pipeData.id].y, old_pos_proiettili[pipeData.id].x, &proiettileSprite, screenMatrix, staticScreenMatrix);
      	}
      	else{
      		aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileSprite);
      	}
      	break;
      case 'p':
      	// nuove coordinate proiettile nemico
      	// se il proiettile ha sforato devo uccidere il processo e decrementare il contatore
      	mvprintw(25+pipeData.id,110,"                                       ");
    		mvprintw(25+pipeData.id,110,"PROIETTILEN tipo: %c, x:%d ,y:%d ,id: %d",pipeData.type,pipeData.x,pipeData.y,pipeData.id);
      	if(pipeData.y>30){
      		mvprintw(25+pipeData.id,110,"                                    ");
    			mvprintw(25+pipeData.id,110,"id: %d uccisione proiettile con pid: %d",pipeData.id,array_pid_proiettili[pipeData.id]);
      		kill(array_pid_proiettili_nemici[pipeData.id], SIGKILL);
      		waitpid(array_pid_proiettili_nemici[pipeData.id],NULL,0);
      		array_pid_proiettili_nemici[pipeData.id]=0;
      		contatore_proiettili_nemici_in_gioco--;
      		pulisciSpriteInMatrice(old_pos_proiettili_nemici[pipeData.id].y, old_pos_proiettili_nemici[pipeData.id].x, &proiettileNemicoSprite, screenMatrix, staticScreenMatrix);
      		old_pos_proiettili_nemici[pipeData.id].type = 'P';
      		
      	}
      	else{
      		aggiornaPosizioneOggetto(&pipeData, &old_pos_proiettili_nemici[pipeData.id], screenMatrix, staticScreenMatrix, &proiettileNemicoSprite);
      	}
      default:
        break;
    }//end switch-case su type
    
    //-------------------------collisioni rana------
		bool frogCollision = false;
		bool autoCollision = false;
		bool enemyBulletCollision = false;
		bool taneAperteCollision = false;
		bool taneChiuseCollision = false;
		bool autoProiettiliNemiciCollision = false;
		
		//TanaStatus t_stat = OPEN;
		
		frogCollision = collisioneRana(old_pos, spriteOggetto);
		autoCollision = collisioneAuto(old_pos, spriteOggetto);
		enemyBulletCollision= collisioneProiettiliNemici(old_pos, old_pos_proiettili_nemici, spriteOggetto);
		autoProiettiliNemiciCollision = collisioneAutoProiettili(old_pos, old_pos_proiettili_nemici, spriteOggetto);
		;
		//if(autoProiettiliNemiciCollision){ beep();}
		
		taneChiuseCollision = collisioneTaneChiuse(old_pos, tane, spriteOggetto, taneSprite);
		taneAperteCollision = collisioneTaneAperte(old_pos, tane, spriteOggetto, taneSprite);
		
		if(taneChiuseCollision) { beep(); }
		
		if(taneAperteCollision){
				beep(); 
		 		for(int i=0; i<5; i++){
			 		if(tane[i].status == CLOSED){
			 			stampaSpriteInMatrice( tane[i].info.y, tane[i].info.x, &taneSprite[CLOSED], staticScreenMatrix);
			 			stampaSpriteInMatrice( tane[i].info.y, tane[i].info.x, &taneSprite[CLOSED], screenMatrix);
			 		}
			 	}
	 	}
	 	
	 	if(autoProiettiliNemiciCollision){ beep();}
	 	
		if((frogCollision && autoCollision) || (enemyBulletCollision) ){ beep(); }	
    
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
    refresh(); // Aggiorna la finestra
	}//end while
  return;  
}//end drawProcess
//--------------------------------------------FINE PROCESSO DISEGNA----------------------------------


//-----------------------------------------------------------
int id_disponibile(pid_t *array_pid, int lunghezza){
	for(int i=0;i<lunghezza;i++){
		if(array_pid[i]==0)return i;
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
/**	Prende due oggetti di gioco con rispettive sprite e verifica se gli oggetti si incontrano 
		Ritorna TRUE se oggetto_1 entra nel perimetro di oggetto_2
*/ 
bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2)
{
	// stabilisce coordinate massime per entrambi gli oggetti 
	int obj1_maxX, obj1_maxY, obj2_maxX, obj2_maxY; 
	obj1_maxX = (object_1->x) + (sprite_1->max_col-1);
	obj1_maxY = (object_1->y) + (sprite_1->max_row-1);
	obj2_maxX = (object_2->x) + (sprite_2->max_col-1);
	obj2_maxY = (object_2->y) + (sprite_2->max_row-1);									//assumendo che tutti gli oggetti siano alti 2row
	
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
//----------------------------------------collisioni auto/camion-----------------------
bool collisioneAuto( PipeData *old_pos, Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	
  bool collision=false;
   
	for(int i=4; i<OLDPOSDIM; i++){ // per ogni auto/camion in gioco (old_pos[4-11])
 		if(old_pos[i].type=='A')
 		{
			collision = checkCollisione(&old_pos[i], rana, &array_sprite[AUTO_SPRITE], &array_sprite[RANA_SPRITE]);
		}else if (old_pos[i].type=='C')
		{
			collision = checkCollisione(&old_pos[i], rana, &array_sprite[CAMION_SPRITE], &array_sprite[RANA_SPRITE]);
 		}
	 	if(collision) break; //se rileva collisione ferma il ciclo e ed esce
	}
 	return collision;
}
//------------------------------------collisioni proiettili nemici-------------------------------

bool collisioneProiettiliNemici( PipeData *old_pos, PipeData *old_pos_proiettiliNemici ,Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	
  bool collision=false;
   
	for(int i=0; i<MAXNPROIETTILINEMICI; i++){ 							// per ogni proiettile nemico di gioco
 		if(old_pos_proiettiliNemici[i].type != 'P'){					// se il proiettile è attivo
 			collision = checkCollisione(&old_pos_proiettiliNemici[i], rana, 
																	&array_sprite[PROIETTILE_NEMICO_SPRITE], &array_sprite[RANA_SPRITE]);
 		}
	 	if(collision) break; //se rileva collisione ferma il ciclo e ed esce
	}
 	return collision;
}
//----------------------------collisione TANE-------------------
bool collisioneTaneAperte( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite)
{
	bool collision=false;
	PipeData *rana = &old_pos[0];
	
	for(int i=0; i<5; i++){
		PipeData *tana = &array_tane[i].info; //punta alla struttura PipeData dentro la struct Tana
		if(array_tane[i].status == OPEN){
			collision = checkCollisione(rana, tana, &array_sprite[RANA_SPRITE], &arr_tana_sprite[OPEN]);
		}
		if(collision) {
			array_tane[i].status = CLOSED;
			break;
		}
	}
 	return collision;
}
//----------------------------------------
bool collisioneTaneChiuse( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite)
{
	bool collision=false;
	PipeData *rana = &old_pos[0];
	
	for(int i=0; i<5; i++){
		PipeData *tana = &array_tane[i].info; //punta alla struttura PipeData dentro la struct Tana
		if(array_tane[i].status == CLOSED){
			collision = checkCollisione(rana, tana, &array_sprite[RANA_SPRITE], &arr_tana_sprite[CLOSED]);
		}
		if(collision) {
			//array_tane[i].status = CLOSED;
			break;
		}
	}
 	return collision;
}
//-----------------------------------------collisione auto-proiettili------------
bool collisioneAutoProiettili( PipeData *old_pos, PipeData * array_proiettili, Sprite *array_sprite)
{
	PipeData *rana = &old_pos[0];
	PipeData *veicolo;
	PipeData *proiettile;
	TipoSprite spriteVeicolo;
	
  bool collision=false;
  
	for(int i=0; i<MAXNPROIETTILI; i++){ 							// per ogni proiettile nemico di gioco
		proiettile = &array_proiettili[i];  					// prendi proiettile attuale
		
		for(int j=4; j<OLDPOSDIM; j++){												// per ogni veicolo  in gioco
			veicolo =  &old_pos[j];															// prendi veicolo corrente
			
			if(proiettile->type == 'p'){					// se il proiettile è attivo
				switch(veicolo->type){							//controlla tipo del veicolo, scegli la sprite corretta
					case 'A':
						spriteVeicolo = AUTO_SPRITE;
						;
						break;
					case 'C':
						spriteVeicolo = CAMION_SPRITE;
						;
						break;
					default:
						spriteVeicolo = CAMION_SPRITE;
						;
						break;
				}
	 			collision = checkCollisione(proiettile, veicolo, 
																		&array_sprite[PROIETTILE_NEMICO_SPRITE], &array_sprite[ spriteVeicolo ]);
	 		}
		 	if(collision) break; //se rileva collisione ferma il ciclo e ed esce
		}
 		
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


