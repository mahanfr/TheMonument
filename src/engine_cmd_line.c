#include "game.h"
#include "nob.h"
#include <raylib.h>

void cmd_draw(Game *game, Nob_String_Builder *command) {
    (void) game;
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();
    int line_h = 30;
    DrawRectangle(0, screen_h - (line_h + 20), screen_w, line_h, GetColor(0x181818aa));
    DrawText(command->items, 10, screen_h - (line_h + 15), 20, RAYWHITE);
}
