#include "logo.h"
void logo(){
	// parte da 3 e arriva a 101 questi sono i caratteri liberi all'interno della box
	// parte da 3 e arriva a 34 questi sono i caratteri liberi all'interno della box
	nodelay(stdscr, FALSE); // riabilita l'input non bloccante
	clear(); // pulisce lo schermo
	
	stampaBox(); // stampa la box attorno al logo
	
	mvprintw(STARTROWBENVENUTO,STARTCOLBENVENUTO,"Benvenuto...");
	refresh();
  
  stampaTitolo(STARTROWTITOLO,STARTCOLTITOLO); // stampa il titolo
  
  stampaLogo(STARTROWLOGO,STARTCOLLOGO); // stampa il logo
  
  
  // stampa la scritta premi un tasto per continuare
  mvprintw(STARTROWCONTINUA,STARTCOLCONTINUA,"Premi un tasto per continuare...");
  
  
  

  getch(); // Aspetta che l'utente prema un tasto qualsiasi

  nodelay(stdscr, TRUE); // riabilita l'input non bloccante
  
  clear();
  refresh();
  clear();
  refresh();
  
  exit(0);
}

void avviaLogo() {
    pid_t pid_logo;

    pid_logo = fork();

    if (pid_logo < 0) {
        perror("Fork failed");
        exit(1);
    }
    if (pid_logo == 0) {
        logo();
    }

    waitpid(pid_logo, NULL, 0);
}

void stampaBox(){
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

void stampaLogo(int start_row,int start_col){
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
			usleep(10000);
			refresh();
		}
	}																
}

void stampaTitolo(int start_row,int start_col){

	char titoloMatrix[DIMTITOLOROW][DIMTITOLOCOL] = {
        " _____                   _____                ",
        "|  ___| __ ___   __ _   |  ___| __ ___   __ _ ",
        "| |_ | '__/ _ \\ / _` |  | |_ | '__/ _ \\ / _` |",
        "|  _|| | | (_) | (_| |  |  _|| | | (_) | (_| |",
        "|_|  |_|  \\___/ \\__,  | |_|  |_|  \\___/ \\__, |",
        "                |___/                   |___/ "
    };
    
    for(int row=start_row;row<start_row+DIMTITOLOROW;row++){
			for(int col=start_col;col<start_col+DIMTITOLOCOL;col++){
				attron(COLOR_PAIR(RANA));
				mvaddch(row, col, titoloMatrix[row - start_row][col - start_col]);
				attroff(COLOR_PAIR(RANA));
				usleep(10000);
				refresh();
			}
	}		

}
