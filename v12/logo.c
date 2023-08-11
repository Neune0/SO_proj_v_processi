#include "logo.h"
void logo(){
	nodelay(stdscr, FALSE); // riabilita l'input non bloccante
	clear(); // pulisce lo schermo
	refresh(); // refresh schermo
	
    
  
  // ... Stampa il logo del videogioco ...
    
  // stampa la scritta premi un tasto per continuare
  mvprintw(5,5,"Premi un tasto per continuare...");
	
	refresh(); // refresh schermo
	
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
