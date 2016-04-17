#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <xcb/xcb.h>



xcb_connection_t *connection;
xcb_screen_t     *screen;
xcb_drawable_t    root;
xcb_drawable_t    win;

void setup() {
    



}


int main(int argc, char* argv[]) {
    connection = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(connection)) return 1;

    screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
    root = screen->
