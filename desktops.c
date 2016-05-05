void desktop_switch(uint8_t i) {
    if (i > DESKTOPS || current_desktop == desktops[i-1]) return;
    for (node *w = head; w; w = w->next) {
        xcb_unmap_window(conn, w->win);
    }

    current_desktop->head = head;
    current_desktop->current = current;

    current_desktop = desktops[i-1];
    head = current_desktop->head;
    current = current_desktop->current;

    for (node *w = head; w; w = w->next) {
        xcb_map_window(conn, w->win);
    }
}
