#ifndef _STACK_H
#define _STACK_H

#include "util.h"

#define INITIAL_STACK_CAP 64

// Maybe we store a stack of "symbols"
// Their meanings can be inferred at runtime?
// We'll ignore text symbols for now and hardcode our arithmetic operations.
typedef union {
    char* text;
    isize integer;
} HorthValue;

typedef struct {
    HorthValue* data;
    usize       top;
    usize       cap;
} HorthStack;

void horth_stack_init(HorthStack*);
void horth_stack_deinit(HorthStack*);
void horth_stack_grow(HorthStack*);
void horth_stack_push(HorthStack*, HorthValue);
HorthValue horth_stack_pop(HorthStack*);

#endif
