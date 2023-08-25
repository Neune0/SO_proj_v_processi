#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

volatile sig_atomic_t child_running = 1;

void handle_stop(int signum) {
    printf("Child process received SIGSTOP\n");
    child_running = 0;
}

void handle_continue(int signum) {
    printf("Child process received SIGCONT\n");
    child_running = 1;
}

int main() {
    pid_t child_pid;
    
    // Creazione del processo figlio
    child_pid = fork();

    if (child_pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (child_pid == 0) {
        // Installa il gestore di segnale per SIGSTOP
        signal(SIGSTOP, handle_stop);
        // Installa il gestore di segnale per SIGCONT
        signal(SIGCONT, handle_continue);

        int counter = 0;
        printf("Child process running...\n");
        while (1) {
            if (child_running) {
                counter++;
                printf("Child counter: %d\n", counter);
            }
            usleep(1000000); // Aggiungi una pausa di 1000 ms (1 secondo)
        }
    } else {
        // Codice del processo padre
        printf("Parent process started. Child PID: %d\n", child_pid);
				
					
						// Blocca il processo figlio
				    kill(child_pid, SIGSTOP);
				    printf("Processo figlio bloccato\n");
						printf("________________________________");
				    // Aspetta che il processo figlio sia bloccato
				    usleep(5000000); // Aspetta 1 secondo
					
					
				
						// Sblocca il processo figlio
				    kill(child_pid, SIGCONT);
				    printf("Processo figlio sbloccato\n");
				
		      
		      
				
				
				
        

        // Aspetta che il processo figlio termini
        waitpid(child_pid, NULL, 0);
        printf("Child process terminated.\n");
    }

    return 0;
}

