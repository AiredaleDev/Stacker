#ifndef _PROGRAM_H
#define _PROGRAM_H

#include "util.h"
#include "stack.h"

// This constant only makes sense in the context
// of interpreting a program live at runtime anyway.
#define INITIAL_PROGRAM_CAP 1024

// Planned for the near future:
// proc, macro (forth can swap between "compile" and "interpret" modes at runtime)
// Ideally I'd like many of these to be written in the language itself down the line.
typedef enum {
    // Stack operations
    OP_PUSH,
    OP_POP,
    OP_DUP,
    OP_PEEK,
    OP_DUMP,
    // Arithmetic
    OP_PLUS,
    OP_MINUS,
    OP_MUL,
    OP_DIV,
    OP_MOD,
    // Comparisons
    OP_EQ,
    // Control flow
    OP_IF,
    OP_ELSE,
    OP_END,
} StackOp;

// This really feels like making the same data structure
// again... I wonder if there's any way I can deduplicate
// some of my code
typedef struct {
    StackOp      op;
    StackerValue val;
} StackerInst;

// A program is just a list of operations.
// Dare I say, a STACK of operations?
// Well, we'll see about that.
typedef struct {
    StackerInst* instructions;
    usize        len;
    usize        cap;
} Program;

void stacker_program_init(Program*);
void stacker_program_deinit(Program*);
void stacker_program_grow(Program*);
void stacker_program_clear(Program*);
void stacker_program_append_instruction(Program*, StackerInst);
void stacker_program_append_instructions(Program*, StackerInst*, usize);

#endif
