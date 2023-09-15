#include "hud.h"

void aggiornaGameInfo(GameHUD *gameHud, int tempo, int vite, int punteggio, int livello) //usata in disegna
{
	gameHud->gameInfo.tempo = tempo;
	gameHud->gameInfo.vite = vite;
	gameHud->gameInfo.punteggio = punteggio;
	gameHud->gameInfo.livello = livello;
}


// aggiorna le stringhe con le info di gioco
void aggiornaGameHud(GameHUD *gameHud)
{
		// aggiorna la visualizzazione grafica delle vite, per ogni vita rimasta stampa un *
		int vite_restanti = gameHud->gameInfo.vite;
		char string_lifes[4] = "    ";
		for(int i=0; i<vite_restanti; i++)
		{
			string_lifes[i]='*';
		}
		//aggiorna le stringhe con le info
		sprintf(gameHud->score_hud, "Livello: %2d \t SCORE: %3d" , gameHud->gameInfo.livello, gameHud->gameInfo.punteggio);
		sprintf(gameHud->life_hud, "tempo: %3d \t vite: %s \t :%d", gameHud->gameInfo.tempo, string_lifes, vite_restanti);
		
		//calcola lunghezza delle nuove stringhe
		int score_hud_len = strlen(gameHud->score_hud);
		int life_hud_len = strlen(gameHud->life_hud);
		int startX	=		gameHud->start_x_hud;
		
		// aggiorna pos dell'ultimo carattere delle stringhe
		gameHud->end_x_hud = startX + score_hud_len +1;
		gameHud->end_x_life_hud = startX + life_hud_len +1; 
}

//----------------------------------------------
void aggiornaHudInMatrice(GameHUD *gameHud, ScreenCell (*screenMatrix)[WIDTH])
{
	aggiornaGameHud(gameHud);
	
	int score_hud_len = strlen(gameHud->score_hud);	// aggiorna dimensione delle stringhe
	int life_hud_len = strlen(gameHud->life_hud);
	
	int startX	=		gameHud->start_x_hud;					// prende coordinate della HUD
	int endX 		=		gameHud->end_x_hud;
	int endXLifeHud = gameHud->end_x_life_hud;
	
	int score_hud_index = 0;	// indici per le stringhe
	int life_hud_index = 0;
	
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<WIDTH;j++)
		{
			screenMatrix[i][j].color = SFONDO;
			switch(i)
			{
				case 1:																// riga y=1
					if ((j>startX && j< endX) )
					{
						screenMatrix[i][j].ch = gameHud->score_hud[score_hud_index];  //stampa i caratteri della scritta
						score_hud_index = (score_hud_index+1)%score_hud_len; // aggiorna indice per la stringa
					}else{
						screenMatrix[i][j].ch = ' ';
					}
					break;
					
				case 2:																// riga y=2
					if((j>startX && j< endXLifeHud))
					{
						screenMatrix[i][j].ch = gameHud->life_hud[life_hud_index];
						life_hud_index = (life_hud_index +1)%life_hud_len;
				 	}else{
						screenMatrix[i][j].ch = ' ';
					}
					break;
				default:
					screenMatrix[i][j].ch = ' ';
					break;
			}
			screenMatrix[i][j].is_changed = true;
		}
	}
	return;
}
