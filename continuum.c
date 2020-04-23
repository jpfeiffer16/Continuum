#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>

#define BSIZE 18
#define ARGSIZE 256

int main()
{
    Display *display;
    display = XOpenDisplay(NULL);
    int count;
    char buf[BSIZE];
    while ((count = fread(buf, 1, BSIZE, stdin)) > 0)
    {
        /* fprintf(stdout, "%c\n", buf[0]); */
        /* fflush(stdout); */
        /* char args[ARGSIZE] = {}; */
        /* char *arg_ptr; */
        /* int arg_index = 0; */
        double x, y;
        switch(buf[0])
        {
            case 'c':
                system("xdotool click 1");
                break;
            case 'm':

                /* strtok(buf, DELEM); */
                /* while (arg_ptr != NULL && arg_index < ARGSIZE) */
                /* { */
                /*     args[arg_index] = arg_ptr; */
                /*     ++arg_index; */
                /*     arg_ptr = strtok(NULL, DELEM); */
                /* } */
                memcpy(&x, buf + 1, sizeof(double));
                memcpy(&y, buf + 9, sizeof(double));
                fprintf(stdout, "%f\n", x);
                fprintf(stdout, "%f\n", y);
                fflush(stdout);
                XWarpPointer(display, None, None, 0, 0, 0, 0, x, y);
                XFlush(display);
                break;
        }
    }
    
    /* Display *display; */
    /* display = XOpenDisplay(NULL); */
    /* int screen = XDefaultScreen(display); */
    /* Window root_window = XRootWindow(display, screen); */
    /* int width = DisplayWidth(display, screen); */
    /* int height = DisplayHeight(display, screen); */
    /* XWarpPointer(display, None, None, 0, 0, 0, 0, 10, 10); */
    /* XFlush(display); */
    return 0;
}
