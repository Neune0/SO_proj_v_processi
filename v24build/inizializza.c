#include "inizializza.h"
void inizializza(GameData* gameData){
	
	inizializzaTane(gameData->tane);
	
	inizializzaSprite(gameData->sprites);
	
	inizializzaOldPos(gameData->oldPos);
  
	inizializzaSchermo(gameData->gameHud, gameData->schermo); 
	
	gameData->contatori.contP=gameData->contatori.contPN=gameData->contatori.contN=0;

	return;
}
void avviaProcessiBase(int* pipe_fd,Pids* pids,int* pipeRana_fd){
	pids->pidRana = avviaRana(pipe_fd, pipeRana_fd); // avvia il processo che gestisce il movimento della rana
	gestoreMacchine(pipe_fd,pids->pidVeicoli); // avvia le auto e i camion
	
	gestoreTronchi(pipe_fd,pids->pidTronchi); // avvia i tronchi
	return;
}
void inizializzaSchermo(GameHUD *gameHud, Schermo* schermo){
	inizializzaFlagMatrice(schermo->screenMatrix); // inizializza a true i flag di modifica della matrice
	inizializzaHUD(gameHud, schermo->screenMatrix); // inizializza le prime 4 righe
	inizializzaTane(schermo->screenMatrix); // inizializza la parte dello schermo dedicata alle tane
	inizializzaFiume(schermo->screenMatrix); // inizializza la parte dello schermo dedicata al fiume
	inizializzaPrato(schermo->screenMatrix); // inizializza la parte dello schermo dedicata al prato
	inizializzaStrada(schermo->screenMatrix); // inizializza la parte dello schermo dedicata alla strada
	inizializzaMarciapiede(schermo->screenMatrix); // inizializza la parte dello schermo dedicata al marciapiede
	
	copiaMatrice(schermo->screenMatrix, schermo->staticScreenMatrix ); //copia la matrice dinamica in quella statica
	
	return;
}
void inizializzaTane(Tane* tane){
		for(int i =0; i<5; i++){
		tane[i].info.y = 6;
		tane[i].info.x = i*22;
		tane[i].info.id = i;
		tane[i].status = 0;
		}
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
    	old_pos->proiettili[i].x=-1;
    	old_pos->proiettili[i].y=-1;
    	old_pos->proiettili[i].type='P';
    	old_pos->proiettili[i].id=i;
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
			/*
			if (i==1 && (j>startX && j< endX) )
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
			screenMatrix[i][j].color = SFONDO;
			/**/
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

void inizializzaTane(ScreenCell (*screenMatrix)[WIDTH]){
	int freqTane = WIDTH/5 ; // ogni quanti char c'è una tana (larghezza/num_tane)
	int colsTana = 9; // MAGIC NUMBER
	bool tana=false;
	
	for(int i=4;i<9;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			
			if(i>5 )		// dalla riga #6 in poi
			{
				if(j%(freqTane - colsTana)==0){ // se j raggiunge la larghezza della tana
    			tana=!tana;
    		}
    		if(!tana){				
        	screenMatrix[i][j].color = FIUME;
    		}else{										// zona delle tane gialle
    			int r=rand()%1000;
    			if(r%3==0){
    				screenMatrix[i][j].ch = '`';
    			} else if(r%7==0){
    				screenMatrix[i][j].ch = ';';
    			}
    			screenMatrix[i][j].color = TANE;
    		}
			}else{														//prime due righe del blocco tane
				screenMatrix[i][j].color = TANE;
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
			if(n_rand%3==0){
				screenMatrix[i][j].ch = '~';
			}else{
				screenMatrix[i][j].ch = ' ';
			}
			screenMatrix[i][j].color = FIUME;
		}
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaPrato(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=19;i<22;i++){
		for(int j=0;j<WIDTH;j++){
			
			int n_rand=rand()%1000;	// genera numero random
			if(n_rand%7==0){
				screenMatrix[i][j].ch = '"';
			}else if(n_rand%6==0){
				screenMatrix[i][j].ch = '`';
			}else{
				screenMatrix[i][j].ch = ' ';
			}
			screenMatrix[i][j].color = PRATO;
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
		}
	}
	
	// inizializzazione striscia
	for(int j=0;j<WIDTH;j++){
		screenMatrix[26][j].ch = ' ';
		screenMatrix[26][j].color = STRISCIA; 
	}
	return;
}
//------------------------------------------------------------------------------------------------------------------------------
void inizializzaMarciapiede(ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=31;i<33;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			screenMatrix[i][j].color = MARCIAPIEDE; 
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

void inizializzaSprite(Sprite* sprites){

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
	// vettore di sprite degli oggetti di gioco, usato per rilevazione delle collisioni
	
	sprites[RANA_SPRITE] = {RANA_ROWS, RANA_COLS, sprite_rana, RANA};
  sprites[TRONCO_SPRITE] = {TRONCO_ROWS, TRONCO_COLS, sprite_tronco, TRONCHI};
  sprites[AUTO_SPRITE] = {AUTO_ROWS, AUTO_COLS, sprite_auto, AUTO};
  sprites[CAMION_SPRITE] = {CAMION_ROWS, CAMION_COLS, sprite_camion, AUTO};
  sprites[PROIETTILE_SPRITE] = {PROIETTILE_ROWS, PROIETTILE_COLS, sprite_proiettile, PROIETTILE};
  sprites[PROIETTILE_NEMICO_SPRITE] = {PROIETTILE_ROWS,PROIETTILE_COLS,sprite_proiettile_nemico,PROIETTILE};
  sprites[NEMICO_SPRITE] = {2,3,sprite_nemico,RANA};
	sprites[OPEN] = {TANA_ROWS, TANA_COLS, sprite_tana_open, TANE};
	sprites[CLOSED] = {TANA_ROWS, TANA_COLS, sprite_tana_closed, TANE};

	return;
}








