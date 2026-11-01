#include "crystal.h"
#include "json.h"
#include "ui.h"


App apps[MAX_APPS];
Color colors[MAX_COLORS];
c8 groups[MAX_GROUPS][16];
u8 app_count, color_count, group_count;

Display *dp;
Window win;
GC gc;


i32 main(i32 argc, c8 *argv[]) {
    printf("\nCrystal Starting...\n");
    c8 config_file[256] = "userconfig.json";

    if (argc > 1) {
        strncpy(config_file, argv[1], sizeof(config_file) - 1);
        config_file[sizeof(config_file) - 1] = '\0';
    } else {
        const c8 *home = getenv("HOME");
        if (home) {
        snprintf(config_file, sizeof config_file, "%s/.config/crystal/userconfig.json", home);
        }
    }
    load_config(config_file);

    dp = XOpenDisplay(NULL);
    if (!dp) {
        fprintf(stderr, "Cannot open X11 display\n");
        return 1;
    }

    i32 screen = DefaultScreen(dp);
    Window root = RootWindow(dp, screen);

    win = XCreateSimpleWindow(
        dp, root,
        0, 0,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        1,
        BlackPixel(dp, screen),
        WhitePixel(dp, screen)
    );

    XSelectInput(dp, win, ExposureMask | ButtonPressMask);
    gc = XCreateGC(dp, win, 0, NULL);

    XFontStruct *font = XLoadQueryFont(dp, "fixed");
    if (font) {
        XSetFont(dp, gc, font->fid);
    }

    Atom wm_delete = XInternAtom(dp, "WM_DELETE_WINDOW", False);
    
    XSetWMProtocols(dp, win, &wm_delete, 1);
    XMapWindow(dp, win);
    XFlush(dp);

    build_ui();
    draw_ui();

    printf("Listening for events...\n\n");
    
    XEvent event;
    b8 running = true;
    while (running) {
        XNextEvent(dp, &event);   // sleeps here until X has an event

        switch (event.type) {
        case Expose:
            if (event.xexpose.count == 0) draw_ui();   // one redraw per burst of Expose events
            break;
        case ButtonPress:
            if (event.xbutton.button == Button1)
                handle_click(event.xbutton.x, event.xbutton.y);
            break;
        case ClientMessage:
            if ((Atom)event.xclient.data.l[0] == wm_delete) running = false;
            break;
        }
    }

    if (font) XFreeFont(dp, font);
    XFreeGC(dp, gc);
    XDestroyWindow(dp, win);
    XCloseDisplay(dp);
    return 0;
}
