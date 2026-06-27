#ifndef TOKEN_H
#define TOKEN_H

#include <stdint.h>

/* Token types */
typedef enum {
  OPERAND,
  NUMBER,
  LPAREN,
  RPAREN
} token_type;

/* Token struct; serves as a wrapper around number_t and operand_t */
typedef struct {
    token_type type;
    void *obj;
} token_t;


/* Supported operations */
typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    NUM_OP
} operation_type;

#define SYNONYMS_PER_OP 3
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

/* Operand data type */
typedef struct {
    char arity;
    char precedence;
    operation_type op;
} operand_t;

/*
* Constructor by type for operand_t objects. Returns a pointer a HEAP-allocated
* operand_t and NULL if malloc fails.
*/
operand_t *init_operand(operation_type type);


/* Number data type */
typedef uint64_t value_t;
typedef struct {
    value_t value;
    int base;
} number_t;


/* 
* Parametrized constructor for number_t objects. Returns a pointer to a HEAP-allocated 
* number_t and NULL if malloc fails.
*/
number_t *init_number(value_t value, int base);


#endif

