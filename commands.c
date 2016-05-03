void dispatch_command(char *command) {
    /*uint16_t w = 0, wi = 0, i;
    char comms[6][255];

    for (i = 0 ; command[i] != '\0' && command[i] != '\n' ; ++i) {
        if (command[i] != ' ') {
            comms[w][wi] = command[i];
            ++wi;

        } else {++w; ++wi }
    }*/

    p("top");
    if (!strcmp("tfw", command)) {
        p("lel");
        win_move_up(current);
        update_current();
    } else if (!strcmp("lel", command)) {
        p("hue");
        current = current->prev;
        update_current();
    }
}
