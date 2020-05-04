#ifndef RECORD
#define RECORD

#include "flags.h"

int start_recording(void (*toggle)());

int stop_recording();

#endif
