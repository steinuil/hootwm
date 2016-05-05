void win_create(xcb_window_t win) {
    node *w = (node*)calloc(1, sizeof(node));
    w->win = win;
    w->next = NULL; w->prev = NULL;

    node_insert_at_tail(w);
    current = w;
}

void win_destroy(xcb_window_t win) {
    node *w;
    int8_t i;
    for (i = DESKTOPS; i; --i) {
        w = desktops[i-1]->head;
        while (w) {
            if (w->win == win) goto rest;
            w = w->next;
        }
    }

    if (!w) return;
rest:
    node_remove(w);

    if (w == current) {
        if (w->next) current = w->next;
        else current = w->prev;
    }

    if (w == desktops[i-1]->head) desktops[i-1]->head = NULL;
    if (w == desktops[i-1]->current) desktops[i-1]->current = NULL;

    free(w);
}

void win_swap(node *a, node *b) {
    if (a == current) current = b;
    else if (b == current) current = a;

    node_swap(a, b);
}

void win_move_down(node *w) {
    if (w->next) {
        if (w == current) current = w->next;
        node_swap(w, w->next);
    }
}

void win_move_up(node *w) {
    if (w != head) {
        if (w == current) current = w->prev;
        node_swap(w, w->prev);
    }
}
