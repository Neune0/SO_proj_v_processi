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
	inizializzaColorazione(); // setting colori
		
	PipeData pipeData; // struttura per leggere la pipe
	PipeData old_pos [OLDPOSDIM]; // array di strutture che contiene dati oggetti al passo precedente; rana, tronchi x3, auto x4, ...
	inizializzaOldPos(old_pos,OLDPOSDIM);
	
  // TODO forse è meglio fare una funzione sola che inizializzi entrambe le matrici
  ScreenCell screenMatrix[HEIGHT][WIDTH]; // matrice che rappresenta lo schermo
	inizializzaMatriceSchermo(screenMatrix); // inizializzazione matrice che rappresenta lo schermo
	ScreenCell staticScreenMatrix[HEIGHT][WIDTH]; // matrice degli elementi statici dello schermo
	inizializzaMatriceSchermo(staticScreenMatrix); // inizializzazione matrice degli elementi statici che rappresenta lo schermo
		
  while (1) {
  	read(pipe_fd[0], &pipeData, sizeof(PipeData)); // Leggi le coordinate inviate dalla pipe
    
    pid_t pidProiettileRana;
    
    
    switch(pipeData.type){
    	case 'X':
    		aggiornaPosizioneRana(&pipeData, &old_pos[0], screenMatrix,staticScreenMatrix); // aggionrna la posizione della rana nella matrice
        break;
			case 'T':
        aggiornaPosizioneTronco(&pipeData, &old_pos[pipeData.id], screenMatrix,staticScreenMatrix); // aggiorna pozione oggetto in matrice schermo
        break;
      case 'A':
      	aggiornaPosizioneAuto(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix);
        break;
      case 'S':
      	//proiettile sparato da utente
      	if(pipeData.y <=0){ 
      		kill(pidProiettileRana,9); 
    		}else{
      		aggiornaPosizioneProiettile(&pipeData, &old_pos[pipeData.id], screenMatrix, staticScreenMatrix);
      	} 
      	break;
      case 'P':
      	// nuove coordinate proiettile
      	break;
      	
    	case 'x': // la rana vuole sparare
    		// controllo sul numero di proiettili
    		// crea il processo per il proiettile e ritorna il pid del processoProiettile
    		pidProiettileRana = creaProiettile(pipe_fd, &pipeData);
    		
    		break;
      default:
        break;
    }
		stampaMatrice(screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
    refresh(); // Aggiorna la finestra
	}
  return;  
}


/*---------RANA-------------------------------------------------------------------------------*/
void aggiornaPosizioneRana(PipeData *pipeData,PipeData *old_pos,ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]){
	// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
  if(pipeData->x!=old_pos->x || pipeData->y!=old_pos->y){
  	pulisciRanaInMatrice(old_pos->y,old_pos->x,screenMatrix,staticScreenMatrix);	
    stampaRanaInMatrice(pipeData->y,pipeData->x,screenMatrix);
    aggiornaOldPos(old_pos,pipeData);
	}
	return;
}

void pulisciRanaInMatrice(int row, int col,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]){
	int max_row=2, max_col=3;
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
void stampaRanaInMatrice(int row,int col,ScreenCell (*screenMatrix)[WIDTH]){
	int max_row=2, max_col=3; //MAGIC NUMBER
	char script[2][3]={{'\\','.','/'},{'/','-','\\'}};
	int i_script=0, j_script=0;
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

/*--------------TRONCHI-------------------------------------------------------------*/

void aggiornaPosizioneTronco(PipeData *pipeData,PipeData *old_pos,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]){
	// se pipedata è diverso da troncodata_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
  if(pipeData->x!=old_pos->x || pipeData->y!=old_pos->y){				
  	pulisciTroncoInMatrice(old_pos->y,old_pos->x,screenMatrix,staticScreenMatrix);  		
    stampaTroncoInMatrice(pipeData->y,pipeData->x,screenMatrix);	
    aggiornaOldPos(old_pos,pipeData);
	}
	return;
}


// stampa un tronco nella matrice che rappresenta lo schermo
void stampaTroncoInMatrice(int row,int col, ScreenCell (*screenMatrix)[WIDTH]){
	int max_row=2,max_col=9;//MAGIC NUMBER
  int sprite_y=0,sprite_x=0;
  
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


void pulisciTroncoInMatrice(int row, int col,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]){
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
}

/*-----------------AUTO-----------------------------*/
void aggiornaPosizioneAuto(PipeData *pipeData,PipeData *old_pos,ScreenCell (*screenMatrix)[WIDTH],ScreenCell (*staticScreenMatrix)[WIDTH]){
	// se pipedata è diverso da auto_data_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
  if(pipeData->x!=old_pos->x || pipeData->y!=old_pos->y){  				
  	pulisciAutoInMatrice(old_pos->y,old_pos->x,screenMatrix,staticScreenMatrix);
  	stampaAutoInMatrice(pipeData->y,pipeData->x,screenMatrix);
    aggiornaOldPos(old_pos,pipeData);
	}
	return;
}

//	Pulisce la sprite alle vecchie posizioni e segna le celle come modificate
void pulisciAutoInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]){
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
void stampaAutoInMatrice(int row,int col, ScreenCell (*screenMatrix)[WIDTH]){
	//dimensioni auto
	int max_row=2,max_col=4; // MAGIC NUMBER
  int sprite_y=0,sprite_x=0;
  
	SpriteAuto a = {{{'x','x','x','x'}, {'o','o','o','o'}}};	// 2 righe x 4 char
  
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
  return;
}

/*---------------------------PROIETTILI---------------------------------*/
void aggiornaPosizioneProiettile(PipeData *pipeData, PipeData *old_pos, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]){
	// se pipedata è diverso da proiettile_data_old allora le coordinate sono cambiate pulisco la matrice e ristampo sulla matrice
  if(pipeData->x!=old_pos->x || pipeData->y!=old_pos->y){  				
  	pulisciProiettileInMatrice(old_pos->y,old_pos->x,screenMatrix,staticScreenMatrix);
  	stampaProiettileInMatrice(pipeData->y,pipeData->x,screenMatrix);
    aggiornaOldPos(old_pos,pipeData);
	}
}// end aggiornaPoiettile

void pulisciProiettileInMatrice(int row, int col, ScreenCell (*screenMatrix)[WIDTH], ScreenCell (*staticScreenMatrix)[WIDTH]){
  if(row!=-1){
  	int i=row;
  	int j=col;
		screenMatrix[i][j].ch=staticScreenMatrix[i][j].ch;
		screenMatrix[i][j].color=staticScreenMatrix[i][j].color;
		screenMatrix[i][j].is_changed=true;
  }      				
  return;
}//end pulisciProiettileInMatrice


// Scrive proiettile su nuove posizioni e segna celle come modificate
void stampaProiettileInMatrice(int row,int col, ScreenCell (*screenMatrix)[WIDTH]){
	int i=row;
	int j=col;
	// prende char e lo copia nella matrice dinamica alla posizione relativa corretta
	screenMatrix[i][j].ch = '*'; //copia char sprite nella matrice
	screenMatrix[i][j].color=RANA;
	screenMatrix[i][j].is_changed=true;
  return;
}//end stampaProiettileInMatrice












/*-----------Stampa Generale-----------------*/
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
