#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "util.h"
#include "stack.h"

// This constant only makes sense in the context
// of interpreting a program live at runtime anyway.
#define INITIAL_PROGRAM_CAP 1024

// Planned for the near future:
// minus, times, div, mod, if, proc...
typedef enum {
    OP_PUSH,
    OP_POP,
    OP_DUP,
    OP_PEEK,
    OP_DUMP,
    OP_PLUS,
    OP_MINUS,
    OP_IF,
    OP_END,
} StackOp;

// This really feels like making the same data structure
// again... I wonder if there's any way I can deduplicate
// some of my code
typedef struct {
    StackOp    op;
    StackerValue val; // Optional. Will only be used for push... A bit wasteful to have this commonly used data structure
    // fill my cache line with a bunch of zeroes...
} StackerInst;

// A program is just a list of operations.
// Dare I say, a STACK of operations?
// Well, we'll see about that.
typedef struct {
    StackerInst* instructions;
    usize      len;
    usize      cap;
} Program;

void stacker_program_init(Program*);
void stacker_program_deinit(Program*);
void stacker_program_grow(Program*);
void stacker_program_clear(Program*);
void stacker_program_append_instruction(Program*, StackerInst);
void stacker_program_append_instructions(Program*, StackerInst*, usize);

#endif
