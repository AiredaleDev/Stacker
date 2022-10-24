#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "program.h"

// Maybe we should just allocate however much room we need instead of this size-doubling stuff
void horth_program_init(Program* program) {
    program->len = 0;
    program->cap = INITIAL_PROGRAM_CAP;
    program->instructions = calloc(program->cap, sizeof(StackOp));
    if (program->instructions == NULL) {
        fprintf(stderr, "Failed to allocate space for program!\n");
        exit(1);
    }
}

void horth_program_deinit(Program* program) {
    // We don't assume you necessarily allocated the program in question using malloc.
    if (program) {
        if (program->instructions) {
            free(program->instructions);
        }
    }
}

// I think what I want is a bump allocator and not malloc.
void horth_program_clear(Program* program) {
    program->len = 0;
    // FIXME: This fucker frees memory that readline uses??? 
    memset(program->instructions, 0, program->cap * sizeof(StackerInst));
}

void horth_program_append_instruction(Program* program, StackerInst inst) {
    if (program->len == program->cap) {
        horth_program_grow(program);
    }
    ++program->len;
    program->instructions[program->len - 1] = inst;
}

// Generally speaking we're going to opt to use this one in the interpreter.
// When I get to compiling stuff, I (theoretically) am going to be able to inspect the number of instructions
// in the file and allocate room for exactly that many. If I need more room for macros/runtime-fixing, then I will just grow accordingly.
void horth_program_append_instructions(Program* program, StackerInst* insts, usize nemb) {
    // This bad boi is O(1) amortized. You'd have to feed this guy a massive program in the interpreter.
    // in order to run 
    while (program->cap < program->len + nemb) {
        horth_program_grow(program); // Arguably we should just allocate all the size upfront.
    }
    
    for (usize i = 0; i < nemb; ++i) {
        horth_program_append_instruction(program, insts[i]);
    }
}

void horth_program_grow(Program* program) {
    program->cap *= 2;
    program->instructions = reallocarray(program->instructions, program->cap, sizeof(StackerInst));
}
