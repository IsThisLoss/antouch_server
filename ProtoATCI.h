//
// Created by isthisloss on 15.04.17.
//

#ifndef ANTOUCH_SERVER_PROTOATCI_H
#define ANTOUCH_SERVER_PROTOATCI_H

#include <X11/Xlib.h>

#define COMMAND   0x00
#define V_SCROLL  0x01
#define MOVE      0x02
#define TEXT      0x03


#define HOLD_ON   0x01 
#define HOLD_OFF  0x00 // means 0x0 

// 10 < x < 20 are mouse click events
#define LEFT_CLICK   0x02 
#define RIGHT_CLICK  0x03 

// 20 < x < 30 are mouse motions events

#define KEY_SYMS_SIZE 6
// x >= 0x06 are keys' events
#define BUTTON_UP        0x06 
#define BUTTON_DOWN      0x07 
#define BUTTON_LEFT      0x08 
#define BUTTON_RIGHT     0x09 
#define BUTTON_VOL_DOWN  0x0A 
#define BUTTON_VOL_UP    0x0B 

struct ProtoAtci
{
    KeySym key_syms[KEY_SYMS_SIZE];
    Display* display;
};


struct ProtoAtci* atci_init();
void atci_close(struct ProtoAtci* atci);

uint8_t atci_get_type(uint8_t head);
void atci_command(struct ProtoAtci* atci, uint8_t head);
void atci_mouse_move(struct ProtoAtci* atci, int dx, int dy);
void atci_mouse_scroll(struct ProtoAtci* atci, int dy);


#endif //ANTOUCH_SERVER_PROTOATCI_H
