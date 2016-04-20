typedef struct node node;
struct node {
    xcb_window_t *win;
    struct node *next;
    struct node *prev;
};

node *master, *head, *current;

void insert_node(node *c, node *prev) {
    if (!prev) {
        c->prev = NULL;
        c->next = master;

        if (master) master->prev = c;

        head = master;
        master = c;
    } else if (prev == master) {
        c->prev = master;
        c->next = head;

        master->next = c; 

        head = c;
    } else {
        c->prev = prev;
        c->next = prev->next;
        
        prev->next = c;
    }

    if (prev && prev->next) prev->next->prev = c;
}

void remove_node(node *c) {
    if (!c->prev) {
        master = c->next;
        if (head) head = c->next->next;
    } else if (c->prev == master) {
        head = c->next;
    }

    if (c->next) c->next->prev = c->prev;
    if (c->prev) c->prev->next = c->next;
}
