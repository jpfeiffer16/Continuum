// #include <X11/extensions/Xevie.h>
#include <stdio.h>
#include <unistd.h>
#include <X11/Xlib.h>

int main()
{
    /* Display *display; */
    /* display = XOpenDisplay(NULL); */
    /* int screen = XDefaultScreen(display); */
    /* Window root_window = XRootWindow(display, screen); */
    /* int width = DisplayWidth(display, screen); */
    /* int height = DisplayHeight(display, screen); */
    /* XWarpPointer(display, None, None, 0, 0, 0, 0, 10, 10); */
    /* XFlush(display); */
    Display *display;
    /* Window win; */
    XEvent event;
    display = XOpenDisplay(NULL);
    if (!display) {
        fprintf(stderr, "Failed to open display");
        return -1;
    }
    XGrabPointer(display, DefaultRootWindow(display), True, 0, GrabModeAsync, GrabModeAsync, None, None, CurrentTime);
    while (1)
    {
        XNextEvent(display, &event);
        printf("%d", event.type);
        sleep(1);
    }
    return 0;
}
