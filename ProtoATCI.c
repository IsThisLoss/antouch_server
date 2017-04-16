#include <stdlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <stdint.h>

#include "ProtoATCI.h"
#include "xlib_wrapper.h"

//
// Created by isthisloss on 15.04.17.
//

struct ProtoAtci* atci_init()
{
    struct ProtoAtci* atci = (struct ProtoAtci*)malloc(sizeof(struct ProtoAtci));
    atci->display = xlw_get_display();
    atci->key_syms[BUTTON_UP - BUTTON_UP] = XK_Up;
    atci->key_syms[BUTTON_DOWN - BUTTON_UP] = XK_Down;
    atci->key_syms[BUTTON_LEFT - BUTTON_UP] = XK_Left;
    atci->key_syms[BUTTON_RIGHT - BUTTON_UP] = XK_Right;
    atci->key_syms[BUTTON_VOL_DOWN - BUTTON_UP] = XF86XK_AudioLowerVolume;
    atci->key_syms[BUTTON_VOL_UP - BUTTON_UP] = XF86XK_AudioRaiseVolume;
    return atci;
}

void atci_close(struct ProtoAtci* atci)
{
    xlw_close_display(atci->display);
    free(atci);
}

uint8_t atci_get_type(uint8_t head)
{
    return (uint8_t)(head & 0x03);
}

void atci_command(struct ProtoAtci* atci, uint8_t head)
{
    uint8_t head_body = head >> 2;
    if (head_body < 2)
    {
        xlw_mouse_half_click(atci->display, head_body);
    }
    else if (head_body < BUTTON_UP)
    {
        unsigned int button = 0;
        if (head_body == LEFT_CLICK)
            button = Button1;
        else if (head_body == RIGHT_CLICK)
            button = Button3;
        else
            return;
        xlw_mouse_click(atci->display, button);
    }
    else
    {
        xlw_key_press(atci->display, atci->key_syms[(head >> 2) - BUTTON_UP], 0);
    }
}

void atci_mouse_move(struct ProtoAtci* atci, int dx, int dy)
{
    xlw_mouse_move(atci->display, dx, dy);
}

void atci_mouse_scroll(struct ProtoAtci* atci, int dy)
{
    xlw_mouse_scroll(atci->display, dy);
}
