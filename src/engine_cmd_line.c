#include "engine_cmd_line.h"
#include "game.h"
#include "level.h"
#include "nob.h"
#include "rmanager.h"
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
static Font cmd_font;
static Nob_String_Builder command = {0};

static void draw_text(const char *text, int posX, int posY, int fontSize, Color color) {
    DrawTextEx(cmd_font, text, (Vector2) {posX, posY}, fontSize, 1, color);
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

void editor_cmd_init() {
    cmd_font = LoadFontEx(
            "./assets/fonts/JetBrainsMono-Regular.ttf",
            32, 0, 250);
    SetTextureFilter(cmd_font.texture, TEXTURE_FILTER_TRILINEAR);
}

void editor_cmd_save_history() {
    FILE *f = fopen("./engine/cmd.history", "a");
    for (size_t i = CmdHistoryLen - 1; i != 0; --i) {
        fprintf(f, "%s\n", cmd_history[i]);
    }
    TraceLog(LOG_DEBUG, "Commands saved to command history");
    fclose(f);
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
    } else if (strcmp(buf, "entity") == 0) {
        next_string(buf, cmdline, &cur);
        if (strcmp(buf, "add") == 0) {
            next_string(buf, cmdline, &cur);
            int modelid = atoi(buf);

            Model model = LoadModel(ModelPaths[modelid]);
            next_string(buf, cmdline, &cur);
            float x = atof(buf);
            next_string(buf, cmdline, &cur);
            float y = atof(buf);
            next_string(buf, cmdline, &cur);
            float z = atof(buf);
            GameEntity entity = (GameEntity) {
                .id = game->entities.count,
                .position = (Vector3) {x, y, z},
                .model = model,
                .modelid = modelid,
            };
            nob_da_append(&game->entities, entity);
        } else if (strcmp(buf, "remove") == 0) {
            next_string(buf, cmdline, &cur);
            int id = atoi(buf);
            nob_da_remove_unordered(&game->entities, id);
        }
    } else if (strcmp(buf, "save") == 0) {
        level_save(game);
        nob_sb_appendf(&notstdio, "Level saved successfully\n");
    } else if (strcmp(buf, "clear") == 0) {
        notstdio.items[0] = 0;
        notstdio.count = 0;
    } else {
        nob_sb_appendf(&notstdio, "Unknown command: %s\n", buf);
    }
}

void editor_cmd_handle_controls(Game *game) {
    if (IsKeyPressed(KEY_BACKSPACE)) {
        if (command.count > 0) {
            command.items[command.count-1] = 0;
        }
        if (command.count > 0) command.count--;
    } else if (IsKeyPressed(KEY_ENTER)) {
        editor_cmd_run(game, command.items);
        command.items[0] = 0;
        command.count = 0;
    } else {
        char ch = GetCharPressed();
        if (ch != 0) {
            nob_sb_appendf(&command, "%c", ch);
        }
    }
}

void editor_cmd_draw() {
    int screen_w = GetScreenWidth();
    int screen_h = GetScreenHeight();
    int line_h = 30;
    DrawRectangle(0, 10, screen_w, screen_h / 2, GetColor(0x18181899));
    draw_text(notstdio.items, 10, 15, 20, RAYWHITE);
    DrawRectangle(0, screen_h - (line_h + 20), screen_w, line_h, GetColor(0x181818aa));
    draw_text(command.items, 10, screen_h - (line_h + 15), 20, RAYWHITE);
}

void editor_cmd_distroy() {
    for (size_t i = 0; i < CmdHistoryLen; i++) {
        free(cmd_history[i]);
    }
    histrory_cur = 0;
    nob_sb_free(notstdio);
    UnloadFont(cmd_font);
}
