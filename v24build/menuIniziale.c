#include "menuIniziale.h"

void menuIniziale(){
	clear(); // pulisce lo schermo
	
	int scelta=0;
	int sottolineato=0;
	bool stay=true;
	stampaBoxMenu(); // stampa la box attorno al menu
	stampaLogoMenu(STARTROWLOGOMENU,STARTCOLLOGOMENU);
	boxMenu(STARTROWBLUBOX,STARTCOLBLUBOX,ENDROWBLUBOX,ENDCOLBLUBOX);
	refresh(); // rende visibili le modifiche allo schermo
	
	// ciclo per il menu
	while(stay){
		// stampa menu
		for(int i=0;i<NUMOP;i++){
			stampaOpzione(i,sottolineato);
		}
		refresh();
		
		scelta=getch();
		switch(scelta){
			case KEY_UP:
				if(sottolineato>0){sottolineato--;}
				break;
			case KEY_DOWN:
				if(sottolineato<NUMOP-1){sottolineato++;}
				break;
			case 10:
				stay=false; // per uscire
				break;
			default:
				break;
		}
		
	}
	
	clear();
	refresh();

	exit(scelta);
}

int avviaMenuIniziale() {
    pid_t pid_menu_iniziale;

    pid_menu_iniziale = fork();

    if (pid_menu_iniziale < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid_menu_iniziale == 0) {
        menuIniziale();
    }
		int status;
    waitpid(pid_menu_iniziale, &status, 0);
    return status;
}

void stampaBoxMenu(){
	for (int row = ROWSTARTBOX; row <= ROWENDBOX; row++) {
  	for (int col = COLSTARTBOX; col <= COLENDBOX; col++) {
    	if (row == ROWSTARTBOX || row == ROWENDBOX) {
    		attron(COLOR_PAIR(RANA));
      	mvprintw(row, col, "=");
      	attroff(COLOR_PAIR(RANA));
      }else{
      	if(col==COLSTARTBOX||col==COLENDBOX){
      		attron(COLOR_PAIR(RANA));
      	mvprintw(row, col, "|");
      	attroff(COLOR_PAIR(RANA));
      	}
      	else{
      		attron(COLOR_PAIR(SFONDO));
      		mvprintw(row, col, " ");
      		attroff(COLOR_PAIR(SFONDO));
      	}
      }
      
    }
  }
}

void stampaLogoMenu(int start_row,int start_col){
	char logoImg[DIMLOGOROW][DIMLOGOCOL]={
																		{' ',' ',' ',' ',' ',' ',' ','_',' ',' ',' ','_',' ',' ',' ',' ',' ',' ',' '},
																		{' ',' ',' ',' ',' ',' ','(','.',')','_','(','.',')',' ',' ',' ',' ',' ',' '},
																		{' ',' ',' ','_',' ','(',' ',' ',' ','_',' ',' ',' ',')',' ','_',' ',' ',' '},
																		{' ',' ','/',' ','\\','/','\'','-','-','-','-','-','\'','\\','/',' ','\\',' ',' '},
																		{'_','_','\\',' ','(',' ','(',' ',' ',' ',' ',' ',')',' ',')',' ','/','_','_'},
																		{')',' ',' ',' ','/','\\',' ','\\','.','_','.','/',' ','/','\\',' ',' ',' ','('},
																		{' ',')','_','/',' ','/','|','\\',' ',' ',' ','/','|','\\',' ','\\','_','(',' '}
																	};
	for(int row=start_row;row<start_row+DIMLOGOROW;row++){
		for(int col=start_col;col<start_col+DIMLOGOCOL;col++){
			attron(COLOR_PAIR(RANA));
			mvaddch(row, col, logoImg[row - start_row][col - start_col]);
			attroff(COLOR_PAIR(RANA));
		}
	}																
}

void stampaOpzione(int i,int sottolineato){
	char opzioni[NUMOP][DIMOP]={"|-  Nuova Partita  -|","|-    Continua     -|","|-     Carica      -|","|- Scegli  Livello -|","|-  Impostazioni   -|","|-      Esci       -|"};
	// per la stampa del sottolineato
			if(i==sottolineato){
				attron(COLOR_PAIR(RANA));
				mvprintw((i*4)+STARTROWOP,STARTCOLOP,"---------------------");
				for(int j=0;j<DIMOP;j++){
					mvaddch((i*4)+1+STARTROWOP,STARTCOLOP+j,opzioni[i][j]);
				}
				mvprintw((i*4)+2+STARTROWOP,STARTCOLOP,"---------------------");
				mvaddch((i*4)+1+STARTROWOP,STARTCOLOP-2,'>');
				attroff(COLOR_PAIR(RANA));
			}
			else{
				// stampa opzioni non sottolineate
				attron(COLOR_PAIR(SFONDO));
				mvprintw((i*4)+STARTROWOP,STARTCOLOP,"---------------------");
				for(int j=0;j<DIMOP;j++){
					mvaddch((i*4)+1+STARTROWOP,STARTCOLOP+j,opzioni[i][j]);
				}
				mvprintw((i*4)+2+STARTROWOP,STARTCOLOP,"---------------------");
				mvaddch((i*4)+1+STARTROWOP,STARTCOLOP-2,' ');
				attroff(COLOR_PAIR(SFONDO));
			}
			return;
}

void boxMenu(int start_row,int start_col,int end_row,int end_col){
	for(int row=start_row;row<end_row+1;row++){
		for(int col=start_col;col<end_col+1;col++){
			if(row==start_row||row==end_row){
				attron(COLOR_PAIR(MENUBOX));
				mvaddch(row, col,'.');
				attroff(COLOR_PAIR(MENUBOX));
			}
			if(col==start_col||col==end_col||col==start_col+1||col==end_col-1){
				attron(COLOR_PAIR(MENUBOX));
				mvaddch(row, col,'.');
				attroff(COLOR_PAIR(MENUBOX));
			}
		}
	}

}
