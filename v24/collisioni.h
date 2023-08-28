#ifndef COLLISIONI_H
#define COLLISIONI_H
#include "utilities.h"

//------------------------------------------------------------COLLISIONI----------------------------
bool collisioneRana( PipeData *old_pos, Sprite *array_sprite);
bool collisioneAuto( PipeData *old_pos, Sprite *array_sprite);
int collisioneProiettiliNemici( PipeData *old_pos, PipeData *old_pos_proiettiliNemici ,Sprite *array_sprite);
bool collisioneTaneAperte( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite);
bool collisioneTaneChiuse( PipeData *old_pos, Tana *array_tane, Sprite *array_sprite, Sprite *arr_tana_sprite);
bool collisioneAutoProiettili( PipeData *old_pos, PipeData * array_proiettili, Sprite *array_sprite, TipoSprite sprite_proiettile);


bool checkCollisione(PipeData *object_1, PipeData *object_2, Sprite* sprite_1, Sprite* sprite_2);
bool isBetween (int value, int min_value, int max_value);




#endif
