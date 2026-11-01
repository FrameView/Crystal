#include "crystal.h"


static AppBlock blocks[MAX_APPS];
static u8 block_count;
static ModeSwitch sw = {0xDDDDDD, 0x000000, 10, 10, false, "Switch mode"};


void build_ui(void) {
    block_count = 0;
    
    for (u8 g = 0; g < group_count; g++) {
        const c8 *group_name = groups[g];
        u16 x = 100;
        u16 y = APP_START_Y + (g * GROUP_SPACING);
        
        for (u8 a = 0; a < app_count; a++) {
            if (strcmp(apps[a].group, group_name) == 0) {
                blocks[block_count].x = x;
                blocks[block_count].y = y;
                blocks[block_count].app_idx = a;
                block_count++;
                x += 120;
            }
        }
    }
}


void draw_ui(void) {
    if(sw.dark) {
        XSetForeground(dp, gc, 0x0D1117);
    } else {
        XSetForeground(dp, gc, 0xCCCCCC);
    }
    XFillRectangle(dp, win, gc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    for (u8 g = 0; g < group_count; g++) {
        if(sw.dark) {
            XSetForeground(dp, gc, 0xFFFFFF);
        } else {
            XSetForeground(dp, gc, 0x000000);
        }

        u16 title_y = GROUP_TITLE_Y + (g * GROUP_SPACING);
        XDrawString(dp, win, gc, 50, title_y, groups[g], strlen(groups[g]));

        for (u8 i = 0; i < block_count; i++) {
            AppBlock *blk = &blocks[i];
            App *app = &apps[blk->app_idx];
                
            if (strcmp(app->group, groups[g]) != 0) {
                continue;
            }
                
            XSetForeground(dp, gc, colors[app->rect_color_idx].rgb);
            XFillRectangle(dp, win, gc, blk->x, blk->y, BUTTON_WIDTH, BUTTON_HEIGHT);
            XSetForeground(dp, gc, colors[app->text_color_idx].rgb);
            XDrawString(dp, win, gc, blk->x + 5, blk->y + 20, app->text, strlen(app->text));
        }
    }

    XSetForeground(dp, gc, sw.rect_color);
    XFillRectangle(dp, win, gc, sw.x, sw.y, BUTTON_WIDTH, BUTTON_HEIGHT);
    XSetForeground(dp, gc, sw.text_color);
    XDrawString(dp, win, gc, sw.x + 5, sw.y + 20, sw.text, strlen(sw.text));

    XFlush(dp);
}


void handle_click(u16 x, u16 y) {
    if (x >= sw.x && x < sw.x + BUTTON_WIDTH &&
        y >= sw.y && y < sw.y + BUTTON_HEIGHT) {
                    
            sw.dark = !sw.dark;
            draw_ui();
            return;
        }

    for (u8 i = 0; i < block_count; i++) {
        AppBlock *blk = &blocks[i];

        if (x >= blk->x && x < blk->x + BUTTON_WIDTH &&
            y >= blk->y && y < blk->y + BUTTON_HEIGHT) {
            
            App *app = &apps[blk->app_idx];
            printf("Executing: %s\n", app->cmd);

            (void)system(app->cmd);
            return;

        } 
    }


}