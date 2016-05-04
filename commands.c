void dispatch_command(char command[]) {
    char comm_list[2][16];
    uint16_t w = 0, // word
             wi = 0, // word index
             i;
    
    // I swear I'll make this into a nicer loop someday
    for (i = 0; command[i] != '\0' && w < 2; ++i) {
        if (command[i] != ' ') {
            comm_list[w][wi] = command[i];
            ++wi;
        } else {
            comm_list[w][wi] = '\0';
            ++w; wi = 0;
        }
    }
    comm_list[w][wi] = '\0';

    if (w == 0) {
        if (!strcmp("quit", comm_list[0])) {
            quit();
        }
    }

    if (w < 1 || !current) return;

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
    } else if (!strcmp("grow", comm_list[0])) {
        int32_t s = strtonum(comm_list[1], -screen_w, screen_w, NULL);
        if (!s) {
            return;
        } else if (s > 0) {
            if (s + master_size > screen_w - (MIN + bord*2 + gap*2)) {
                master_size  = screen_w - (MIN + bord*2 + gap*2);
            } else master_size += s;
        } else {
            if (s + master_size < (MIN + bord*2 + gap*2)) {
                master_size = MIN + bord*2 + gap*2;
            } else master_size += s;
        }

        tile();
        update_current();
    }
}
