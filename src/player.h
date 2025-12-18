#ifndef PLAYER_HEADER
#define PLAYER_HEADER
#include "game.h"
#include "raylib.h"

void player_init(Game *game);
void player_update_camera(Game *game);
void player_distroy(Player *player);
void player_handle_controls(Game *game);
void player_draw(Game *game);

#endif
