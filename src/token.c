#include "token.h"

#include <stdlib.h>


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
