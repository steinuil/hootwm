void dispatch_command(char *command) {
    /*uint16_t w = 0, wi = 0, i;
    char comms[6][255];

    for (i = 0 ; command[i] != '\0' && command[i] != '\n' ; ++i) {
        if (command[i] != ' ') {
            comms[w][wi] = command[i];
            ++wi;

        } else {++w; ++wi }
    }*/

    if (!strcmp("tfw", command)) {
        p("moving current up");
        win_move_up(current);
        tile();
        update_current();
    } else if (!strcmp("lel", command)) {
        p("shifting focus up");
        current = current->prev;
        update_current();
    } else if (!strcmp("kek", command)) {
        p("shifting focus down");
        current = current->next;
        update_current();
    } else if (!strcmp("p", command)) {
        uint8_t n = 0;
        node *w = head;
        while (w) {
            printf("%d - %d %s\n", n, w->win, (current == w)?"c":" ");
            ++n; w = w->next;
        }
    }

}
