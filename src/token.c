#include "token.h"

#include <stdlib.h>
#include <stdbool.h>


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
