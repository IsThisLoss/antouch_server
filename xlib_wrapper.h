//
// Created by isthisloss on 13.01.17.
//

#ifndef ANTOUCH_SERVER_XLIB_WRAPPER_H
#define ANTOUCH_SERVER_XLIB_WRAPPER_H

Display* xlw_get_display();
void xlw_close_display(Display* display);

void xlw_mouse_half_click(Display* display, uint8_t state);
void xlw_mouse_click(Display* display, unsigned int button);
void xlw_mouse_move(Display* display, int dx, int dy);
void xlw_mouse_scroll(Display* display, int dy);
void xlw_key_press(Display* display, KeySym key_sum, KeySym mod_sym);

#endif //ANTOUCH_SERVER_XLIB_WRAPPER_H
