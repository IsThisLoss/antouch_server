//
// Created by isthisloss on 13.01.17.
//

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>

#include "xlib_wrapper.h"
#include <X11/keysym.h>
#include <X11/XF86keysym.h>
#include <time.h>
#include <printf.h>

#include "key_defines.h"

static Display* display = NULL;
static XEvent* click_event = NULL;
static time_t last_click = 0;


static void xlw_mouse_fake_key_press(unsigned int btn);
static void xlw_fake_key_press(KeySym keySym, KeySym modSym);

void xlw_init()
{
    display = XOpenDisplay(NULL);
}

void xlw_close()
{
    XCloseDisplay(display);
}

// interface
// move mouse pointer
void xlw_mouse_move(int dx, int dy)
{
    XTestFakeRelativeMotionEvent(display, dx, dy, 0);
    XFlush(display);
}

// scroll by whele
void xlw_mouse_scroll(int dy)
{
    if (dy < 0)
        xlw_mouse_fake_key_press(Button4);
    else
        xlw_mouse_fake_key_press(Button5);
}

// press mouse key
void xlw_mouse_key(int key)
{
    unsigned int button;
    switch (key) {
        case LEFT_CLICK:
            button = Button1;
            break;
        case RIGHT_CLICK:
            button = Button3;
            break;
        default:
            return;
    }
    xlw_mouse_fake_key_press(button);
}

// hold on or hold off mouse text selection
void xlw_half_mouse_click(int cmd)
{
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, Button1, cmd,  CurrentTime);

    XFlush(display);
}

// press keyboard key
void xlw_key(int key)
{
    KeySym keySym;
    switch (key)
    {
        case BUTTON_UP:
            keySym = XK_Up;
            break;
        case BUTTON_DOWN:
            keySym = XK_Down;
            break;
        case BUTTON_LEFT:
            keySym = XK_Left;
            break;
        case BUTTON_RIGHT:
            keySym = XK_Right;
            break;
        case BUTTON_VOL_DOWN:
            keySym = XF86XK_AudioLowerVolume;
            break;
        case BUTTON_VOL_UP:
            keySym = XF86XK_AudioRaiseVolume;
            break;
        default:
            return;
    }
    xlw_fake_key_press(keySym, 0);
}

static void xlw_mouse_fake_key_press(unsigned int btn)
{
    XEvent event;
    /* Get the current pointer position */
    XQueryPointer(display, RootWindow(display, 0), &event.xbutton.root,
                  &event.xbutton.window, &event.xbutton.x_root,
                  &event.xbutton.y_root, &event.xbutton.x, &event.xbutton.y,
                  &event.xbutton.state);

    XSync(display, 0);

    /* Fake the mouse button Press and Release events */
    XTestFakeButtonEvent(display, btn, True,  CurrentTime);
    XTestFakeButtonEvent(display, btn, False,  CurrentTime);

    XFlush(display);
}

//https://bharathisubramanian.wordpress.com/2010/03/14/x11-fake-key-event-generation-using-xtest-ext/
static void xlw_fake_key_press(KeySym keySym, KeySym modSym)
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
