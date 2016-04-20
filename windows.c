void add_window(xcb_window_t win) {
    node *c = (node*)calloc(1, sizeof(node));
    c->win = win;

    insert_node(c, NULL);

    current = c;
}

void destroy_window(xcb_window_t win) {
    node *c;

    for (c = master ; c ; c = c->next) {
        if (c->win == win) c = c->win;
    }

    remove_node(c);

    if (c == current) current = c->prev;

    free(c);
}

void swap_windows(node *c, node *dest) {
    node *from = c->prev;
    node *to = dest->prev;

    remove_node(dest); remove_node(c);

    insert_node(c, to); insert_node(dest, from);
}

void move_window_up(node *c) {
    if (c != master && c != head) {
        node *tmp = c->prev->prev;

        remove_node(c->prev); remove_node(c);

        insert_node(c, tmp); insert_node(c->prev, c);
    }
}

void move_window_down(node *c) {
    if (c->next && c != master) {
        node *tmp = c->prev;

        remove_node(c->next); remove_node(c);

        insert_node(c->next, tmp); insert_node(c, c->next);
    }
}
