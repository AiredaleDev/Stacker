#include <assert.h>
#include <errno.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "program.h"
#include "stkr.h"

#define MAX_TOKEN_NAME_LEN 256

// I kinda feel like this could be tidier.
// This isn't "elegant" but it gets the job done.
static StackerInst parse_inst(char* str) {
    assert(str != NULL);

    StackerInst inst = {0};
    // Replacing this with a hash table makes no difference in terms of performance,
    // but it will clean this up.
    if (strcmp(str, "+") == 0) {
        inst.op = OP_PLUS;
    } else if (strcmp(str, "-") == 0) {
        inst.op = OP_MINUS;
    } else if (strcmp(str, "*") == 0) {
        inst.op = OP_MUL;
    } else if (strcmp(str, "/") == 0) {
        inst.op = OP_DIV;
    } else if (strcmp(str, "%") == 0) {
        inst.op = OP_MOD;
    } else if (strcmp(str, ".") == 0) {
        inst.op = OP_PEEK;
    } else if (strcmp(str, "pop") == 0) {
        inst.op = OP_POP;
    } else if (strcmp(str, "dup") == 0) {
        inst.op = OP_DUP;
    } else if (strcmp(str, ".s") == 0) {
        inst.op = OP_DUMP;
    } else if (strcmp(str, "=") == 0) {
        inst.op = OP_EQ;
    } else if (strcmp(str, "if") == 0) {
        inst.op = OP_IF;
    } else if (strcmp(str, "else") == 0) {
        inst.op = OP_ELSE;
    } else if (strcmp(str, "end") == 0) {
        inst.op = OP_END;
    } else { // any old thing: push value onto stack
        StackerValue val = {0};
        int v = atoi(str);
        // FIXME: Need to properly check that parsing failed.
        // Parser failure => it's a symbol name.
        if (v == 0 && str[0] != '0') {
            val.type = SYMBOL;
            val.text = strndup(str, MAX_TOKEN_NAME_LEN);
        } else {
            val.type = INTEGER;
            val.integer = v; // On small word systems we're truncating without asking. Not great for correctness!
        }    
        
        inst.op = OP_PUSH;
        inst.val = val;
    }    
    return inst;
}

// Run a Stacker program. So far just returns whether or not it had a problem,
// not necessarily which one.
// Might cut this guy up into smaller pieces
static EvalResult evaluate(Program* program, Stack* stack) {    
    for (int pc = 0; pc < program->len; ++pc) {
        switch(program->instructions[pc].op) {
        case OP_PUSH: {
            stacker_stack_push(stack, program->instructions[pc].val);
        } break;
        case OP_DUP: {
            stacker_stack_push(stack, stack->data[stack->top - 1]);
        } break;
        case OP_POP: {
            stacker_stack_pop(stack);
        } break;
        case OP_PEEK: {
            stacker_stack_print_top(stack);
        } break;
        case OP_DUMP: {
            stacker_stack_dump(stack);
        } break;
        case OP_PLUS: {
            EVAL_ARITHMETIC(+)
        } break;
        case OP_MINUS: {
            EVAL_ARITHMETIC(-)
        } break;
        case OP_MUL: {
            EVAL_ARITHMETIC(*)
        } break;
        case OP_DIV: {
            EVAL_ARITHMETIC(/)
        } break;
        case OP_MOD: {
            EVAL_ARITHMETIC(%)
        } break;
        case OP_EQ: {
            EVAL_ARITHMETIC(==)
        } break;
        case OP_IF: {
            StackerValue cond = stacker_stack_pop(stack);
                
            int lookahead = pc;
            int else_loc = -1;
            while (program->instructions[lookahead].op != OP_END) {
                if (lookahead + 1 >= program->len) {
                    die("Encountered `if` without matching `end`");
                }

                // This currently doesn't quite work as expected for chaining if blocks
                if (program->instructions[lookahead].op == OP_ELSE) {
                    else_loc = lookahead;
                }

                ++lookahead;
            }

            // We get truthy/falsy values for free!
            if (!cond.integer) {
                if (else_loc > -1) {
                    pc = else_loc;
                } else {
                    pc = lookahead;
                }
            }
        } break;
        case OP_ELSE: {
            // Also don't really do anything. This is a target to jump to not a meaningful instruction
        } break;
        case OP_END: {
            // skip
        } break;
        default: // Should be unreachable
            return EVAL_UNKNOWNOP;
        }
    }
    
    return EVAL_OK;
}

void repl(void) {
    // When we move onto compilation, I think I might use LLVM since it's relatively
    // straightforward. I can just slap a `pp` on the end of all my filenames and start
    // leveraging it when the time comes.
            
    // TODO: Read argv to decide whether or not to interpret or compile.
    puts("Welcome to the Horth interpreter.");
    puts("Type '?' to get some help, type 'bye' or ctrl-d to quit.");
    
    Program program;
    stacker_program_init(&program);
    
    Stack environment_stack;
    stacker_stack_init(&environment_stack);
    assert(environment_stack.top == 0);
    
    while (true) {
        char* line = readline("> ");
        if (line == NULL || strcmp(line, "bye") == 0) {
            break;
        }
        
        // introducing probably *the* worst string tokenizing library ever conceived
        // surprisingly working okay for my current purposes,
        char* token = strtok(line, " ");
        while (token) {
            if (token[0] == '\\') { // ok comment time
                break;
            }
            StackerInst inst = parse_inst(token);
            stacker_program_append_instruction(&program, inst);

            token = strtok(NULL, " ");
        }
        
        EvalResult eval_result = evaluate(&program, &environment_stack);
        if (eval_result == EVAL_OK) {
            puts("OK");
        } else {
            print_eval_error(eval_result);
        }
        
        // FIXME: This bad boi frees memory readline still needs?
        // Maybe I'm overshooting with my memset, but the man pages suggest otherwise...
        // stacker_program_clear(&program);
        
        // This is semantically equivalent (and works!) but is more wasteful of existing allocations.
        stacker_program_deinit(&program);
        stacker_program_init(&program);
    }
    
    puts("Goodbye!");
}

// Is there a theoretically better way to do this?
// One that does not assume that source files are under some arbitrary size?
// File-reading is the one thing where we have no idea how large it will be.
#define PROGRAM_BUF_SIZE 8192

#if 0
// For now, this will read our file into a valid program.
Program* read_stacker_source(const char* filepath) {
    FILE* stacker_source = fopen(filepath, "r");
    // TODO: determine correct errno
    char contents[PROGRAM_BUF_SIZE];
    usize char_count = fread(contents, sizeof(char), PROGRAM_BUF_SIZE, stacker_source);
    
    // Want to tokenize by two different things at the same time? TOO BAD!
    char* line = strtok(contents, "\n");
    char* token = strtok(line, " ");
    
    fclose(stacker_source);
    return NULL;
}
#endif

// Eventually we'll feed this badboi things it can compile.
// For now, we'll just enjoy the ease of interpretation.
int main(const int argc, const char** argv) {
    if (argc < 2) {
        repl();
    }
    
    // ok we've been given an argument, is it a file we can read?
    // read_stacker_source(argv[1]);
}

