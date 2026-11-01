#ifndef CRYSTAL_H
#define CRYSTAL_H


#include "aliaslnx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <X11/Xlib.h>
#include <sys/stat.h>
#include <time.h>
#include <ctype.h>
#include <stdbool.h>


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define BUTTON_WIDTH 90
#define BUTTON_HEIGHT 30
#define GROUP_SPACING 100
#define GROUP_TITLE_Y 96
#define APP_START_Y 100
#define MAX_GROUPS 8
#define MAX_APPS 64
#define MAX_COLORS 32


typedef struct {
    u32 rgb;
    c8 name[16];
} Color;

typedef struct {
    c8 group[16];
    c8 text[24];
    c8 cmd[128];
    u8 rect_color_idx;
    u8 text_color_idx;
} App;

typedef struct {
    u16 x;
    u16 y;
    u8 app_idx;
} AppBlock;

typedef struct {
    u32 rect_color;
    u32 text_color;
    u16 x;
    u16 y;
    b8 dark;
    c8 text[12];
} ModeSwitch;


extern App apps[MAX_APPS];
extern Color colors[MAX_COLORS];
extern c8 groups[MAX_GROUPS][16];
extern u8 app_count, color_count, group_count;

extern Display *dp;
extern Window win;
extern GC gc;


#endif