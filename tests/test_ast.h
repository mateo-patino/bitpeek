#ifndef TEST_AST_H
#define TEST_AST_H

#include "token.h"

#include <stdbool.h>


/* 
* Runner for the AST tests. 
*/
int run_ast_tests(int *total_ran, int *crashes);


/*
* Calls tokenizer and evaluator on a mathematical expression, simulating the entire pipeline
* of pcalc.
*
* Returns the result of evaluating the expression and -1 if an error occurs.
*/
value_t _evaluate_expression(const char *expr);


/*
* Integration test: tests the tokenization, semantic validation, and AST building and evaluation
* pipeline on algebraic expressions.
*
* See _evaluate_expression to see the pipeline tested.
*/
bool test_evaluation_simple(void);
bool test_evaluation_medium(void);
bool test_evaluation_hard(void);
bool test_evaluation_harder(void);


/*
* Test parenthesis handling
*/
bool test_evaluation_parens_madness(void);

#endif
