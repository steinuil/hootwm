hootwm
=====
A very simple tiling window manager written from scratch using C and xcb, inspired by catwm.

hootwm itself doesn't support keyboard events, but it can be controlled by sending commands using `hootwm -c <command>` (not actually implemented yet). A grabbing keys program such as `xbindkeys` or `sxhkd` may be used to implement hotkeys.

## Modes
hootwm tiles windows using one master area and a stacking area on the right.

    +--------+---+
    |        |   |
    |        +---+
    | Master |   |
    |        +---+
    |        |   |
    +--------+---+
    Tiled

More modes will be added in the future.

## Installation
 * Install xcb
 * Compile with `make`
