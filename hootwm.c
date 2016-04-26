#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <xcb/xcb.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define XCB_MOVE XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y
#define XCB_RESIZE XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT
#define XCB_MOVE_RESIZE XCB_MOVE | XCB_RESIZE
#define move_resize(win, x, y, w, h) uint32_t tmpvals[4] = { x, y, w, h }; \
    xcb_configure_window(conn, win, XCB_MOVE_RESIZE, tmpvals);

xcb_connection_t *conn;
xcb_screen_t     *screen;
int16_t           pipe_fd;
char             *pipe_f;

uint16_t screen_w, screen_h;
uint16_t master_size;

uint8_t gap, bord;

uint32_t win_focus, win_unfocus;

bool run;

void p(char *s) {
    printf("hootwm: %s\n", s);
}

// Manage nodes
#include "nodes.c"

// Manage windows
#include "windows.c"

// Move windows
void tile(void) {
    if (!master) {
        return;
    } else if (!head) {
        move_resize(master->win, gap, gap,
                screen_w - gap*2 - bord*2, screen_h - gap*2 - bord*2);
    } else {
        move_resize(master->win, gap, gap,
                master_size - gap*2 - bord*2, screen_h - gap*2 - bord*2);

        node *c;
        uint16_t y = 0;
        uint8_t n = 0;
        for (c = head ; c ; c = c->next) ++n;
        for (c = head ; c ; c = c->next) {
            move_resize(c->win, master_size + gap, y + gap,
                    screen_w - master_size - gap*2 - bord*2,
                    ((c->next) ? (screen_h / n) : screen_h - y) -
                    gap*2 - bord*2);
            y += screen_h / n;
        }
    }
}

void update_current(void) {
    node *c;

    for (c = master ; c ; c = c->next) {
        uint32_t a[1] = { bord };
        xcb_configure_window(conn, c->win, XCB_CONFIG_WINDOW_BORDER_WIDTH, a);

        if (c == current) {
            xcb_change_window_attributes(conn, c->win, XCB_CW_BORDER_PIXEL,
                    &win_focus);
            xcb_set_input_focus(conn, XCB_INPUT_FOCUS_POINTER_ROOT,
                    c->win, XCB_CURRENT_TIME);

        } else {
            xcb_change_window_attributes(conn, c->win, XCB_CW_BORDER_PIXEL,
                    &win_unfocus);
        }
    }
}

// Handle events
void map_request(xcb_map_request_event_t *ev) {
    add_window(ev->window);
    xcb_map_window(conn, ev->window);

    tile();
    update_current();

}

void destroy_notify(xcb_destroy_notify_event_t *ev) {
    destroy_window(ev->window);
    
    tile();
    update_current();
}

// Stuff to make it run
void setup(void) {
    screen = xcb_setup_roots_iterator(xcb_get_setup(conn)).data;

    pipe_f = "/tmp/hoot";
    mkfifo(pipe_f, 0666);
    pipe_fd = open(pipe_f, O_RDONLY | O_NONBLOCK);

    screen_w = screen->width_in_pixels;
    screen_h = screen->height_in_pixels;

    master = NULL;
    head = NULL;
    current = NULL;

    master_size = screen_w * 0.6;
    gap = 1;
    bord = 3;

    win_focus = 52260;
    win_unfocus = 34184;

    run = true;

    uint32_t mask[1] = {
        XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY | // destroy notify
        XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT }; // map request
    xcb_change_window_attributes(conn, screen->root, XCB_CW_EVENT_MASK, mask);

    xcb_flush(conn);
}

void quit() {
    run = false;
    p("Thanks for using!");

    close(pipe_fd); unlink(pipe_f);
    xcb_disconnect(conn);

    exit(0);
}

void event_loop(void) {
    uint32_t length;
    xcb_generic_event_t *ev; 

    do {
        if ((ev = xcb_poll_for_event(conn))) {
            switch(ev->response_type & ~0x80) {
            case XCB_MAP_REQUEST: {
                map_request((xcb_map_request_event_t*)ev);
                xcb_flush(conn);
                } break; 
            case XCB_DESTROY_NOTIFY: {
                destroy_notify((xcb_destroy_notify_event_t*)ev);
                xcb_flush(conn);
                } break; 
            }
        }

        char buffer[] = { '\0', [255] = '\0', };
        length = read(pipe_fd, buffer, sizeof(buffer));
        if (length) printf("hootwm: %s", buffer); 

        free(ev);

    } while (run);
}

int main(void) {
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) return 1;

    setup();
    signal(SIGINT, quit);
    p("Welcome to hootwm.");
    event_loop();

    return 0;
}
