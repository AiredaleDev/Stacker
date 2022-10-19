#include <readline/readline.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

HorthValue parse_into_token(char* str) {
    HorthValue val;
    int v = atoi(str);
    // FIXME: Need to properly check that parsing failed.
    if (v == 0 && str[0] != '0') {
        // Parser failure => Fuck it, it's a symbol name.
        // Also not very robust, but I just want to see that I can get things evaluating.
    } else {
        val.integer = v; // On small word systems we're truncating without asking. Not great for correctness!
    }
    
    return val;
}

// Eventually we'll feed this badboi things it can compile.
// For now, we'll just enjoy the ease of interpretation.
int main(int argc, char** argv) {
    // Forth is a stack-based language that embraces reverse polish notation.
    // To call an argument, you push your arguments onto the stack and then do the operation,
    // much like assembly language.
    
    // To compute 6 + 9, you type the following:
    // 6 9 + ; This pushes 6 onto the stack, 9 onto the stack, and then
    // Stack becomes `15`
    // In interpreted mode, we always print the top of the stack after a function call.
    // Interpreted mode also allows you to dump the stack into stdout.
    // You can also dynamically change Forth programs while they are running!
    
    // When we move onto compilation, I think I might use LLVM since it's relatively
    // straightforward. I can just slap a `pp` on the end of all my filenames and start
    // leveraging it when the time comes.
    
    // TODO: Read argv to decide whether or not to interpret or compile.
    puts("Welcome to the Horth interpreter.");
    puts("Type '?' to get some help, type ':q' or ctrl-d to quit.");
    
    HorthStack environment_stack;
    horth_stack_init(&environment_stack);
    
    while (true) {
        char* line = readline("> ");
        if (line == NULL) {
            break;
        }
        
        // introducing probably *the* worst string tokenizing library ever conceived
        char* token = strtok(line, " ");
        while (token) {
            HorthValue value;
            // we parse as we tokenize. I don't think this will impact my modularity goals
            // too much, but in case it does I can very easily decouple these two things.

            
            horth_stack_push(&environment_stack, value);
        }
        
        printf("%s\n", line);
    }
    
    puts("Goodbye!");
}
