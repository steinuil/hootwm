#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <xcb/xcb.h>

#define XCB_MOVE XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y
#define XCB_RESIZE XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT
#define XCB_MOVE_RESIZE XCB_MOVE | XCB_RESIZE

xcb_connection_t *conn;
xcb_screen_t     *screen;
xcb_drawable_t    root;

uint16_t screen_w, screen_h;
uint16_t master_size;

uint16_t gap, bord;

bool run;

// Manage nodes
#include "nodes.c"

// Manage windows
#include "windows.c"

// Move windows
void move_resize(xcb_drawable_t win, const uint16_t x, const uint16_t y,
                                     const uint16_t w, const uint16_t h) {
    xcb_configure_window(conn, win, XCB_MOVE_RESIZE, (uint16_t[]){x,y,w,h});
}

void tile() {
    if (!master) {
        return;
    } else if (!head) {
        move_resize(master->win, gap, gap,
                screen_w - gap*2 - bord*2, screen_h - gap*2 - bord*2);
    } else {
        move_resize(master->win, gap, gap,
                master_size - gap*2 - bord*2, screen_h - gap*2 - bord*2);

        node *c;
        uint8_t y, n = 0;
        for (c = head ; c ; c = c->next) ++n;
        for (c = head ; c ; c = c->next) {
            move_resize(c->win, master_size + gap, y + gap,
                    screen_w - master_size - gap*2 - bord*2,
                    screen_h / n - gap*2 - bord*2);
            y += screen_h / n;
        }
    }
}

void update_current() {
    node *c;

    for (c = master ; c ; c = c->next) {
        if (c == current) {
            xcb_change_window_attributes(conn, c->win, XCB_CW_BORDER_PIXEL,
                    win_focus);
            xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT,
                    c->win, XCB_CURRENT_TIME);

        } else {
            xcb_change_window_attributes(conn, c->win, XCB_CW_BORDER_PIXEL,
                    win_unfocus);
        }
    }
}

// Requests
void map_request(xcb_map_request_event *ev) {
    add_window(ev->window);
    xcb_map_window(conn, ev->window);

    tile();
    update_current();
}

void destroy_notify(xcb_destroy_notify_event *ev) {
    destroy_window(ev->window);
    
    tile();
    update_current();
}

// Stuff to make it run
void setup() {
    screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;
    root = screen->root;

    screen_w = screen->width_in_pixels;
    screen_h = screen->height_in_pixels;

    master, head, current = NULL;

    master_size = screen_w / 0.6;
    run = true;
}

void quit() {
    run = false;
    puts("ma rippe,,");
    xcb_disconnect(conn);
    exit(0);
}

void loop() {
    xcb_generic_event_t *ev;

    while (run && ev = xcb_wait_for_event(conn)) {
        switch(ev->response_type & ~0x80) {
        case XCB_MAP_REQUEST: {
            map_request((xcb_map_request_event_t*)ev);
            xcb_flush(conn);
            break; }
        case XCB_DESTROY_NOTIFY: {
            destroy_notify((xcb_destroy_notify_event*)ev);
            xcb_flush(conn);
            break; }
        }

        free(ev);
    }
}

int main(int argc, char* argv[]) {
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) return 1;

    setup();
    signal(SIGINT, quit);
    loop();

    return 0;
}
