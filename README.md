CATWM
=====
     /\___/\
    ( o   o )  Made by cat...
    (  =^=  )
    (        )            ... for cat!
    (         )
    (          ))))))________________ Cute And Tiny Window Manager

## Summary
catwm is a very simple and lightweight tiling window manager.

## Modes
catwm allows vertical tiled mode with one master area and a stacking area, and monocle mode.

    +--------+---+    +------------+
    |        |   |    |            |
    |        +---+    |            |
    | Master |   |    |   Master   |
    |        +---+    |            |
    |        |   |    |            |
    +--------+---+    +------------+
    Tiled             Monocle

A horizontal stacking mode might be added in the future.

## Installation
 * Install Xlib
 * Edit `config.h`
 * Compile with `make`

## Changes from pyknite/catwm
 * Border width and gap size are now configurable
 * Fixed monocle mode bug where the window would bleed off-screen if the border was bigger than 0
 * Monocle mode now has a different color setting

## Bugs
 * Invoking a window in monocle mode overlays it to the fullscreen window
   * FIX: send it to next desktop?
 * Sending a window to another desktop causes the wm to crash
 * Compiler warnings
