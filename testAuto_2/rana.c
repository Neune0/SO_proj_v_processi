#include "rana.h"
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
