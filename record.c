#include <stdio.h>
#include <X11/Xlib.h>
#include <X11/extensions/record.h>
#include <X11/Xlibint.h>
#include <X11/keysym.h>
#include <stdlib.h>
#include <string.h>
#include "flags.h"

Display *display;
XRecordContext ctx;
void (*toggle_fn)();
int toggle_key;

static void callback(XPointer prt, XRecordInterceptData *data)
{
    printf("Getting here");
    fflush(stdout);
    XEvent test_event;
    XNextEvent(display, &test_event);
    if (data->category == XRecordFromServer)
    {
        xEvent *event;
        event = (xEvent*)data->data;
        if (event->u.u.type == KeyRelease)
        {

            XEvent *btn_evt;
            memset(btn_evt, 0, sizeof(XEvent));
            if (!_XWireToEvent(display, btn_evt, event))
            {
                fprintf(stderr, "Error converting xEvent to XEvent");
                return;
            }
            /* printf("CODE: %d\nSTATE: %d\n", btn_evt->xbutton.button, btn_evt->xbutton.state); */
            /* printf("TOGGLE-KEY: %d\n", toggle_key); */
            /* printf("KEY: %d\n", btn_evt->xbutton.button); */
            if (btn_evt->xbutton.button == toggle_key
                    && btn_evt->xbutton.state == 12)
            {
                toggle_fn();
            }
        }
    }
    fflush(stdout);
    XRecordFreeData(data);
}

int start_recording(void (*toggle)())
{
    toggle_fn = toggle;
    display = XOpenDisplay(0);
    toggle_key = XKeysymToKeycode(display, XK_Escape);
    XRecordRange *range = XRecordAllocRange();
    if (range == 0)
    {
        fprintf(stderr, "Unable to allocate range");
        return RECORD_FAILURE;
    }
    XRecordClientSpec clients = XRecordAllClients;
    memset(range, 0, sizeof(XRecordRange));
    range->device_events.first = KeyRelease;
    range->device_events.last = KeyRelease;
    if ((ctx = XRecordCreateContext(display, 0, &clients, 1, &range, 1)) == 0)
    {
        fprintf(stderr, "Error creating context");
        return RECORD_FAILURE;
    }

    XFree(range);

    XSync(display, True);

    Display *data_link_display;

    if ((data_link_display = XOpenDisplay(0)) == 0)
    {
        fprintf(stderr, "ERror opening data_link_display");
        return RECORD_FAILURE;
    }

    if (!XRecordEnableContext(data_link_display, ctx, callback, (XPointer) start_recording))
    {
        fprintf(stderr, "Error enabling recording");
        return RECORD_FAILURE;
    }

    return RECORD_SUCCESS;
}

int stop_recording()
{
    XRecordDisableContext(display, ctx);
    XRecordFreeContext(display, ctx);
}
