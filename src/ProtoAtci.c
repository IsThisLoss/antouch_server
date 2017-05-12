#include <stdlib.h>
#include <X11/X.h>
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <stdint.h>

#include "ProtoAtci.h"
#include "XlibWrapper.h"

//
// Created by isthisloss on 15.04.17.
//


struct ProtoAtci* atci_init()
{
    struct ProtoAtci* atci = (struct ProtoAtci*)malloc(sizeof(struct ProtoAtci));

    atci->key_syms = (KeySym*)malloc(ATCI_KEY_SYMS_SIZE * sizeof(KeySym));
    atci->text = (char*)malloc(256 * sizeof(char));

    atci->display = xlw_get_display();
    atci->xdo = xdo_new_with_opened_display(atci->display, NULL, 0);

    atci->key_syms[ATCI_BUTTON_UP - ATCI_BUTTON_UP] = XK_Up;
    atci->key_syms[ATCI_BUTTON_DOWN - ATCI_BUTTON_UP] = XK_Down;
    atci->key_syms[ATCI_BUTTON_LEFT - ATCI_BUTTON_UP] = XK_Left;
    atci->key_syms[ATCI_BUTTON_RIGHT - ATCI_BUTTON_UP] = XK_Right;

    atci->key_syms[ATCI_BUTTON_ESC - ATCI_BUTTON_UP] = XK_Escape;
    atci->key_syms[ATCI_BUTTON_ENTER - ATCI_BUTTON_UP] = XK_KP_Enter;
    atci->key_syms[ATCI_BUTTON_BACKSPACE - ATCI_BUTTON_UP] = XK_BackSpace;

    atci->key_syms[ATCI_BUTTON_VOL_DOWN - ATCI_BUTTON_UP] = XF86XK_AudioLowerVolume;
    atci->key_syms[ATCI_BUTTON_VOL_UP - ATCI_BUTTON_UP] = XF86XK_AudioRaiseVolume;

    atci->key_syms[ATCI_BUTTON_MEDIA_LAST - ATCI_BUTTON_UP] = XF86XK_AudioPrev;
    atci->key_syms[ATCI_BUTTON_MEDIA_NEXT - ATCI_BUTTON_UP] = XF86XK_AudioNext;
    atci->key_syms[ATCI_BUTTON_MEDIA_PLAY - ATCI_BUTTON_UP] = XF86XK_AudioPlay;
    atci->key_syms[ATCI_BUTTON_MEDIA_PAUSE - ATCI_BUTTON_UP] = XF86XK_AudioPause;

    return atci;
}

void atci_close(struct ProtoAtci* atci)
{
    xdo_free(atci->xdo);
    xlw_close_display(atci->display);
    free(atci);
}

uint8_t atci_get_type(uint8_t head)
{
    uint8_t tmp = (uint8_t)(head & 0x03);
    if (tmp != 0)
        return tmp & ~(head >> 2);
    return tmp;
}

void atci_command(struct ProtoAtci* atci, uint8_t head)
{
    uint8_t head_body = head >> 2;
    if (head_body < 2)
    {
        xlw_mouse_half_click(atci->display, head_body);
    }
    else if (head_body < ATCI_BUTTON_UP)
    {
        unsigned int button = 0;
        if (head_body == ATCI_LEFT_CLICK)
            button = Button1;
        else if (head_body == ATCI_RIGHT_CLICK)
            button = Button3;
        else
            return;
        xlw_mouse_click(atci->display, button);
    }
    else
    {
        xlw_key_press(atci->display, atci->key_syms[(head >> 2) - ATCI_BUTTON_UP], 0);
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

void atci_text(struct ProtoAtci* atci)
{
    xdo_enter_text_window(atci->xdo, CURRENTWINDOW, atci->text, 0);
}
