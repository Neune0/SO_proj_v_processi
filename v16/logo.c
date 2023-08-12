#include "logo.h"
void logo(){
	nodelay(stdscr, FALSE); // riabilita l'input non bloccante
	clear(); // pulisce lo schermo

  // ... Stampa il logo del videogioco ...
  stampaBox(); // stampa la box attorno al logo
  stampaLogo(); // stampa il logo
  // stampaTitolo();
  
  // stampa la scritta premi un tasto per continuare
  mvprintw((((2+35)/2)),40,"Premi un tasto per continuare..."); // MAGIC NUMBER
  

  getch(); // Aspetta che l'utente prema un tasto qualsiasi

  nodelay(stdscr, TRUE); // riabilita l'input non bloccante
  
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

void stampaLogo(){

	int start_row_logo=(((2+35)/2)-7)-5; //MAGIC NUMBER
	int start_col_logo=((2+102)/2)-9; // MAGIC NUMBER
	int rows_logo=7; // MAGIC NUMBER
	int cols_logo=19; // MAGIC NUMBER
	char logoImg[7][19]={
																		{' ',' ',' ',' ',' ',' ',' ','_',' ',' ',' ','_',' ',' ',' ',' ',' ',' ',' '},//ok
																		{' ',' ',' ',' ',' ',' ','(','.',')','_','(','.',')',' ',' ',' ',' ',' ',' '},//ok
																		{' ',' ',' ','_',' ','(',' ',' ',' ','_',' ',' ',' ',')',' ','_',' ',' ',' '},//ok
																		{' ',' ','/',' ','\\','/','\'','-','-','-','-','-','\'','\\','/',' ','\\',' ',' '},//ok
																		{'_','_','\\',' ','(',' ','(',' ',' ',' ',' ',' ',')',' ',')',' ','/','_','_'},//ok
																		{')',' ',' ',' ','/','\\',' ','\\','.','_','.','/',' ','/','\\',' ',' ',' ','('},//ok
																		{' ',')','_','/',' ','/','|','\\',' ',' ',' ','/','|','\\',' ','\\','_','(',' '}//ok
																	};
	for(int row=start_row_logo;row<start_row_logo+rows_logo;row++){
		for(int col=start_col_logo;col<start_col_logo+cols_logo;col++){
			mvaddch(row, col, logoImg[row - start_row_logo][col - start_col_logo]);
		}
	}																
}
