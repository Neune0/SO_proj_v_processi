#include "disegna.h"

void avviaDrawProcess(int pipe_fd[2]) {
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (draw_pid == 0) {
        // Processo "disegna"
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe
        drawProcess(pipe_fd);
        exit(0);
    }
}

void drawProcess(int* pipe_fd) {
		start_color();
    init_pair(RANA, COLOR_GREEN, COLOR_BLACK); // Crea una combinazione di colori (colore verde su sfondo nero)
    init_pair(TRONCHI,COLOR_YELLOW,COLOR_RED); // per la colorazione dei tronchi
    init_pair(SFONDO,COLOR_WHITE,COLOR_BLACK); // colore sfondo
    init_pair(FIUME,COLOR_WHITE,COLOR_BLUE); // colore fiume 
    init_pair(TANE,COLOR_WHITE,COLOR_YELLOW); // colore tane
    init_pair(PRATO,COLOR_WHITE,COLOR_GREEN); // colore prato
    init_pair(STRADA,COLOR_WHITE,COLOR_BLACK); // colore strada
    init_pair(STRISCIA,COLOR_WHITE,COLOR_WHITE); // colore striscia 
    init_pair(MARCIAPIEDE,COLOR_WHITE,COLOR_WHITE); // colore marciapiede
		
		// struttura per leggere la pipe
    struct PipeData pipeData;
    
    // array di strutture che contiene dati oggetti al passo precedente
    // rana, tronchi x3, auto x4, ...
    struct PipeData old_pos [8]; 
    // rana
    old_pos[0].x=-1;
    old_pos[0].y=-1;
    old_pos[0].type=' ';
    old_pos[0].id=0;
    
    // tronco 1
    old_pos[1].x=-1;
    old_pos[1].y=-1;
    old_pos[1].type=' ';
    old_pos[1].id=1;
    
    // tronco 2
    old_pos[2].x=-1;
    old_pos[2].y=-1;
    old_pos[2].type=' ';
    old_pos[2].id=2;
    
    // tronco 3
    old_pos[3].x=-1;
    old_pos[3].y=-1;
    old_pos[3].type=' ';
    old_pos[3].id=3;
    
    // auto
    old_pos[4].x=-1;
    old_pos[4].y=-1;
    old_pos[4].type=' ';
    old_pos[4].id=4;
    
    // auto 2
    old_pos[5].x=-1;
    old_pos[5].y=-1;
    old_pos[5].type=' ';
    old_pos[5].id=5;
    
    // auto 3
    old_pos[6].x=-1;
    old_pos[6].y=-1;
    old_pos[6].type=' ';
    old_pos[6].id=6;
    
    // auto 4
    old_pos[7].x=-1;
    old_pos[7].y=-1;
    old_pos[7].type=' ';
    old_pos[7].id=7;
    
    
    // inizializzazione matrice che rappresenta lo schermo
		struct ScreenCell screenMatrix[HEIGHT][WIDTH];
		inizializzaMatriceSchermo(screenMatrix);
		
		// inizializzazione matrice degli elementi statici che rappresenta lo schermo
		struct ScreenCell staticScreenMatrix[HEIGHT][WIDTH];
		inizializzaMatriceSchermo(staticScreenMatrix);
		
		// prima stampa statica dell'area di gioco
		//stampaMatriceStatica(staticScreenMatrix); 
		
    while (1) {
        // Leggi le coordinate inviate dalla pipe
        read(pipe_fd[0], &pipeData, sizeof(struct PipeData));
        
        // se pipeData.type=='X' ovvero sono coordinate della rana
        if(pipeData.type=='X'){
        	// se pipedata è diverso da pipedata_old allora le coordinate sono cambiate pulisco la matrice e la riscrivo
        	if(pipeData.x!=old_pos[0].x || pipeData.y!=old_pos[0].y){
        		
        		pulisciRanaInMatrice(old_pos[0].y,old_pos[0].x,screenMatrix,staticScreenMatrix);
        		
        		stampaRanaInMatrice(pipeData.y,pipeData.x,screenMatrix);
        		
        		// aggiorno coords_old
        		old_pos[0].x=pipeData.x;
        		old_pos[0].y=pipeData.y;
        	}
        }
        
        // se pipeData.type=='T' ovvero sono coordinate del tronco
        if(pipeData.type=='T'){
        	switch(pipeData.id){
        		case 1:
        			aggiornaPosizioneOggetto(&pipeData, &old_pos[1], screenMatrix,staticScreenMatrix); // aggiorna pozione oggetto in matrice schermo
        			break;
        		case 2:
        			aggiornaPosizioneOggetto(&pipeData, &old_pos[2], screenMatrix,staticScreenMatrix); // aggiorna pozione oggetto in matrice schermo
        			break;
        		case 3:
        			aggiornaPosizioneOggetto(&pipeData, &old_pos[3], screenMatrix,staticScreenMatrix); // aggiorna pozione oggetto in matrice schermo
        			break;
        		default:
        			break;
        	
      		}// chiusura switch
    		}// chiusura if type==T
        
        
        //sono coordinate auto
        if(pipeData.type=='A'){ 
        	switch(pipeData.id){
        		case 4:
        			/*------	aggiorna posizione auto	---------*/
	        	 	aggiornaPosizioneAuto(&pipeData, &old_pos[4], screenMatrix, staticScreenMatrix);
        			break;
      			case 5:
      				aggiornaPosizioneAuto(&pipeData, &old_pos[5], screenMatrix, staticScreenMatrix);
      				break;
    				case 6:
        			/*------	aggiorna posizione auto	---------*/
	        	 	aggiornaPosizioneAuto(&pipeData, &old_pos[6], screenMatrix, staticScreenMatrix);
        			break;
      			case 7:
      				aggiornaPosizioneAuto(&pipeData, &old_pos[7], screenMatrix, staticScreenMatrix);
      				break;
    				default:
    					break;
        	}//end switch
        }//end if type==A
        
        
        //---------continua drawProcess------------
        	//clear(); // Pulisci la finestra di gioco 
        	// stampa a video solo celle della matrice dinamica marcate 
					stampaMatrice(screenMatrix);
        	refresh(); // Aggiorna la finestra
        }
    
    		//endwin(); // Termina ncurses
}

void inizializzaMatriceSchermo(struct ScreenCell (*screenMatrix)[WIDTH]){
	// inizializza valori flag
	for(int row=0; row < HEIGHT; row++){
		for(int cols=0; cols < WIDTH; cols++){
			screenMatrix[row][cols].is_changed = true;
		}
	} 
	
	// inizializzazione prime 4 righe
	
	// inizio funzione
	// variabili di gioco da rendere globali
	int score=rand()%100; 
	int livello= rand()%10;
	
	char score_hud[WIDTH]; // stringa per le info di gioco
	sprintf(score_hud, "Livello: %2d \t SCORE: %3d" , livello, score); //mette nel vettore di char, la stringa formattata
	
	int score_hud_len = strlen(score_hud);
	int start_x_hud = WIDTH/10;						// dove inizia e finisce la Hud
	int end_x_hud = WIDTH/10 + score_hud_len +1;
	int score_hud_index = 0;
	
	for(int i=0;i<4;i++){
		for(int j=0;j<WIDTH;j++){
			if (i==1 && (j>start_x_hud && j< end_x_hud) ){  // posizione della scritta riga#1 
				
				screenMatrix[i][j].ch = score_hud[score_hud_index];  //stampa i caratteri della scritta
				score_hud_index = (score_hud_index+1)%score_hud_len; // aggiorna indice per la stringa
			}else{
				screenMatrix[i][j].ch = ' ';
			}
			screenMatrix[i][j].color = SFONDO;
		}
	}
	// fine funzione
	
	// inizio funzione
	// inizializzazione tane
	int freqTane = WIDTH/5 ; // ogni quanti char c'è una tana (larghezza/num_tane)
	int colsTana = 9;
	bool tana=true;
	
	for(int i=4;i<9;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			
			if(i>5 )		// dalla riga #6 in poi
			{
				if(j%(freqTane -colsTana)==0){ // se j raggiunge la larghezza della tana
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
	// fine funzione
	
	// inizializzazione fiume
	
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
	
	// inizializzazione prato
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
	
	// funzione inizio
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
	// funzione fine
	
	// inizializzazione marciapiede
	for(int i=31;i<33;i++){
		for(int j=0;j<WIDTH;j++){
			screenMatrix[i][j].ch = ' ';
			screenMatrix[i][j].color = MARCIAPIEDE; 
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
}//end InizializzaMatriceSchermo

/*---------RANA-------------*/

void pulisciRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=3;
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  		for(int j=col;j<col+max_col;j++){
  			//cancella la sprite sulla matrice dinamica alle vecchie posizioni, imposta la cella come modificata
    		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
    		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
    		screenMatrix[i][j].is_changed=true;
    	}
  	}
  }      				
  return;
}

// stampa rana nella matrice che rappresenta lo schermo
void stampaRanaInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=3;
	char script[2][3]={{'\\','.','/'},{'/','-','\\'}};
	int i_script=0;
	int j_script=0;
	// ciclo che disegna la rana in matrice
	for(int i=row;i<row+max_row;i++){
		for(int j=col;j<col+max_col;j++){
			screenMatrix[i][j].ch=script[i_script][j_script];
			screenMatrix[i][j].color=RANA;
			screenMatrix[i][j].is_changed=true;
			j_script++;
		}
		i_script++;
		j_script=0;
	}
	return;
}

/*--------------TRONCHI-------------------*/

void aggiornaPosizioneOggetto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
  if(pipeData->x!=old_pos->x || pipeData->y!=old_pos->y){
        				
  	pulisciTroncoInMatrice(old_pos->y,old_pos->x,screenMatrix,staticScreenMatrix);
        		
    stampaTroncoInMatrice(pipeData->y,pipeData->x,screenMatrix);
        	
    // aggiorno pozizione al passo precedente
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
	}
	return;
}


// stampa un tronco nella matrice che rappresenta lo schermo
void stampaTroncoInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=9;
  int sprite_y=0;
  int sprite_x=0;
  
  SpriteTronco t = {{{"~ ~ ~ ~ ~"},
										 {" ~ ~ ~ ~ "}}};	// 2 righe x 9 char
  				
  for(int i=row;i<row+max_row;i++){
  	for(int j=col;j<col+max_col;j++){
  	// prende char dalla sprite e copiarli nella matrice schermo alla posizione relativa corretta
  		
  		screenMatrix[i][j].ch = t.sprite[sprite_y][sprite_x]; //copia char sprite nella matrice
    	screenMatrix[i][j].color=TRONCHI;
    	screenMatrix[i][j].is_changed=true;
  		sprite_x= (sprite_x+1)%max_col; // verifica che l'indice per la sprite non ecceda la dimensione della sprite
    }
    sprite_y++;
  }
  return;
}//end stampaTroncoInMatrice


void pulisciTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=9;
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  		for(int j=col;j<col+max_col;j++){
  		//cancella la sprite sulla matrice dinamica alle vecchie posizioni, imposta la cella come modificata
    		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
    		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
    		screenMatrix[i][j].is_changed=true;
    	}
  	}
  }      				
  return;
}// end pulisciTroncoin Matrice


/*-------------AUTO-----------------*/

void aggiornaPosizioneAuto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	// se pipedata è diverso da auto_data_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
  if(pipeData->x!=old_pos->x || pipeData->y!=old_pos->y){
        				
  	pulisciAutoInMatrice(old_pos->y,old_pos->x,screenMatrix,staticScreenMatrix);
  	
  	stampaAutoInMatrice(pipeData->y,pipeData->x,screenMatrix);
  	   	
    // aggiorno pozizione al passo precedente
    old_pos->x=pipeData->x;
    old_pos->y=pipeData->y;
	}
	return;
}//end aggiornaPosizAuto

//	Pulisce la sprite alle vecchie posizioni e segna le celle come modificate
void pulisciAutoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	//dimensioni sprite auto
	int max_row=2;
	int max_col=4; //4char
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  		for(int j=col;j<col+max_col;j++){
    		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
    		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
    		screenMatrix[i][j].is_changed=true;
    	}
  	}
  }      				
  return;
}

// Scrive sprite su nuove posizioni e segna celle come modificate
void stampaAutoInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]){
	//dimensioni auto
	int max_row=2;
	int max_col=4;
  int sprite_y=0;
  int sprite_x=0;
  
  //SpriteAuto a = {{"xxxx","oooo"}};	// 2 righe x 4 char


	SpriteAuto a = {{{'x','x','x','x'}, {'o','o','o','o'}}};	// 2 righe x 4 char
/**/
  
  for(int i=row;i<row+max_row;i++){
  	for(int j=col;j<col+max_col;j++){
  	// prende char dalla sprite e lo copia nella matrice dinamica alla posizione relativa corretta
  		
  		screenMatrix[i][j].ch = a.sprite[sprite_y][sprite_x]; //copia char sprite nella matrice
    	screenMatrix[i][j].color=STRADA;
    	screenMatrix[i][j].is_changed=true;
    	
  		sprite_x= (sprite_x+1)%max_col; // verifica che l'indice per la sprite non ecceda la dimensione della sprite
    }
    sprite_y++;
  }
  //fflush(stdin);
  //fflush(stdout);
  return;
}//end stampaAutoInMatrice


/*-----------Stampa Generale-----------------*/

void stampaMatrice(struct ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
		// stampa a schermo il carattere della matrice dinamica solo se il flag del carattere è TRUE
		// il flag sara TRUE solo se il carattere è stato modificato
			if(screenMatrix[i][j].is_changed){							
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				mvaddch(i, j, screenMatrix[i][j].ch);
				attron(COLOR_PAIR(screenMatrix[i][j].color));
				screenMatrix[i][j].is_changed = false; // una volta stampato, il flag viene resettato per la prossima modifica
			}
		}
	}
	return;
}// end stampaMatrice

// Stampa a schermo l'intera matrice statica come prima stampa 
void stampaMatriceStatica(struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
		// stampa a schermo il carattere della matrice statica 
			attron(COLOR_PAIR(staticScreenMatrix[i][j].color));
			mvaddch(i, j, staticScreenMatrix[i][j].ch);
			attron(COLOR_PAIR(staticScreenMatrix[i][j].color));
			
		}
	}
	return;
}


