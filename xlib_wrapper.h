//
// Created by isthisloss on 13.01.17.
//

#ifndef ANTOUCH_SERVER_XLIB_WRAPPER_H
#define ANTOUCH_SERVER_XLIB_WRAPPER_H

#define XLW_LEFT_CLICK      1
#define XLW_RIGHT_CLICK     3
#define XLW_WHEEL_CLICK     2
#define XLW_WHEEL_UP        4
#define XLW_WHEEL_DOWN      5

void xlw_init();
void xlw_close();
void xlw_mouse_move(int dx, int dy);
void xlw_mouse_click(unsigned int button);


#endif //ANTOUCH_SERVER_XLIB_WRAPPER_H
