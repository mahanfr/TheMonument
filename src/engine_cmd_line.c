#include "game.h"
#include "nob.h"
#include <raylib.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>

#define MAX_HISTORY 50

static char *cmd_history[MAX_HISTORY] = {0};
#define CmdHistoryLen (sizeof(cmd_history) / sizeof(cmd_history[0]))
static size_t histrory_cur = 0;
static Nob_String_Builder notstdio;

void editor_cmd_save_history() {
    FILE *f = fopen("./engine/cmd.history", "a");
    for (size_t i = CmdHistoryLen - 1; i > -1; --i) {
        fprintf(f, "%s\n", cmd_history[i]);
    }
    TraceLog(LOG_DEBUG, "Commands saved to command history");
    fclose(f);
}

static void next_string(char *buf, char* str, size_t *cur) {
    while(str[*cur] == ' ' || str[*cur] == '\t') {
        (*cur)++;
    }
    size_t buf_cur = 0;
    while(true) {
        char ch = str[*cur];
        if (ch == ' ' || ch == '\0' || ch == '\n') {
            break;
        }
        buf[buf_cur] = ch;
        buf_cur++;
        (*cur)++;
    }
    buf[buf_cur + 1] = 0;
}

void editor_cmd_run(Game *game, char* cmdline) {
    // cmd_history[histrory_cur] = malloc(strlen(cmdline));
    // strcpy(cmd_history[histrory_cur], cmdline);
    // histrory_cur++;

    char buf[1024] = {0};
    size_t cur = 0;

    next_string(buf, cmdline, &cur);
    if (strcmp(buf, "echo") == 0) {
        next_string(buf, cmdline, &cur);
        nob_sb_appendf(&notstdio, "%s\n", buf);
    } else if (strcmp(buf, "clear") == 0) {
        notstdio.items[0] = 0;
        notstdio.count = 0;
    } else {
        nob_sb_appendf(&notstdio, "Unknown command: %s\n", buf);
    }
}

void editor_cmd_draw(Game *game, Nob_String_Builder *command) {
    (void) game;
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();
    int line_h = 30;
    DrawRectangle(0, 10, screen_w, screen_h / 2, GetColor(0x18181899));
    DrawText(notstdio.items, 10, 15, 20, RAYWHITE);
    DrawRectangle(0, screen_h - (line_h + 20), screen_w, line_h, GetColor(0x181818aa));
    DrawText(command->items, 10, screen_h - (line_h + 15), 20, RAYWHITE);
}
