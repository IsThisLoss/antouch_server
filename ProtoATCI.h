//
// Created by isthisloss on 15.04.17.
//

#ifndef ANTOUCH_SERVER_PROTOATCI_H
#define ANTOUCH_SERVER_PROTOATCI_H

#include <X11/Xlib.h>
#include <xdo.h>

#define ATCI_COMMAND  0x00
#define ATCI_V_SCROLL  0x01
#define ATCI_MOVE  0x02
#define ATCI_TEXT  0x03

// half mouse codes < 2
#define ATCI_HOLD_ON  0x01
#define ATCI_HOLD_OFF  0x00

// mouse key codes < ATCI_BUTTON_UP
#define ATCI_LEFT_CLICK  0x02
#define ATCI_RIGHT_CLICK  0x03

// key codes > ATCI_BUTTON_UP
#define ATCI_BUTTON_UP  0x06
#define ATCI_BUTTON_DOWN  0x07
#define ATCI_BUTTON_LEFT  0x08
#define ATCI_BUTTON_RIGHT  0x09
#define ATCI_BUTTON_ESC  0x0A
#define ATCI_BUTTON_ENTER  0x0B
#define ATCI_BUTTON_BACKSPACE 0x0C

#define ATCI_BUTTON_VOL_DOWN  0x0D
#define ATCI_BUTTON_VOL_UP  0x0E
#define ATCI_BUTTON_MEDIA_LAST  0x0F
#define ATCI_BUTTON_MEDIA_NEXT  0x10
#define ATCI_BUTTON_MEDIA_PLAY  0x11
#define ATCI_BUTTON_MEDIA_PAUSE  0x12

#define ATCI_KEY_SYMS_SIZE 14
#define ATCI_TEXT_LIMIT 256

struct ProtoAtci
{
    KeySym key_syms[ATCI_KEY_SYMS_SIZE];
    Display* display;
    char text[256];
    ssize_t text_size;
    xdo_t* xdo;
};


struct ProtoAtci* atci_init();
void atci_close(struct ProtoAtci* atci);

uint8_t atci_get_type(uint8_t head);
void atci_command(struct ProtoAtci* atci, uint8_t head);
void atci_mouse_move(struct ProtoAtci* atci, int dx, int dy);
void atci_mouse_scroll(struct ProtoAtci* atci, int dy);
void atci_text(struct ProtoAtci* atci);


#endif //ANTOUCH_SERVER_PROTOATCI_H
