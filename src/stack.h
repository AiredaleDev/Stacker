#ifndef _STACK_H
#define _STACK_H

#include "util.h"

#define INITIAL_STACK_CAP 64

typedef enum {
    SYMBOL,
    INTEGER,
} HorthType;

typedef struct { 
    HorthType type;
    union {
        char* text;
        iptr  integer;
    };
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
void horth_stack_dump(HorthStack*);
void horth_stack_print_top(HorthStack*);

#endif
