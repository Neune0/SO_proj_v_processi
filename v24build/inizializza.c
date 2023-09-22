#include "inizializza.h"
void inizializza(GameData* gameData){
	
	inizializzaOggettiTane(gameData->tane);
	
	inizializzaSprites(gameData->sprites);
	
	inizializzaOldPos(&(gameData->oldPos));
  
	inizializzaSchermo(gameData); 
	
	gameData->contatori.contP=gameData->contatori.contPN=gameData->contatori.contN=0;

	return;
}
void inizializzaSchermo(GameData* gameData){
	inizializzaFlagMatrice(gameData->schermo.screenMatrix); // inizializza a true i flag di modifica della matrice
	inizializzaHUD(&(gameData->gameHud), gameData->schermo.screenMatrix); // inizializza le prime 4 righe
	inizializzaTane(gameData); // inizializza la parte dello schermo dedicata alle tane
	inizializzaFiume(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata al fiume
	inizializzaPrato(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata al prato
	inizializzaStrada(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata alla strada
	inizializzaMarciapiede(gameData->schermo.screenMatrix); // inizializza la parte dello schermo dedicata al marciapiede
	
	copiaMatrice(gameData->schermo.screenMatrix, gameData->schermo.staticScreenMatrix ); //copia la matrice dinamica in quella statica
	
	return;
}
void avviaProcessiBase(int* pipe_fd,Pids* pids,int* pipeRana_fd){
	pids->pidRana = avviaRana(pipe_fd, pipeRana_fd); // avvia il processo che gestisce il movimento della rana
	gestoreMacchine(pipe_fd,pids->pidVeicoli); // avvia le auto e i camion
	
	gestoreTronchi(pipe_fd,pids->pidTronchi); // avvia i tronchi
	return;
}
void inizializzaOldPos(OldPos* old_pos){
   for(int i=0;i<OLDPOSDIM;i++){
    	old_pos->general[i].x=-1;
    	old_pos->general[i].y=-1; 
    	old_pos->general[i].type=' ';
    	old_pos->general[i].id=i;
   }
   for(int i=0;i<MAXNPROIETTILI;i++){
    	old_pos->proiettili[i].x=-1;
    	old_pos->proiettili[i].y=-1;
    	old_pos->proiettili[i].type='P';
    	old_pos->proiettili[i].id=i;
   }
   for(int i=0;i<MAXNPROIETTILINEMICI;i++){
    	old_pos->proiettiliNemici[i].x=-1;
    	old_pos->proiettiliNemici[i].y=-1;
    	old_pos->proiettiliNemici[i].type='P';
    	old_pos->proiettiliNemici[i].id=i;
   }
   return;
}
//-----------------------------------------------------------------------
void inizializzaGameInfo(GameInfo *gameInfo)
{
	gameInfo->tempo=60;
	gameInfo->vite=4;
	gameInfo->punteggio=0; 
	gameInfo->livello= 1;
}
//------------------------------------------------------
void inizializzaGameHUD(GameHUD *gameHud)
{
	
	inizializzaGameInfo(&gameHud->gameInfo);
	
	//mette nel vettore di char, la stringa formattata
	sprintf(gameHud->score_hud, "Livello: %2d \t SCORE: %3d" , gameHud->gameInfo.livello, gameHud->gameInfo.punteggio); 
	
	int score_hud_len = strlen(gameHud->score_hud);
	gameHud->start_x_hud = WIDTH/10;						// dove inizia e finisce la Hud
	gameHud->end_x_hud = WIDTH/10 + score_hud_len +1;
	gameHud->score_hud_y = 1;
	
	sprintf(gameHud->life_hud, "tempo: %3d \t vite: %2d", gameHud->gameInfo.tempo, gameHud->gameInfo.vite);
	int life_hud_len = strlen(gameHud->life_hud);
	gameHud->life_hud_y = 2;
	gameHud->start_x_life_hud =  WIDTH/10;
	gameHud->end_x_life_hud =  WIDTH/10 + life_hud_len +1;
	
	gameHud->time_hud_y = 2;
	
	
	//int score_hud_index = 0;
}


//----------------------------------------------------------
void inizializzaHUD(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH]){
	
	inizializzaGameHUD(gameHud);
	int startX	=		gameHud->start_x_hud;
	int endX 		=		gameHud->end_x_hud;
	int startY	=		gameHud->score_hud_y;
	int endXLifeHud = gameHud->end_x_life_hud;
	
	
	int score_hud_len = strlen(gameHud->score_hud);
	int life_hud_len = strlen(gameHud->life_hud);
	int score_hud_index = 0;
	int life_hud_index = 0;
	
	for(int i=0;i<4;i++){
		for(int j=0;j<WIDTH;j++){
			//if (i==1 && (j>start_x_hud && j< end_x_hud) ){  // posizione della scritta riga#1 
			screenMatrix[i][j].color = SFONDO;
			switch(i)
			{
				case 1:																// riga y=1
					if ((j>startX && j< endX) )
					{
						screenMatrix[i][j].ch = gameHud->score_hud[score_hud_index];  //stampa i caratteri della scritta
						score_hud_index = (score_hud_index+1)%score_hud_len; // aggiorna indice per la stringa
						
					}
					break;
					
				case 2:																// riga y=2
					if((j>startX && j< endXLifeHud))
					{
						screenMatrix[i][j].ch = gameHud->life_hud[life_hud_index];
						life_hud_index = (life_hud_index +1)%life_hud_len;
				 	}
					break;
				default:
					screenMatrix[i][j].ch = ' ';
					break;
			}
			/*if (i==1 && (j>startX && j< endX) )
			{
				screenMatrix[i][j].ch = gameHud->score_hud[score_hud_index];  //stampa i caratteri della scritta
				score_hud_index = (score_hud_index+1)%score_hud_len; // aggiorna indice per la stringa
				
			}else if(i==2 && (j>startX && j< endXLifeHud))
			{
				screenMatrix[i][j].ch = gameHud->life_hud[life_hud_index];
				life_hud_index = (life_hud_index +1)%life_hud_len;
				
			}else{
				screenMatrix[i][j].ch = ' ';
			}
			screenMatrix[i][j].color = SFONDO;*/
		}
	}
	// inizializzazione sezione bottom
	for(int i=33;i<37;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			screenMatrix[i][j].color = SFONDO; 
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaOggettiTane(Tana* tane){
		for(int i =0; i<5; i++){
		tane[i].info.y = 6;
		tane[i].info.x = i*22;
		tane[i].info.id = i;
		tane[i].status = 0;
		}
		return;
}
void inizializzaTane(GameData* gameData){

	// disegno degli oggetti tana
	// scorro array oggetti Tana che ha 5 elementi
	for(int i=0;i<5;i++){
		// per ogni elemento estraggo y, x, id
		int start_col = gameData->tane[i].info.x;
		int start_row = gameData->tane[i].info.y;
		int id = gameData->tane[i].info.id;
		
		int tana_width=9;
		
		// scrivo sulla matrice schermo la singola tana
		for(int row=start_row;row<9;row++){
			for(int col=start_col;col<tana_width+start_col;col++){
				// generazione random dello sprite della tana
				int r=rand()%1000;
				gameData->schermo.screenMatrix[row][col].ch = (r%3==0) ? '`' : ';';
				gameData->schermo.screenMatrix[row][col].color = TANE;
				gameData->schermo.screenMatrix[row][col].tipo = TANA_OPEN_OBJ;
				gameData->schermo.screenMatrix[row][col].id=id;
			}
		}
	}
	
	// disegno della parte superiore
	for(int i=4;i<6;i++){
		for(int j=0;j<WIDTH;j++){
			gameData->schermo.screenMatrix[i][j].ch = ' ';
			gameData->schermo.screenMatrix[i][j].color = TANE;
			gameData->schermo.screenMatrix[i][j].tipo = SFONDO_OBJ;
			gameData->schermo.screenMatrix[i][j].id=0;
		}
	}
	
	// disegno del fiume tra una tana e l'altra
	for(int k=0;k<5;k++){
		// estraggo dagli oggetti tana i bordi destri delle tane
		int start_col = gameData->tane[k].info.x + 9;
		// la larghezza del tratto di fiume tra una tana e l'altra è di 13
		int end_col = start_col + 13;
		
		// disegno dei singoli spazi
		for(int i=6;i<9;i++){
			for(int j=start_col;j<end_col;j++){
				//disegno solo se non sforo la larghezza dello schermo
				if(j<WIDTH){
					gameData->schermo.screenMatrix[i][j].ch = ' ';
					gameData->schermo.screenMatrix[i][j].color = FIUME;
					gameData->schermo.screenMatrix[i][j].tipo = FIUME_OBJ;
					gameData->schermo.screenMatrix[i][j].id=0;
				}
			}
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaFiume(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=9;i<19;i++){
		for(int j=0;j<WIDTH;j++){
			
			int n_rand=rand()%1000;	// genera numero random
			screenMatrix[i][j].ch = (n_rand%3==0) ? '~': ' ';
			screenMatrix[i][j].color = FIUME;
			screenMatrix[i][j].tipo = FIUME_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaPrato(ScreenCell (*screenMatrix)[WIDTH]){
	char carattere=' ';
	for(int i=19;i<22;i++){
		for(int j=0;j<WIDTH;j++){
			// scelta randomica del carattere
			int n_rand=rand()%1000;	// genera numero random
			if(n_rand%7==0){
				carattere = '"';
			}else if(n_rand%6==0){
				carattere = '`';
			}else{
				carattere = ' ';
			}
			
			screenMatrix[i][j].ch = carattere;
			screenMatrix[i][j].color = PRATO;
			screenMatrix[i][j].tipo = PRATO_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaStrada(ScreenCell (*screenMatrix)[WIDTH]){
	// inizializzazione strada
	for(int i=22;i<31;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			screenMatrix[i][j].color = STRADA;
			screenMatrix[i][j].tipo = STRADA_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	
	// inizializzazione striscia
	for(int j=0;j<WIDTH;j++){
		screenMatrix[26][j].ch = ' ';
		screenMatrix[26][j].color = STRISCIA; 
		screenMatrix[26][j].tipo = STRADA_OBJ;
			screenMatrix[26][j].id = 0;
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=31;i<33;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			screenMatrix[i][j].color = MARCIAPIEDE; 
			screenMatrix[i][j].tipo = MARCIAPIEDE_OBJ;
			screenMatrix[i][j].id = 0;
		}
	}
	return;
}
//--------------------------------------------------------------------------------------------------------------------------------
void inizializzaFlagMatrice(ScreenCell (*screenMatrix)[WIDTH]){
	for(int row=0; row < HEIGHT; row++){
		for(int cols=0; cols < WIDTH; cols++){
			screenMatrix[row][cols].is_changed = true;
		}
	} 
	return;
}

//----------------------------------------------------------------------------
void copiaMatrice(ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]){
	for (int row=0; row<HEIGHT; row++){
		for (int col=0; col<WIDTH; col++){
			staticScreenMatrix[row][col] = screenMatrix[row][col];
		}
	}
	return;
}

void inizializzaSprites(Sprite* sprites){
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
    
	sprites[RANA_SPRITE] = inizializzaSprite(RANA_ROWS, RANA_COLS, sprite_rana, RANA);
  sprites[TRONCO_SPRITE] = inizializzaSprite(TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI);
  sprites[AUTO_SPRITE] = inizializzaSprite(AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO);
  sprites[CAMION_SPRITE] = inizializzaSprite(CAMION_ROWS, CAMION_COLS, sprite_camion, AUTO);
  sprites[PROIETTILE_SPRITE] = inizializzaSprite(PROIETTILE_ROWS, PROIETTILE_COLS, sprite_proiettile, PROIETTILE);
  sprites[PROIETTILE_NEMICO_SPRITE] = inizializzaSprite(PROIETTILE_ROWS,PROIETTILE_COLS,sprite_proiettile_nemico,PROIETTILE);
  sprites[NEMICO_SPRITE] = inizializzaSprite(2,3,sprite_nemico,RANA);
	sprites[OPEN] = inizializzaSprite(TANA_ROWS, TANA_COLS, sprite_tana_open, TANE);
	sprites[CLOSE] = inizializzaSprite(TANA_ROWS, TANA_COLS, sprite_tana_closed, TANE);
	
	return;
}

Sprite inizializzaSprite(int rows, int cols, char **sprite, int color) {
    Sprite spriteObj;
    spriteObj.max_row = rows;
    spriteObj.max_col = cols;
    // Alloca memoria per la matrice di sprite e copia i dati
    spriteObj.sprite = (char **)malloc(rows * sizeof(char *));
    if (spriteObj.sprite == NULL) {
        fprintf(stderr, "Errore nell'allocazione della matrice di sprite\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < rows; i++) {
        spriteObj.sprite[i] = (char *)malloc((cols + 1) * sizeof(char));
        if (spriteObj.sprite[i] == NULL) {
            fprintf(stderr, "Errore nell'allocazione della riga %d della matrice di sprite\n", i);
            exit(EXIT_FAILURE);
        }
        strcpy(spriteObj.sprite[i], sprite[i]);
    }
    //spriteObj.sprite = sprite;
    spriteObj.color = color;
    return spriteObj;
}
