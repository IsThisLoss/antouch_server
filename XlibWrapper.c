#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <stdint.h>

#include "XlibWrapper.h"

//
// Created by isthisloss on 13.01.17.
//

Display* xlw_get_display()
{
    return XOpenDisplay(NULL);
}

void xlw_close_display(Display* display)
{
    XCloseDisplay(display);
}

void xlw_mouse_half_click(Display* display, uint8_t state)
{
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, Button1, state,  CurrentTime);

    XSync(display, 0);
}

void xlw_mouse_click(Display* display, unsigned int button)
{
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, button, True,  CurrentTime);
    XTestFakeButtonEvent(display, button, False,  CurrentTime);

    XSync(display, 0);
}

void xlw_mouse_move(Display* display, int dx, int dy)
{
    XTestFakeRelativeMotionEvent(display, dx, dy, 0);
    XSync(display, 0);
}

void xlw_mouse_scroll(Display* display, int dy)
{
    if (dy < 0)
        xlw_mouse_click(display, Button4);
    else
        xlw_mouse_click(display, Button5);
}

//https://bharathisubramanian.wordpress.com/2010/03/14/x11-fake-key-event-generation-using-xtest-ext/
void xlw_key_press(Display* display, KeySym key_sum, KeySym mod_sym)
{
    KeyCode key_code = 0, mod_code = 0;
    key_code = XKeysymToKeycode(display, key_sum);

    if (key_code == 0)
        return;

    XTestGrabControl(display, True);

    if (mod_sym != 0)
    {
        mod_code = XKeysymToKeycode(display, mod_sym);
        XTestFakeKeyEvent(display, mod_code, True, CurrentTime);
    }

    XTestFakeKeyEvent(display, key_code, True, CurrentTime);
    XTestFakeKeyEvent(display, key_code, False, CurrentTime);

    if (mod_sym != 0)
        XTestFakeKeyEvent(display, mod_code, False, CurrentTime);

    XSync(display, False);
    XTestGrabControl(display, False);
}
