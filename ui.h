#ifndef UI_H
#define UI_H


void build_ui(void);
void draw_ui(void);
void handle_click(u16 x, u16 y);

u16 music_row_y(u8 i);
b8 hit_music_row(u16 x, u16 y, u16 row_y);


#endif