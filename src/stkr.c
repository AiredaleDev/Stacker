#include <assert.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"
#include "program.h"

#define MAX_TOKEN_NAME_LEN 256

// TODO: Make me parse instructions, not stack values.
StackerInst parse_inst(char* str) {
    assert(str != NULL);

    StackerInst inst = {0};
    if (strcmp(str, "+") == 0) {
        inst.op = OP_PLUS;
    } else if (strcmp(str, ".") == 0) {
        inst.op = OP_PEEK;
    } else if (strcmp(str, "pop") == 0) {
        inst.op = OP_POP;
    } else if (strcmp(str, "dup") == 0) {
        inst.op = OP_DUP;
    } else if (strcmp(str, ".s") == 0) {
        inst.op = OP_DUMP;
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

// Run a horth program. So far just returns whether or not it had a problem,
// not necessarily which one.
bool evaluate(Program* program, Stack* stack) {
    for (int pc = 0; pc < program->len; ++pc) {
        switch(program->instructions[pc].op) {
        case OP_PUSH: {
            horth_stack_push(stack, program->instructions[pc].val);
        } break;
        case OP_DUP: {
            horth_stack_push(stack, stack->data[stack->top - 1]);
        } break;
        case OP_POP: {
            horth_stack_pop(stack);
        } break;
        case OP_PEEK: {
            horth_stack_print_top(stack);
        } break;
        case OP_DUMP: {
            horth_stack_dump(stack);
        } break;
        case OP_PLUS: {
            StackerValue v2 = horth_stack_pop(stack);
            StackerValue v1 = horth_stack_pop(stack);
            if (v1.type == INTEGER && v2.type == INTEGER) {
                horth_stack_push(stack, (StackerValue){ .type = INTEGER, .integer = v1.integer + v2.integer });
            } else {
                // TODO: Tell the user *where* in the file the error is.
                fprintf(stderr, "Type mismatch when adding!\n");
                return false;
            }
        } break;
        default: // Should be unreachable
            return false;
        }
    }
    
    return true;
}

// Eventually we'll feed this badboi things it can compile.
// For now, we'll just enjoy the ease of interpretation.
int main(int argc, char** argv) {    
    // When we move onto compilation, I think I might use LLVM since it's relatively
    // straightforward. I can just slap a `pp` on the end of all my filenames and start
    // leveraging it when the time comes.
            
    // TODO: Read argv to decide whether or not to interpret or compile.
    puts("Welcome to the Horth interpreter.");
    puts("Type '?' to get some help, type 'bye' or ctrl-d to quit.");
    
    Program program;
    horth_program_init(&program);
    
    Stack environment_stack;
    horth_stack_init(&environment_stack);
    assert(environment_stack.top == 0);
    
    while (true) {
        char* line = readline("> ");
        if (line == NULL || strcmp(line, "bye") == 0) {
            break;
        }
        
        // introducing probably *the* worst string tokenizing library ever conceived
        char* token = strtok(line, " ");
        while (token) {
            if (token[0] == '\\') { // ok comment time
                break;
            }
            StackerInst inst = parse_inst(token);
            horth_program_append_instruction(&program, inst);

            token = strtok(NULL, " ");
        }
        
        if (evaluate(&program, &environment_stack)) {
            puts("OK");
        } else {
            puts("Evaluation failure!");
        }
        
        // FIXME: This bad boi frees memory readline still needs?
        // horth_program_clear(&program);

        // This is semantically equivalent (and works!) but is more wasteful of existing allocations.
        horth_program_deinit(&program);
        horth_program_init(&program);
    }
    
    puts("Goodbye!");
}
