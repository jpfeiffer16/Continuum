// ORIGNAL SOURCE: https://github.com/temporaryrespite/XGrabPointer/blob/master/xi-bug.c
//src: https://gitlab.freedesktop.org/xorg/xserver/issues/597
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/extensions/XInput2.h>
#include "record.h"

Display *dpy;
char capture_enabled = 0;

static Window create_win(Display *dpy)
{
    XIEventMask mask;

    Window win = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 200,
            200, 0, 0, WhitePixel(dpy, 0));
    Window subwindow = XCreateSimpleWindow(dpy, win, 50, 50, 50, 50, 0, 0,
            BlackPixel(dpy, 0));

    XSelectInput(dpy, win, StructureNotifyMask);

    mask.deviceid = XIAllMasterDevices;
    mask.mask_len = XIMaskLen(XI_RawMotion);
    mask.mask = calloc(mask.mask_len, sizeof(unsigned char));
    //memset(mask.mask, 0, mask.mask_len);
    explicit_bzero(mask.mask, mask.mask_len);

    XISetMask(mask.mask, XI_RawMotion);

    XISelectEvents(dpy, DefaultRootWindow(dpy), &mask, 1);

    free(mask.mask);
    XMapWindow(dpy, subwindow);
    XMapWindow(dpy, win);
    XSync(dpy, True);
    return win;
}

static void destroy_window(Display *dpy, Window *window)
{
    XDestroyWindow(dpy, *window);
}

//Write an arg to stdout
static size_t write_data_out(void *data, size_t size)
{
    fwrite(data, size, 1, stdout);
    return size;
}

// Pad stdout to 18 bytes
static void pad_data_out(int bytes_written)
{
    while (bytes_written < 18)
    {
        fprintf(stdout, "%c", '\0');
        ++bytes_written;
    }
}

static void print_rawmotion(XIRawEvent *event)
{
    int i;
    double *raw_valuator = event->raw_values,
           *valuator = event->valuators.values;

    //printf("    device: %d (%d)\n", event->deviceid, event->sourceid);
    /* fprintf(stdout, "m"); */
    
    // m
    char m = 109;
    write_data_out(&m, sizeof(char));

    for (i = 0; i < event->valuators.mask_len * 8; i++)
    {
        if (XIMaskIsSet(event->valuators.mask, i))
        {
            //printf("  acceleration on valuator %d: %f <%f %f>\n",
            //        i, *valuator - *raw_valuator, *valuator, *raw_valuator);
            /* fprintf(stdout, "|%f", *valuator - *raw_valuator); */
            double data = *valuator - *raw_valuator;
            /* fprintf(stderr, "VAL: %f\n", *valuator); */
            /* fprintf(stderr, "RAW: %f\n", *raw_valuator); */
            write_data_out(valuator, sizeof(double));
            /* fprintf(stderr, "%f\n", data); */
            /* fflush(stderr); */
            valuator++;
            raw_valuator++;
        }
    }
    pad_data_out(17);
    fflush(stdout);
}

static void print_rawkeypress(XIRawEvent *event)
{
    fprintf(stderr, "KEY");
    fflush(stderr);
    double *valuator = event->valuators.values;
    for (int i = 0; i < event->valuators.mask_len * 8; i++)
    {
        if (XIMaskIsSet(event->valuators.mask, i))
        {
            fprintf(stderr, "KEY: %f\n", *valuator);
            fflush(stderr);
        }
    }
}

static void toggle_capture()
{
    if (capture_enabled)
    {
        // Uncapture
        capture_enabled = 0;
    }
    else
    {
        /* stop_recording(); */
        /* XSync(dpy, True); */
        /* XFlush(dpy); */
        // Capture
        /* XGrabPointer(dpy, */
        /*     //win */
        /*     DefaultRootWindow(dpy) // no change when using this, well the mentioned bug(events duplicated) doesn't happen anymore, but I meant no change for my use case. */
        /*     //None //can't use this here */
        /*     , True, 0, GrabModeAsync, */
        /*         GrabModeAsync, */
        /*         //win */
        /*         //DefaultRootWindow(dpy) // needing this to unconstrain */
        /*         None //can use this here, same effect as root-win */
        /*         , None, CurrentTime); */
        /* XGrabButton(dpy, AnyButton, AnyModifier, */
        /*     //win */
        /*     DefaultRootWindow(dpy) */
        /*     , True, 0, GrabModeAsync, GrabModeAsync, */
        /*     //DefaultRootWindow(dpy) */
        /*     None //works */
        /*     , None); */
        /* XGrabKeyboard(dpy, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync, CurrentTime);  */
        /* XGrabButton(dpy, AnyButton, AnyModifier, DefaultRootWindow(dpy), True, 0, GrabModeAsync, GrabModeAsync, None, None); */
        capture_enabled = 1;
    }
    printf("Capture %s\n", capture_enabled ? "ON" : "OFF");
    fflush(stdout);
}


int main (int argc, char **argv)
{
    int xi_opcode, event, error;
    int major = 2, minor = 1;
    Window win;
    XEvent ev;

    dpy = XOpenDisplay(NULL);

    if (!dpy) {
        fprintf(stderr, "Failed to open display.\n");
        return -1;
    }

    if (!XQueryExtension(dpy, "XInputExtension", &xi_opcode, &event, &error)) {
        fprintf(stderr, "X Input extension not available.\n");
        return -1;
    }

    if (XIQueryVersion(dpy, &major, &minor) != Success) {
        return -1;
    }

    win = create_win(dpy); //yes this is still necessary!
    // destroy_window(dpy, &win);

    start_recording(toggle_capture);

    while(1)
    {
        XGenericEventCookie *cookie = &ev.xcookie;

        XNextEvent(dpy, &ev);

        if (ev.type == MapNotify) {
            /* XGrabPointer(dpy, */
            /*     //win */
            /*     DefaultRootWindow(dpy) // no change when using this, well the mentioned bug(events duplicated) doesn't happen anymore, but I meant no change for my use case. */
            /*     //None //can't use this here */
            /*     , True, 0, GrabModeAsync, */
            /*         GrabModeAsync, */
            /*         //win */
            /*         //DefaultRootWindow(dpy) // needing this to unconstrain */
            /*         None //can use this here, same effect as root-win */
            /*         , None, CurrentTime); */
            /* XGrabButton(dpy, AnyButton, AnyModifier, */
            /*     //win */
            /*     DefaultRootWindow(dpy) */
            /*     , True, 0, GrabModeAsync, GrabModeAsync, */
            /*     //DefaultRootWindow(dpy) */
            /*     None //works */
            /*     , None); */
            /* XGrabKeyboard(dpy, DefaultRootWindow(dpy), True, GrabModeAsync, GrabModeAsync, CurrentTime);  */
            /* XGrabButton(dpy, AnyButton, AnyModifier, DefaultRootWindow(dpy), True, 0, GrabModeAsync, GrabModeAsync, None, None); */
        }
        //if ((ev.type == KeyPress)||(cookie->type == KeyPress)||(cookie->evtype == KeyPress)) { // TODO: no idea how to make this work, Alt+F4 on 'win' is needed, or C-c on terminal(after an alt+tab)
        //  // types like KeyPress and MapNotify are listed in /usr/include/X11/X.h
        //  break;
        //}
        

        fprintf(stderr, "%d\n", cookie->evtype);
        fflush(stderr);

        if (cookie->type != GenericEvent ||
            cookie->extension != xi_opcode ||
            !XGetEventData(dpy, cookie))
            continue;

        //printf("EVENT TYPE %d\n", cookie->evtype);
        if (cookie->evtype == XI_RawKeyPress)
            print_rawkeypress(cookie->data);
        if (cookie->evtype == XI_RawMotion)
            print_rawmotion(cookie->data);

        XFreeEventData(dpy, cookie);
    }

    XCloseDisplay(dpy);
    return 0;
}

