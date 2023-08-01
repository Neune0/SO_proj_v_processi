#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <ncurses.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

#define WIDTH 104
#define HEIGHT 37

#define RANA 1
#define TRONCHI 2
#define SFONDO 3
#define FIUME 4
#define TANE 5
#define PRATO 6
#define STRADA 7
#define STRISCIA 8
#define MARCIAPIEDE 9

/**
		Versione del main con un processo "gestore Figli" 
		che si occupa di controllare le coordinate di spawn dei processi auto e tronchi
		e determinarne la direzione.
*/



// Definizione della struttura dati per le coordinate (x, y) e per il tipo
struct PipeData {
    int x;
    int y;
    char type;
    int id;
};

//							 Sprite per i Tronchi
typedef struct {
    char sprite[2][9]; // 2 righe - 9 char  
} SpriteTronco;

typedef struct {
		char sprite[2][5]; // 2righe - 4 char + terminatore
}SpriteAuto;

typedef struct {
		char sprite[2][8]; // 2righe - 7 char + terminatore
}SpriteCamion;



struct ScreenCell{
	char ch;
	int color;
};



void drawProcess(int* pipe_fd);
void moveProcess(int* pipe_fd);
void tronco(int* pipe_fd, int y, int direzione_x, int id);
void printFrog(int x,int y);
void disegnaTronco(int row, int col);
void stampaTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH]);
void pulisciTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaMatrice(struct ScreenCell (*screenMatrix)[WIDTH]);
void aggiornaPosizioneOggetto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH], 
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void inizializzaMatriceSchermo(struct ScreenCell (*screenMatrix)[WIDTH]);
void pulisciRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH]);

void macchina(int* pipe_fd, int y, int direzione_x, int id);
void aggiornaPosizioneAuto(struct PipeData *pipeData,
															struct PipeData *old_pos,
															struct ScreenCell (*screenMatrix)[WIDTH],
															struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void pulisciAutoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]);
void stampaAutoInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]);

int main() {
		srand(time(NULL));
		
		initscr(); // Inizializza ncurses
    curs_set(FALSE); // Nasconde il cursore
    nodelay(stdscr, TRUE); // Abilita l'input non bloccante
    keypad(stdscr, TRUE); // Abilita il keypad mode
		
    // Crea la pipe
    int pipe_fd[2]; // Pipe file descriptors
    if (pipe(pipe_fd) == -1) {
        perror("Pipe creation failed");
        return 1;
    }

    // Crea il processo per il disegno
    pid_t draw_pid = fork();
    if (draw_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (draw_pid == 0) {
        // Processo "disegna"
        close(pipe_fd[1]); // Chiudi l'estremità di scrittura della pipe
        drawProcess(pipe_fd);
        exit(0);
    }

    // Crea il processo per il movimento
    pid_t move_pid = fork();
    if (move_pid < 0) {
        perror("Fork failed");
        return 1;
    } else if (move_pid == 0) {
        // Processo "muovi"
        close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
        moveProcess(pipe_fd);
        exit(0);
    }
    
    pid_t gestore_figli_pid = fork();
    if(gestore_figli_pid < 0){
    	perror("Fork failed");
      return 1;
    }else if(gestore_figli_pid == 0){  // gestore tronchi
    
    		int dir_tronco[3];
    		int tmp_r = rand()%2; // imposta direzione random del primo tronco
    		if(tmp_r == 1){
    			dir_tronco[0]= 1;
    		}else{
    			dir_tronco[0]= -1;
    		}
    		dir_tronco[1]= -1* dir_tronco[0]; // imposta la direzione del tronco opposta al tronco precedente
    		dir_tronco[2]= -1* dir_tronco[1];
    		
    		
    	// Crea il processo per il tronco 1
				pid_t tronco_pid = fork();
				if (tronco_pid < 0) {
				    perror("Fork failed");
				    return 1;
				} else if (tronco_pid == 0) {
				    // Processo tronco 1
				    //int col_tronco = (int) rand()%(WIDTH-10)
				    close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
				    tronco(pipe_fd,10, dir_tronco[0] ,1);
				    exit(0);
				}
				
				// Crea il processo per il tronco 2
				pid_t tronco_pid2 = fork();
				if (tronco_pid2 < 0) {
				    perror("Fork failed");
				    return 1;
				} else if (tronco_pid2 == 0) {
				    // Processo tronco 2
				    close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
				    tronco(pipe_fd,13, dir_tronco[1],2);
				    exit(0);
				}
				
				// Crea il processo per il tronco 3
				pid_t tronco_pid3 = fork();
				if (tronco_pid3 < 0) {
				    perror("Fork failed");
				    return 1;
				} else if (tronco_pid3 == 0) {
				    // Processo tronco 3
				    close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
				    tronco(pipe_fd,16, dir_tronco[2],3);
				    exit(0);
				}
				//gestore tronchi
				//aspetta termine dei figli-tronchi
				wait(NULL);
				wait(NULL);
				wait(NULL);
				exit(0);
    
    } //fine gestore tronchi, continua il padre...
    
    pid_t gestore_auto_pid = fork();
    /*----------------Gestore figli macchine/caminon ---------*/
    if (gestore_auto_pid < 0) {
		    perror("Fork failed");
		    return 1;
		} else if (gestore_auto_pid == 0) { 
				// gestore macchine
				
				pid_t macchina_pid1 = fork();
				if (macchina_pid1 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid1 == 0) {
					// figlio-macchina 1
					int row_y = 24;
					int direzione_x = 1;
					int id = 4;
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					exit(0);
					
					 
				}// continua gestore macchine...
				
				pid_t macchina_pid2 = fork();
				if (macchina_pid2 < 0) {
		    	perror("Fork failed");
		    	return 1;
				} else if (macchina_pid2 == 0) {
					// figlio-macchina 1
					int row_y = 27;
					int direzione_x = -1;
					int id = 5;
					close(pipe_fd[0]); // Chiudi l'estremità di lettura della pipe
					macchina(pipe_fd, row_y, direzione_x, id);
					exit(0);
				
				}// continua gestore macchine...
				
				wait(NULL); //aspetta fine di macchina1
				wait(NULL); //aspetta fine di macchina2
				exit(0);
				
				
		}//fine gestore macchine, continua il padre...
				
    // Chiudi le estremità della pipe nel processo padre
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Aspetta che i processi figlio terminino
    //moveProcess, drawProcess, gestore-tronchi, gestore-macchine
    wait(NULL);
    wait(NULL);
    wait(NULL);
    wait(NULL);
    return 0;
}

void moveProcess(int* pipe_fd) {
		struct PipeData pipeData;
		pipeData.x=6;
		pipeData.y=6;
		pipeData.type='X';
		
    while (1) {
        // Leggi il carattere dall'input
        int ch = getch();
        if (ch != ERR) {
            
            // Muovi il personaggio in base all'input dell'utente
            switch (ch) {
                case KEY_UP:
                	if(pipeData.y>0){pipeData.y--;}
                  break;
                case KEY_DOWN:
                	if(pipeData.y<HEIGHT-6){pipeData.y++;}
                  break;
                case KEY_LEFT:
                	if(pipeData.x>0){pipeData.x--;}  
                  break;
                case KEY_RIGHT:
                	if(pipeData.x<WIDTH-3){pipeData.x++;}
                  break;
            }

            // Invia le coordinate attraverso la pipe
            write(pipe_fd[1], &pipeData, sizeof(struct PipeData));
        }

        // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
        usleep(100000);
    }
    return;
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
    struct PipeData old_pos [4];
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
    
    
    // inizializzazione matrice che rappresenta lo schermo
		struct ScreenCell screenMatrix[HEIGHT][WIDTH];
		inizializzaMatriceSchermo(screenMatrix);
		
		// inizializzazione matrice degli elementi statici che rappresenta lo schermo
		struct ScreenCell staticScreenMatrix[HEIGHT][WIDTH];
		inizializzaMatriceSchermo(staticScreenMatrix);

    while (1) {
        // Leggi le coordinate inviate dalla pipe
        read(pipe_fd[0], &pipeData, sizeof(struct PipeData));
        
        // se pipeData.type=='X' ovvero sono coordinate della rana
        if(pipeData.type=='X'){
        	// se pipedata è diverso da pipedata_old allora le coordinate sono cambiate pulisco la matrice e la riscrivo
        	if(pipeData.x!=old_pos[0].x || pipeData.y!=old_pos[0].y){
        		
        		pulisciRanaInMatrice(old_pos[0].y,old_pos[0].x,screenMatrix,staticScreenMatrix);
        		/*
        		// sovrascrivo posizione vecchia
        		screenMatrix[old_pos[0].y][old_pos[0].x].ch=' ';
        		*/
        		
        		stampaRanaInMatrice(pipeData.y,pipeData.x,screenMatrix);
        		/*
        		// scrivo posizione nuova
        		screenMatrix[pipeData.y][pipeData.x].ch=pipeData.type;
        		*/
        		
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
    				default:
    					break;
        	}//end switch
        }//end if type==A
        
        
        //---------continua drawProcess------------
        	clear(); // Pulisci la finestra di gioco 
					stampaMatrice(screenMatrix); // stampa a video l'intera matrice
        	refresh(); // Aggiorna la finestra
        }
    
    		endwin(); // Termina ncurses
}

// ok
void disegnaTronco(int row, int col){
    SpriteTronco t = {{{"~ ~ ~ ~ "},
                                         {" ~ ~ ~ ~"}}};    // 2 righe x 8 char
    attron(COLOR_PAIR(TRONCHI));

    // ogni tronco occupa due righe, stampa riga per riga
    for(int tmp_row=0; tmp_row<2; tmp_row++){
        mvprintw(row+tmp_row, col, t.sprite[tmp_row]);
    }
    
    attroff(COLOR_PAIR(TRONCHI));
		return;
}

// ok
void tronco(int* pipe_fd, int y, int direzione_x, int id) {
   	
   	// determina random la coordinata_X  di spawn del tronco 
   	int i=0;
   	int spawn_rand;
   	do{
   		 spawn_rand = rand() % 100;
   		 i++;
   	}while(i!=id);
   	
		struct PipeData pipeData;
		pipeData.x=spawn_rand;
		pipeData.y=y;
		pipeData.type='T';
		pipeData.id=id;
				
		int lunghezza_tronco= 9;
		
		int direzione = direzione_x;
		
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_tronco + 1 < WIDTH){
      		pipeData.x++;
      	}
      	else{
      		direzione*=-1;
      	}
    	}else{
    		if(pipeData.x - 1 > 0){
    			pipeData.x--;
    		}
    		else{
    			direzione*=-1;
    		}
    	}
    	//pipeData.x+=direzione;
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(struct PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}//end tronco


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
  	// prendere char dalla sprite e copiarli nella matrice schermo alla posizione relativa corretta
  		
  		screenMatrix[i][j].ch = t.sprite[sprite_y][sprite_x]; //copia char sprite nella matrice
    	screenMatrix[i][j].color=TRONCHI;
  		sprite_x= (sprite_x+1)%max_col; // verifica che l'indice per la sprite non ecceda la dimensione della sprite
  		/*-----------------
    	screenMatrix[i][j].ch='T';
    	screenMatrix[i][j].color=TRONCHI;
    	/**/
    }
    sprite_y++;
  }
  return;
}//end stampaTroncoInMatrice

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
			j_script++;
		}
		i_script++;
		j_script=0;
	}
	return;
}

// ok
void pulisciTroncoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=9;
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  		for(int j=col;j<col+max_col;j++){
    		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
    		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
    	}
  	}
  }      				
  return;
}

void pulisciRanaInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	int max_row=2;
	int max_col=3;
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  		for(int j=col;j<col+max_col;j++){
    		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
    		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
    	}
  	}
  }      				
  return;
}

// ok
void stampaMatrice(struct ScreenCell (*screenMatrix)[WIDTH]){
	for(int i=0;i<HEIGHT;i++){
		for(int j=0;j<WIDTH;j++){
			attron(COLOR_PAIR(screenMatrix[i][j].color));
			mvaddch(i, j, screenMatrix[i][j].ch);
			attron(COLOR_PAIR(screenMatrix[i][j].color));
		}
	}
	return;
}

// ok
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

// ok
void inizializzaMatriceSchermo(struct ScreenCell (*screenMatrix)[WIDTH]){
	// inizializzazione prime 4 righe
	
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


void macchina(int* pipe_fd, int y, int direzione_x, int id){
	   	// determina random la coordinata_X  di spawn della macchina 
   	int i=0;
   	int spawn_rand;
   	do{
   		 spawn_rand = rand() % 100;
   		 i++;
   	}while(i!=id);
   	
		struct PipeData pipeData;
		pipeData.x=spawn_rand;
		pipeData.y=y;
		pipeData.type='A';
		pipeData.id=id;
				
		int lunghezza_auto= 4;
		
		int direzione = direzione_x;
		
    while (1) {
    	
    	if(direzione==1){
    		if(pipeData.x + lunghezza_auto + 1 < WIDTH){
      		pipeData.x++;
      	}
      	else{	// se l'auto esce dallo schermo a destra, ricompare a sinistra 
      		//direzione*=-1;
      		pipeData.x= lunghezza_auto;
      	}
    	}else{
    		if(pipeData.x - 1 > 0){
    			pipeData.x--;
    		}
    		else{
    			//direzione*=-1;
    			pipeData.x= WIDTH - lunghezza_auto;
    		}
    	}
    	//pipeData.x+=direzione;
      // Invia le coordinate attraverso la pipe
      write(pipe_fd[1], &pipeData, sizeof(struct PipeData));

      // Aspetta un po' prima di generare nuove coordinate forse andrebbe diminuito
      usleep(100000);
    }
}//end macchina

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

void pulisciAutoInMatrice(int row, int col, struct ScreenCell (*screenMatrix)[WIDTH],struct ScreenCell (*staticScreenMatrix)[WIDTH]){
	//dimensioni auto
	int max_row=2;
	int max_col=4; //4char
  if(row!=-1){
  	for(int i=row;i<row+max_row;i++){
  		for(int j=col;j<col+max_col;j++){
    		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
    		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
    	}
  	}
  }      				
  return;
}


void stampaAutoInMatrice(int row,int col, struct ScreenCell (*screenMatrix)[WIDTH]){
	//dimensioni auto
	int max_row=2;
	int max_col=4;
  int sprite_y=0;
  int sprite_x=0;
  
  SpriteAuto a = {{"xxxx",
									 "oooo"}};	// 2 righe x 4 char
  
  for(int i=row;i<row+max_row;i++){
  	for(int j=col;j<col+max_col;j++){
  	// prendere char dalla sprite e copiarli nella matrice schermo alla posizione relativa corretta
  		
  		screenMatrix[i][j].ch = a.sprite[sprite_y][sprite_x]; //copia char sprite nella matrice
    	screenMatrix[i][j].color=STRADA;
  		sprite_x= (sprite_x+1)%max_col; // verifica che l'indice per la sprite non ecceda la dimensione della sprite
  		
    }
    sprite_y++;
  }
  return;
}//end stampaAutoInMatrice


