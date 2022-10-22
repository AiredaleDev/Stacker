#include <assert.h>
#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

#define MAX_TOKEN_NAME_LEN 256

HorthValue parse_into_token(char* str) {
    assert(str != NULL);

    HorthValue val = {0};
    int v = atoi(str);
    // FIXME: Need to properly check that parsing failed.
    // FIXME: we also just segfault if we don't prepare the stack properly before calling something
    // that's pretty cringe
    if (v == 0 && str[0] != '0') {
        // Parser failure => Fuck it, it's a symbol name.
        // Also not very robust, but I just want to see that I can get things evaluating.
        val.type = SYMBOL;
        val.text = strndup(str, MAX_TOKEN_NAME_LEN);
    } else {
        val.type = INTEGER;
        val.integer = v; // On small word systems we're truncating without asking. Not great for correctness!
    }
    
    return val;
}

bool evaluate(HorthStack* stack) {
    for (int head = 0; head < stack->top; ++head) {
        HorthValue val = stack->data[head];
        
        switch (val.type) {
        case INTEGER: {
            // Not much we can do here chief.
        } break;
        case SYMBOL: {
            // TODO: variables and procedures
            if (strcmp(val.text, "+") == 0) {
                horth_stack_pop(stack); // Pop operation
                // fuck we're not typechecking these
                // I'm starting to see why raw unions aren't really that
                // nice to work with.
                int a2 = horth_stack_pop(stack).integer;
                int a1 = horth_stack_pop(stack).integer;
                HorthValue result;
                result.type = INTEGER;
                result.integer = a1 + a2;

                // algowise this should work as intended...
                // seems like it's a stack impl problem
                horth_stack_push(stack, result);
            }
            if (strcmp(val.text, ".s") == 0) {
                horth_stack_pop(stack);
                horth_stack_print(stack);
            }
        } break;
        // Should be _unreachable_
        default:
            puts("Failed to match a type!");
            return false;
        }
    }
    
    return true;
}

// Eventually we'll feed this badboi things it can compile.
// For now, we'll just enjoy the ease of interpretation.
int main(int argc, char** argv) {
    // Forth is a stack-based language that embraces reverse polish notation.
    // To call a procedure, you push your arguments onto the stack and then do the operation,
    // much like assembly language.
    
    // To compute 6 + 9, you type the following:
    // 6 9 + ; This pushes 6 onto the stack, 9 onto the stack, and then
    // Stack becomes `15`
    // In interpreted mode, we always print the top of the stack after a function call.
    // Interpreted mode also allows you to dump the stack into stdout. (`.s` in actual Forth.)
    // You can also dynamically change Forth programs while they are running!
    
    // When we move onto compilation, I think I might use LLVM since it's relatively
    // straightforward. I can just slap a `pp` on the end of all my filenames and start
    // leveraging it when the time comes.
    
    // Rework: Instead of having the stack BE the program, we should instead have the program
    // tell the stack what to do.
    
    // In other words, we're going to split this guy up
    
    // TODO: Read argv to decide whether or not to interpret or compile.
    puts("Welcome to the Horth interpreter.");
    puts("Type '?' to get some help, type 'bye' or ctrl-d to quit.");
    
    HorthStack environment_stack;
    horth_stack_init(&environment_stack);
    assert(environment_stack.top == 0);
    
    while (true) {
        char* line = readline("> ");
        if (line == NULL) {
            break;
        }
        
        // introducing probably *the* worst string tokenizing library ever conceived
        char* token = strtok(line, " ");
        while (token) {
            // we parse as we tokenize. I don't think this will impact my modularity goals
            // too much, but in case it does I can very easily decouple these two things.
            HorthValue value = parse_into_token(token);
            horth_stack_push(&environment_stack, value);            
            
            token = strtok(NULL, " ");
            if (token[0] == '\\') { // ok comment time
                break;
            }
        }
        
        // ok now we evaluate
        // man wtf how do we evaluate?
        // I thought this would be super straightforward.
        if (evaluate(&environment_stack)) {
            puts("OK");
            horth_stack_print(&environment_stack);
        } else {
            puts("Evaluation failure!");
            horth_stack_print(&environment_stack);
        }
    }
    
    puts("Goodbye!");
}
