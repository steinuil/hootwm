hootwm
=====
A very simple tiling window manager written from scratch using C and xcb, inspired by catwm.

hootwm ignores all keyboard events, but it can be controlled by sending commands to the named pipe located in `/tmp/hoot` by default.
A grabbing keys utility such as `xbindkeys` or `sxhkd` may be used to implement hotkeys.

## Commands
| Command | Arguments | Description |
| ------- | --------- | ----------- |
| `move`  | `up/down` | Move current window up or down |
| `focus` | `up/down` | Shift focus up or down |
| `quit`  | -         | Exit the program cleanly |

Example:
    echo move down > /tmp/hoot

Initial gap and border size, master proportion and pipe filename can be defined at compile time by editing `config.h`.
A config file to be evaluated at startup may be added later.

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
