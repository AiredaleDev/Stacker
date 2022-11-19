#ifndef _STACK_H
#define _STACK_H

#include "util.h"

#define INITIAL_STACK_CAP 64

typedef enum {
    NIL,
    SYMBOL,
    INTEGER,
} StackerType;

typedef struct { 
    StackerType type;
    union {
        char* text;
        iptr  integer;
    };
} StackerValue;

typedef struct {
    StackerValue* data;
    usize         top;
    usize         cap;
} Stack;

void stacker_stack_init(Stack*);
void stacker_stack_deinit(Stack*);
void stacker_stack_grow(Stack*);
void stacker_stack_push(Stack*, StackerValue);
StackerValue stacker_stack_pop(Stack*);
void stacker_stack_dump(Stack*);
void stacker_stack_print_top(Stack*);

#define NIL_VALUE (StackerValue){0}

#endif
