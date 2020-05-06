#include "record.h"
#include <stdlib.h>
#include <stdio.h>

void toggle()
{
    printf("Stuff");
    fflush(stdout);
}

int main()
{
    start_recording(toggle);
    return EXIT_SUCCESS;
}
