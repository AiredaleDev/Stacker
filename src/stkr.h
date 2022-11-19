#ifndef _STKR_H
#define _STKR_H

typedef enum {
    EVAL_OK,
    EVAL_TYPEMISMATCH,
    EVAL_DIVBYZERO,
    EVAL_UNKNOWNOP,
} EvalResult;

void print_eval_error(EvalResult);

#define EVAL_ARITHMETIC(op) \
StackerValue v2 = stacker_stack_pop(stack);\
StackerValue v1 = stacker_stack_pop(stack);\
if (v1.type == INTEGER && v2.type == INTEGER) {\
    stacker_stack_push(\
        stack,\
        (StackerValue){\
            .type = INTEGER,\
            .integer = v1.integer op v2.integer\
        }\
    );\
} else {\
    return EVAL_TYPEMISMATCH;\
}\

#endif
