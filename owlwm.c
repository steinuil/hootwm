#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <signal.h>
#include <xcb/xcb.h>

#define XCB_MOVE XCB_CONFIG_WINDOW_X | XCB_CONFIG_WINDOW_Y
#define XCB_RESIZE XCB_CONFIG_WINDOW_WIDTH | XCB_CONFIG_WINDOW_HEIGHT
#define XCB_MOVE_RESIZE XCB_MOVE | XCB_RESIZE
#define DIE(err) printf("owlwm: %s\n", err); exit(1)

typedef struct node node;
struct node {
    xcb_window_t *win;
    struct node *next;
    struct node *prev;
} master, head, current;

xcb_connection_t *conn;
xcb_screen_t     *screen;
xcb_drawable_t    root;
uint16_t          s_width;
uint16_t          s_height;
uint16_t          master_size;

bool run;

void add_node(node *c, node *prev);
void remove_node(node *c);
void move_node(node *c, bool up);

void add_window(xcb_window_t win);
void swap_master();
void destroy_window();

void map_request(xcb_map_request_event *ev);
void destroy_notify(xcb_destroy_notify_event *ev);
void loop();
void setup();

// Manage windows
void add_window(xcb_window_t win) {
    node *c = (node*)calloc(1,sizeof(node));
    c->win = win;
    if (!master) {
        master = c;
    } else if (head == NULL) {
        add_node(c, NULL);
        head = c;
    } else {
        add_node(c, head->prev);
    }

    current = c;
}

void swap_master() {
    node *c = current;

    remove_node(c);
    add_node(master, c->prev);
    if (head == c) head = master;

    master = c;
}

void destroy_window(node *c) {
    if (c == master) {
        if (head = NULL) {
            master = NULL;
            current = NULL;
        } else {
            remove_node(head);
            master = head;
            if (head == head->next) {
                head = NULL;
            } else {
                head = head->next;
            }
        }
    } else if (c == c->next) {
        head = NULL;
        current = master;
    } else {
        remove_node(c);
        current = c->next;
    }

    free(c);
}

// Manage window list
void add_node(node *c, node *prev) {
    if (prev == NULL) {
        c->next = c;
        c->prev = c;
    } else {
        c->next = prev->next;
        c->prev = prev;
        prev->next->prev = c;
        prev->next = c;
    }
}

void remove_node(node *c) {
    c->prev->next = c->next;
    c->next->prev = c->prev;
}

void move_node(node *c, bool up) {
    node *tmp;

    if (up && c->prev != head) {
        tmp = c->prev->prev;

        c->prev->prev = c;
        c->prev->next = c->next;
        c->next = c->prev;
        c->prev = tmp;
        c->prev->next = c;
        c->next->next->prev = c->next;
    } else if (c->next != head) {
        tmp = c->next->next;

        c->next->prev = c->prev;
        c->next->next = c;
        c->prev = c->next;
        c->next = tmp;
        c->next->prev = c;
        c->prev->prev->next = c->prev;
    }
}

// Move windows
void move_resize(xcb_drawable_t w, const uint16_t x, const uint16_t y,
        const uint16_t width, const uint16_t height) {
    uint32_t values[4] = { x, y, width, height }
    xcb_configure_window(conn, w, XCB_MOVE_RESIZE, values);
}

void tile() {
    if (master == NULL) {
        return;
    } else if (head == NULL) {
        uint16_t g = (monocle) ? 0 : gap;
        move_resize(master->win, g, g, screen_w-bord*2, screen_h-bord*2);
    } else if (monocle) {
        move_resize(master->win, 0, 0, );
    } else {
        move_resize(master->win, gap, gap, master_size - gap*2 - bord*2,
                s_height - gap*2 - bord*2);
        node *tmp = NULL;
        uint8_t y, n = 0;

        while (tmp != head) { if (tmp == NULL) tmp = head; ++n, tmp = tmp->next; }
        tmp = NULL;
        while (tmp != head) {
            if (tmp == NULL) tmp = head;
            moveresize(tmp->win, master_size + gap, y + gap,
                    s_width - master_size - gap*2 - bord*2,
                    (s_height / n) - gap*2 - bord*2);
            y += s_height / n;
    }
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


void map_request(xcb_map_request_event *ev) {
    add_window(ev->window);
    xcb_map_window(conn, ev->window);

    tile();
}

void destroy_notify(xcb_destroy_notify_event *ev) {
    node *c = NULL;

    if (current == NULL) {
        c = master;
    } else {
        node *tmp = NULL;
        while (!(tmp == current)) {
            if (tmp == NULL) tmp = current;
            if (tmp->win == ev->window) { c = tmp; break; }
            tmp = tmp->next;
        }
        if (c == NULL) c = master;
    }

    destroy_window(c);
    tile();
}

void loop() {
    xcb_generic_event_t  *ev;

    while (run && ev = xcb_wait_for_event(conn)) {
        switch (ev->response_type & ~0x80) {
        case XCB_MAP_REQUEST: {
            map_request((xcb_map_request_event_t*)ev); xcb_flush(conn); break; }
        case XCB_DESTROY_NOTIFY: {
            destroy_notify((xcb_destroy_notify_event*)ev); xcb_flush(conn); break; }
        default: break; }

        free(ev);
    }
}

void quit() {
    run = false;
    puts("ma rippe,,");
    xcb_disconnect(conn);
    exit(0);
}

int main(int argc, char* argv[]) {
    conn = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(conn)) { DIE("Cannot open display"); }

    setup();

    signal(SIGINT, quit);

    loop();

    return 0;
}
