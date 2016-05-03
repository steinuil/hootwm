// Assumes next and prev are set to NULL
typedef struct node node;
struct node {
    xcb_window_t win;
    struct node *next;
    struct node *prev;
};

node *head;

void node_insert_at_head(node *n) {
    if (head) {
        head->prev = n;
        n->next = head;
    }
    head = n;
}

void node_insert_at_tail(node *n) {
    if (head) {
        node *tmp = head;
        while (tmp->next) tmp = tmp->next;
        tmp->next = n;
        n->prev = tmp;
    } else {
        head = n;
    }
}

void node_remove(node *n) {
    if (n == head) head = n->next;
    if (n->next) n->next->prev = n->prev;
    if (n->prev) n->prev->next = n->next;
}

void node_swap(node *a, node *b) {
    xcb_window_t tmp;
    tmp = a->win;
    a->win = b->win;
    b->win = tmp;
}
