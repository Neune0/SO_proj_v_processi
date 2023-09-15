#include "pausa.h"
volatile sig_atomic_t child_running = 1;
void handle_stop(int signum) {
    printf("Child process received SIGSTOP\n");
    child_running = 0;
}

void handle_continue(int signum) {
    printf("Child process received SIGCONT\n");
    child_running = 1;
}

int pausa(pid_t child_pid) {
   
    
    // Blocca il processo figlio
		kill(child_pid, SIGSTOP);
		nodelay(stdscr, TRUE); // riabilita l'input non bloccante
		clear();
		mvprintw(10,20,"Premi un tasto per continuare");
		refresh();
		char scelta=getch();
		while(scelta!='f'){
			scelta=getch();
		}
		nodelay(stdscr, FALSE); // riabilita l'input non bloccante
		// Sblocca il processo figlio
		kill(child_pid, SIGCONT);
    clear();
    refresh();
    return 0;
}

