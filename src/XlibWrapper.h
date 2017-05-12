//
// Created by isthisloss on 13.01.17.
//

#ifndef ANTOUCH_SERVER_XLIB_WRAPPER_H
#define ANTOUCH_SERVER_XLIB_WRAPPER_H

/**
 * @brief wrapper of XOpenDisplay(NULL)
 * @return pointer to xlib Display
 */
Display* xlw_get_display();

/**
 * @brief wrapper of XCloseDisplay(display)
 * @param display is a pointer to Display to close
 */
void xlw_close_display(Display* display);

/**
 * @brief "half" left mouse button event
 * It is used to emulate text selection, by other words to hold on or
 * hold off left mouse key
 * @param display is a pointer to xlib Display
 * @param state is a switch, if state == true, this will hold on left mouse key and
 * hold off otherwise
 */
void xlw_mouse_half_click(Display* display, uint8_t state);

/**
 * @brief emulates mouse click
 * @param display is a pointer to xlib Display
 * @param button is a number of button to click, use predefined constants
 * such like Button1, Button3 and so on
 */
void xlw_mouse_click(Display* display, unsigned int button);

/**
 * @brief emulates relative mouse move
 * @param display is a pointer to xlib Display
 * @param dx is a horizontal offset
 * @param dy is a vertical offset
 */
void xlw_mouse_move(Display* display, int dx, int dy);

/**
 * @brief emulates mouse scroll
 * @param display is a pointer to xlib Display
 * @param dy is a horizontal offset [IN FACT, IT IS NOT USED]
 */
void xlw_mouse_scroll(Display* display, int dy);

/**
 * @brief emulates key ecents
 * @param display is a pointer to xlib Display
 * @param key_sum is a KeySym to press
 * @param mod_sym is a Key to hold on before press key_sum
 */
void xlw_key_press(Display* display, KeySym key_sum, KeySym mod_sym);

#endif //ANTOUCH_SERVER_XLIB_WRAPPER_H
