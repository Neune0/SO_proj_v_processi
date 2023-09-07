#ifndef COLLISIONI_H
#define COLLISIONI_H
#include "utilities.h"

//------------------------------------------------------------COLLISIONI----------------------------
bool checkRanaTronco( PipeData *old_pos, Sprite *array_sprite);
bool checkRanaVeicolo( PipeData *old_pos, Sprite *array_sprite);
bool checkRanaTanaAperta( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite);
bool checkRanaTanaChiusa( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite);
bool checkAutoProiettile( PipeData *old_pos, PipeData * array_proiettili, Sprite *array_sprite, TipoSprite sprite_proiettile);
bool checkRanaFiume( PipeData *old_pos, Sprite *array_sprite);
bool checkProiettileNemicoProiettile( PipeData *array_proiettili_A, PipeData * array_proiettili_B, 
																			Sprite *array_sprite, TipoSprite sprite_proiettile_A, TipoSprite sprite_proiettile_B);





int collisioneProiettiliNemici( PipeData *old_pos, PipeData *old_pos_proiettiliNemici ,Sprite *array_sprite);
int collisioneRanaTronco( PipeData *old_pos, Sprite *array_sprite );
int collisioneProiettileNemicoProiettile( PipeData *array_proiettili_A, PipeData * array_proiettili_B, 
																			Sprite *array_sprite, TipoSprite sprite_proiettile_A, TipoSprite sprite_proiettile_B);
int collisioneAutoProiettile( PipeData *old_pos, PipeData * array_proiettili, Sprite *array_sprite, TipoSprite sprite_proiettile);



bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2);
bool checkCollisioneOggetto(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2);
bool isBetween (int value, int min_value, int max_value);




#endif
