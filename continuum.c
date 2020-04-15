#include <X11/Xlib.h>

int main(argc, argv)
{
    Display *display;
    display = XOpenDisplay(NULL);
    int screen = XDefaultScreen(display);
    Window root_window = XRootWindow(display, screen);
    int width = DisplayWidth(display, screen);
    int height = DisplayHeight(display, screen);
    XWarpPointer(display, None, None, 0, 0, 0, 0, 10, 10);
    XFlush(display);
}
