//
// Created by isthisloss on 13.01.17.
//

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "xlib_wrapper.h"
#include <X11/keysym.h>
#include <X11/XF86keysym.h>

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

//void xlw_mouse_right_click()
//{
//    XEvent event;
//    /* Get the current pointer position */
//    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
//                  &event.xbutton.window, &event.xbutton.x_root,
//                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
//                  &event.xbutton.state);
//
//    XSync(display, 0);
//
//    /* Fake the mouse button Press and Release events */
//    XTestFakeButtonEvent(display, 1, True,  CurrentTime);
//    XTestFakeButtonEvent(display, 1, False, CurrentTime);
//    XFlush(display);
//}

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

// todo deal with types

//https://bharathisubramanian.wordpress.com/2010/03/14/x11-fake-key-event-generation-using-xtest-ext/
static void xlw_send_key_event(KeySym keySym, KeySym modSym)
{
    KeyCode keyCode = 0, modCode = 0;
    keyCode = XKeysymToKeycode(display, keySym);

    if (keyCode == 0)
        return;

    XTestGrabControl(display, True);

    if (modSym != 0)
    {
        modCode = XKeysymToKeycode(display, modSym);
        XTestFakeKeyEvent(display, modCode, True, CurrentTime);
    }

    XTestFakeKeyEvent(display, keyCode, True, CurrentTime);
    XTestFakeKeyEvent(display, keyCode, False, CurrentTime);

    if (modSym != 0)
        XTestFakeKeyEvent(display, modCode, False, CurrentTime);

    XSync(display, False);
    XTestGrabControl(display, False);
}

//Here
void xlw_key_press(unsigned int key_cmd)
{
    switch (key_cmd)
    {
        case 10:
            xlw_send_key_event(XK_Up, 0);
            break;
        case 11:
            xlw_send_key_event(XK_Down, 0);
            break;
        case 12:
            xlw_send_key_event(XK_Left, 0);
            break;
        case 14:
            xlw_send_key_event(XK_Right, 0);
            break;
        case 15:
            xlw_send_key_event(XF86XK_AudioLowerVolume, 0);
            break;
        case 17:
            xlw_send_key_event(XF86XK_AudioRaiseVolume, 0);
            break;
    }
}

void xlw_selection_mode()
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

    XFlush(display);
}

void xlw_test_up() {
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, 1, False,  CurrentTime);

    XFlush(display);
}

void xlw_test_down() {
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, 1, True,  CurrentTime);

    XFlush(display);

}
