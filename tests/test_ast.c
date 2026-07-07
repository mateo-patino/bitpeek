#include "test_ast.h"
#include "test_assert.h"
#include "token.h"
#include "lexer.h"
#include "ast.h"

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


static const test_case_t ast_tests[] = {
    TEST(test_evaluation_simple)
};


int run_ast_tests(int *total_ran, int *crashes) {
    size_t test_count = ARRAY_LEN(ast_tests);
    int pass = run_forked_tests(ast_tests, test_count, crashes);
    if (total_ran) { *total_ran = test_count; }
    return pass;
}

value_t _evaluate_expression(const char *expr) { 
    if (!expr) { return -1; }

    size_t len = strlen(expr);
    char str[len + 1];
    strncpy(str, expr, len + 1);
    str[len] = '\0';

    size_t token_count = count_tokens(expr);
    token_t tokens[token_count];
    tokens_status status;
    if ((status = create_tokens_from_string(str, tokens, NULL)) != TOKENS_OK) {
        free_tokens_invalid(tokens);
        return -1;
    }

    if (validate_tokens_semantic(tokens, token_count) != TOKENS_OK) {
        free_tokens_count(tokens, token_count);
        return -1;
    }
    
    /* Parse the token array to build an Abstract Syntax Tree (AST) */
    AST ast;
    ast.root = create_ast_from_tokens(tokens, token_count); 
    value_t out = evaluate_ast(&ast);

    free_tokens_count(tokens, token_count);
    return out;
}


bool test_evaluation_simple(void) {
    ASSERT_EXPR("1 + 1", (value_t)2);
    ASSERT_EXPR("10 - 1 ", (value_t)9);
    ASSERT_EXPR("100 / 20", (value_t)5);
    ASSERT_EXPR("2 * 2", (value_t)4);
    ASSERT_EXPR("0xff", (value_t)255);
    ASSERT_EXPR("0b10", (value_t)2);
    ASSERT_EXPR("010", (value_t)8);
    ASSERT_EXPR("00", (value_t)0);

    return true;
}
