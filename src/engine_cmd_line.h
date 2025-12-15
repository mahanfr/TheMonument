#ifndef CMD_DRAW_HEADER
#define CMD_DRAW_HEADER

#include "game.h"
#include "nob.h"

void editor_cmd_init(void);
void editor_cmd_draw(Game *game, Nob_String_Builder *command);
void editor_cmd_run(Game *game, char* cmdline);
void editor_cmd_distroy(void);

#endif
