#include "token.h"

#include <stdlib.h>
#include <stdbool.h>


const char* const operation_labels[NUM_OP][SYNONYMS_PER_OP] = {
    [ADD] = {"add", "plus", "+"},
    [SUB] = {"sub", "minus", "-"},
    [MUL] = {"mul", "times", "x"},
    [DIV] = {"div", "over", "/"}
};


const char op_arity[NUM_OP] = {
    [ADD] = 2,
    [SUB] = 2,
    [MUL] = 2,
    [DIV] = 2
};


const char op_precedence[NUM_OP] = {
    [ADD] = 0,
    [SUB] = 0,
    [MUL] = 1,
    [DIV] = 1
};


number_t *init_number(value_t value, int base) {
    number_t *obj = malloc(sizeof(number_t));
    if (!obj) {
        return NULL;
    } 
    obj->value = value;
    obj->base = base;
    
    return obj;
}


operand_t *init_operand(operation_type type) {
    operand_t *op = malloc(sizeof(operand_t));
    if (!op) {
        return NULL;
    }
    op->op = type;
    op->arity = op_arity[type];
    op->precedence = op_precedence[type];

    return op;
}


bool free_token_obj(token_t *tok) {
    if (!tok) {
        return false;
    }
    if (tok->obj) {
        free(tok->obj);
        tok->obj = NULL;
        return true;
    }
    return false;
}


int free_tokens_invalid(token_t *tok) {
    if (!tok) {
        return 0;
    }
    int count = 0;
    token_t *ptr = tok;
    while (ptr->type != INVALID_TOKEN) {
        free_token_obj(ptr);
        count++;
        ptr++;
    }
    return count;
}
