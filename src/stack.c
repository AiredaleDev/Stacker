#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "stack.h"
#define DEBUG 0

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
    stack->data = reallocarray(stack->data, stack->cap, sizeof(HorthValue));
    // TODO: read realloc's docs to see if this is how it handles allocation failure
    if (stack->data == NULL) {
        fprintf(stderr, "Failed to expand stack!\n");
        exit(1);
    }
}

#if DEBUG
static void horth_value_print(HorthValue value) {
    switch (value.type) {
    case INTEGER:
        fprintf(stderr, "[INFO] Value is %zu\n", value.integer);
    case SYMBOL:
        fprintf(stderr, "[INFO] Value is %s\n", value.text);
    default:
        fprintf(stderr, "[ERROR] Received value with meaningless type! TypeID = %d\n", value.type);
    }
}
#endif

void horth_stack_push(HorthStack* stack, HorthValue value) {
    assert(stack != NULL);
    
    if (stack->top == stack->cap) {
        horth_stack_grow(stack);
    }
    
#if DEBUG
    horth_value_print(value);
#endif

    stack->data[stack->top++] = value;
}

HorthValue horth_stack_pop(HorthStack* stack) {
    assert(stack != NULL);    
    // uh yeah moron if you just blindly pop
    // past the bottom of the stack you'll segfault
    return stack->data[--stack->top];
}

static void horth_stack_print_val(HorthStack* stack, usize i) {
    assert(stack != NULL);
    
    if (stack->data[i].type == SYMBOL) {
        printf(" %s", stack->data[i].text);
    }
    if (stack->data[i].type == INTEGER) {
        printf(" %ld", stack->data[i].integer);
    }
}

void horth_stack_dump(HorthStack* stack) {
    assert(stack != NULL);

    printf("[Size: %zu]", stack->top);
    for (int i = 0; i < stack->top; ++i) {
        horth_stack_print_val(stack, i);
    }
    puts("");
}

void horth_stack_print_top(HorthStack* stack) {
    horth_stack_print_val(stack, stack->top - 1);
    puts("");
}

