#include <stdio.h>

#include "stkr.h"

void print_eval_error(EvalResult error) {
    // TODO: Tell the user where incorrect code is in the file.
    switch (error) {
    case EVAL_TYPEMISMATCH: { 
        fprintf(stderr, "Type mismatch!\n");
    } break;
    case EVAL_UNKNOWNOP: {
        fprintf(stderr, "Unknown operator!\n");
    } break;
    case EVAL_DIVBYZERO: {
        fprintf(stderr, "Division by zero!\n");
    }
    case EVAL_OK: {}
    }
}


