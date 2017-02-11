//
// Created by isthisloss on 13.01.17.
//

#include <X11/Xlib.h>
#include "xlib_wrapper.h"
#include <X11/extensions/XTest.h>
static Display* display = NULL;
static XEvent* click_event = NULL;

void xlw_init()
{
    display = XOpenDisplay(NULL);
}

void xlw_close()
{
    XCloseDisplay(display);
}

void xlw_mouse_move(int dx, int dy)
{
    XTestFakeRelativeMotionEvent(display, dx, dy, 0);
    XFlush(display);
}

void xlw_mouse_right_click()
{
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, 1, True,  CurrentTime);
    XTestFakeButtonEvent(display, 1, False, CurrentTime);
    XFlush(display);
}

void xlw_mouse_click(unsigned int button)
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
    XTestFakeButtonEvent(display, button, False, CurrentTime);

    XFlush(display);
}
