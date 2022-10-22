#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"

// Panic on allocation failure already? That's not suitable for kernel development or realtime applications!
// I'll figure out a way around this somehow.

void horth_stack_init(HorthStack* stack) {    
    stack->top = 0;
    stack->cap = INITIAL_STACK_CAP;
    stack->data = calloc(stack->cap, sizeof(HorthValue));
    if (stack->data == NULL) {
        fprintf(stderr, "Failed to allocate room for stack's underlying array!\n");
        exit(1);
    }    
}

// for subroutines? do they individually manage their own stacks?
void horth_stack_deinit(HorthStack* stack) {
    if (stack != NULL) {
        if (stack->data != NULL) {
            free(stack->data);
        }
        free(stack);
    }
}

void horth_stack_grow(HorthStack* stack) {
    assert(stack != NULL);
    
    stack->cap *= 2;
    stack->data = realloc(stack->data, stack->cap);
    // TODO: read realloc's docs to see if this is how it handles allocation failure
    if (stack->data == NULL) {
        fprintf(stderr, "Failed to expand stack!\n");
        exit(1);
    }
}

void horth_stack_push(HorthStack* stack, HorthValue value) {
    assert(stack != NULL);
    
    if (stack->top == stack->cap) {
        horth_stack_grow(stack);
    }
    
    stack->data[stack->top++] = value;
}

HorthValue horth_stack_pop(HorthStack* stack) {
    assert(stack != NULL);    
    // uh yeah moron if you just blindly pop
    // past the bottom of the stack you'll segfault
    return stack->data[stack->top--];
}


void horth_stack_print(HorthStack* stack) {
    printf("[Size: %zu]", stack->top);
    for (int i = 0; i <= stack->top; ++i) {
        if (stack->data[i].type == SYMBOL) {
            printf(" %s", stack->data[i].text);
        }
        if (stack->data[i].type == INTEGER) {
            printf(" %ld", stack->data[i].integer);
        }
    }
    puts("");
}
