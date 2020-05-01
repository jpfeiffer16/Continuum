#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <string.h>

int main()
{
    Display *display;
    Window   window, rootwindow;
    XEvent   event;
    KeySym   escape;

    display = XOpenDisplay(NULL);
    rootwindow = DefaultRootWindow(display);
    window = XCreateWindow(display, rootwindow,
                           -99, -99, 1, 1, /* x, y, width, height */
                           0, 0, InputOnly, /* border, depth, class */
                           CopyFromParent, /* visual */
                           0, NULL); /* valuemask and attributes */
    Display *redirect_display = XOpenDisplay(NULL);
    Window redirect_window = XCreateWindow(redirect_display, rootwindow, -99, -99, 1, 1, 0, 0, InputOnly, CopyFromParent, 0, NULL);

    XSelectInput(display, window, StructureNotifyMask | SubstructureRedirectMask | ResizeRedirectMask | KeyPressMask | KeyReleaseMask);
    XLowerWindow(display, window);
    XMapWindow(display, window);

    do {
        XNextEvent(display, &event);
    } while (event.type != MapNotify);

    XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
    XLowerWindow(display, window);

    escape = XKeysymToKeycode(display, XK_Escape);
    printf("\nPress ESC to exit.\n\n");
    fflush(stdout);

    while (1) {

        XNextEvent(display, &event);

        /* printf("%d\n", *((int*)event.xcookie.data)); */
        /* printf("%d\n", event.xcookie.data == NULL); */
        if (event.type == KeyPress) {
            /* printf("KeyPress: keycode %u state %u\n", event.xkey.keycode, event.xkey.state); */
            fflush(stdout);
            // Forward event
            XEvent forwared_event;
            forwared_event.type = KeyPress;
            forwared_event.xkey.state = event.xkey.state;
            forwared_event.xkey.keycode = event.xkey.keycode;
            Bool cookie_value = True;
            forwared_event.xcookie.data = &cookie_value;
            /* XUngrabKeyboard(display, CurrentTime); */
            XSendEvent(redirect_display, redirect_window, True, KeyPress, &forwared_event);
            XFlush(display);
            /* XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime); */

        } else
        if (event.type == KeyRelease) {

            /* printf("KeyRelease: keycode %u state %u\n", event.xkey.keycode, event.xkey.state); */
            fflush(stdout);

            if (event.xkey.keycode == escape)
                break;
            // Forward event
            XEvent forwared_event;
            forwared_event.type = KeyPress;
            forwared_event.xkey.state = event.xkey.state;
            forwared_event.xkey.keycode = event.xkey.keycode;
            Bool cookie_value = True;
            forwared_event.xcookie.data = &cookie_value;
            /* XUngrabKeyboard(display, CurrentTime); */
            XSendEvent(redirect_display, redirect_window, True, KeyRelease, &forwared_event);
            XFlush(display);
            /* XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime); */
        } else
        if (event.type == UnmapNotify) {

            XUngrabKeyboard(display, CurrentTime);
            XDestroyWindow(display, window);
            XDestroyWindow(redirect_display, redirect_window);
            XCloseDisplay(display);
            XCloseDisplay(redirect_display);

            display = XOpenDisplay(NULL);
            rootwindow = DefaultRootWindow(display);
            window = XCreateWindow(display, rootwindow,
                                   -99, -99, 1, 1, /* x, y, width, height */
                                   0, 0, InputOnly, /* border, depth, class */
                                   CopyFromParent, /* visual */
                                   0, NULL); /* valuemask and attributes */

            XSelectInput(display, window, StructureNotifyMask | SubstructureRedirectMask | ResizeRedirectMask | KeyPressMask | KeyReleaseMask);
            XLowerWindow(display, window);
            XMapWindow(display, window);

            do {
                XNextEvent(display, &event);
            } while (event.type != MapNotify);

            printf("Grabbing");
            fflush(stdout);
            XGrabKeyboard(display, window, True, GrabModeAsync, GrabModeAsync, CurrentTime);
            XLowerWindow(display, window);

            escape = XKeysymToKeycode(display, XK_Escape);

        } else {

            printf("Event type %d\n", event.type);
            fflush(stdout);
        }
    }

    XUngrabKeyboard(display, CurrentTime);

    XDestroyWindow(display, window);
    XCloseDisplay(display);
    return 0;
}
