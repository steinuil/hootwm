void dispatch_command(char *command) {
    // This could be made a function but
    // it's way easier than figuring out
    // how to return an array and then
    // zeroing it when you're done.

    char comm_list[2][16];
    uint16_t w = 0, // word
             wi = 0, // word index
             i;
    
    // fugg: last string is not null terminated
    for (i = 0; command[i] != '\0' && w < 2; ++i) {
        if (command[i] != ' ') {
            comm_list[w][wi] = command[i];
            ++wi;
        } else {
            comm_list[w][wi] = '\0';
            ++w; w = 0;
        }
    }

    // I wish I could make this a switch
    if (w < 2 || !current) return;
    if (!strcmp("move", comm_list[0])) {
        if (!strcmp("up", comm_list[1])) {
            win_move_up(current);
            tile();
            update_current();
        } else if (!strcmp("down", comm_list[1])) {
            win_move_down(current);
            tile();
            update_current();
        }
    } else if (!strcmp("focus", comm_list[0])) {
        if (!strcmp("up", comm_list[1])) {
            if (current->prev) current = current->prev;
            update_current();
        } else if (!strcmp("down", comm_list[1])) {
            if (current->next) current = current->next;
            update_current();
        }
    }

/*
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
        node *w;
        for (w = head; w; ++n, w = w->next) {
            printf("%d - %d %s\n", n, w->win,
                (current == w)?"c":" ");
        }
    }*/
}
