#include <stdio.h>
#include <stdlib.h>

#include "util.h"

void die(const char* message) {
    fprintf(stderr, "[FATAL] %s\n", message);
    exit(1);
}
