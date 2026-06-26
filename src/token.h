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

/* Supported operations */
typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} operation_type;


/* Token struct; serves as a wrapper around number_t and operand_t */
typedef struct {
    token_type type;
    void *obj;
} token_t;

/* Number data type */
typedef uint64_t value_t;
typedef struct {
    value_t value;
    int base;
} number_t;

/* Operand data type */
typedef struct {
    char arity;
    char precedence;
    operation_type op;
} operand_t;

#endif

