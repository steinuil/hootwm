#ifndef CONFIG_H
#define CONFIG_H

// Mod, master size and ricer gaps
#define MOD             Mod4Mask
#define MASTER_SIZE     0.7
#define BORDER		3
#define GAP		6

// Colors
#define FOCUS           "rgb:cc/24/1d"
#define UNFOCUS         "rgb:45/85/88"
#define MONOCLE         "rgb:68/9d/6a"

// Avoid multiple paste
#define DESKTOPCHANGE(K,N) \
    {  MOD,             K,                          change_desktop, {.i = N}}, \
    {  MOD|ShiftMask,   K,                          client_to_desktop, {.i = N}},

// Shortcuts
static struct key keys[] = {
    // MOD              KEY         FUNCTION        ARGS
    {  MOD,             XK_h,       decrease,       {NULL}},
    {  MOD,             XK_l,       increase,       {NULL}},
    {  MOD,             XK_x,       kill_client,    {NULL}},
    {  MOD,             XK_j,       next_win,       {NULL}},
    {  MOD,             XK_Tab,     next_win,       {NULL}},
    {  MOD,             XK_k,       prev_win,       {NULL}},
    {  MOD|ShiftMask,   XK_j,       move_down,      {NULL}},
    {  MOD|ShiftMask,   XK_k,       move_up,        {NULL}},
    {  MOD|ShiftMask,   XK_Return,  swap_master,    {NULL}},
    {  MOD|ShiftMask,   XK_space,   switch_mode,    {NULL}},
    {  MOD,             XK_Right,   next_desktop,   {NULL}},
    {  MOD,             XK_Left,    prev_desktop,   {NULL}},
       DESKTOPCHANGE(   XK_0,                       0)
       DESKTOPCHANGE(   XK_1,                       1)
       DESKTOPCHANGE(   XK_2,                       2)
       DESKTOPCHANGE(   XK_3,                       3)
       DESKTOPCHANGE(   XK_4,                       4)
       DESKTOPCHANGE(   XK_5,                       5)
       DESKTOPCHANGE(   XK_6,                       6)
       DESKTOPCHANGE(   XK_7,                       7)
       DESKTOPCHANGE(   XK_8,                       8)
       DESKTOPCHANGE(   XK_9,                       9)
    {  MOD|ShiftMask,   XK_q,       quit,           {NULL}}
};

#endif
