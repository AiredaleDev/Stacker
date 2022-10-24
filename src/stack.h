#ifndef _STACK_H
#define _STACK_H

#include "util.h"

#define INITIAL_STACK_CAP 64

typedef enum {
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
    usize       top;
    usize       cap;
} Stack;

void horth_stack_init(Stack*);
void horth_stack_deinit(Stack*);
void horth_stack_grow(Stack*);
void horth_stack_push(Stack*, StackerValue);
StackerValue horth_stack_pop(Stack*);
void horth_stack_dump(Stack*);
void horth_stack_print_top(Stack*);

#endif
