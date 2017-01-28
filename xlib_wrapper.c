//
// Created by isthisloss on 13.01.17.
//

#include <X11/Xlib.h>
#include <stdlib.h>
#include <stdio.h>
#include "xlib_wrapper.h"

static Display* display = NULL;
static XEvent* click_event = NULL;

void xlw_init()
{
    display = XOpenDisplay(NULL);

    click_event = (XEvent*)malloc(sizeof(XEvent));
}

void xlw_close()
{
    XCloseDisplay(display);
    free(click_event);
}

void xlw_mouse_move(int dx, int dy)
{
    XWarpPointer(display, None, None, 0, 0, 0, 0, dx, dy);
    XFlush(display);
}

void xlw_mouse_click()
{
    click_event->xbutton.button = Button1;
    click_event->xbutton.same_screen = True;
    click_event->xbutton.subwindow = DefaultRootWindow(display);
    while (click_event->xbutton.subwindow)
    {
        click_event->xbutton.window = click_event->xbutton.subwindow;
        XQueryPointer (display, click_event->xbutton.window,
                       &click_event->xbutton.root, &click_event->xbutton.subwindow,
                       &click_event->xbutton.x_root, &click_event->xbutton.y_root,
                       &click_event->xbutton.x, &click_event->xbutton.y,
                       &click_event->xbutton.state);
    }


    click_event->type = ButtonPress;
    if (XSendEvent (display, PointerWindow, True, ButtonPressMask, click_event) == 0)
        fprintf(stderr, "Error to send the event!\n");
    click_event->type = ButtonRelease;
    if (XSendEvent (display, PointerWindow, True, ButtonReleaseMask, click_event) == 0)
        fprintf (stderr, "Error to send the event!\n");
    XFlush(display);
}
