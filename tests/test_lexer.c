#include "test_lexer.h"
#include "test_assert.h"
#include "token.h"
#include "lexer.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>


static const test_case_t lexer_tests[] = {
    TEST(test_valid_arithmetic_op_tokenizer),
    TEST(test_invalid_arithmetic_op_tokenizer),
    TEST(test_valid_decimal_tokenizer),
    TEST(test_invalid_decimal_tokenizer),
    TEST(test_valid_hexadecimal_tokenizer),
    TEST(test_invalid_hexadecimal_tokenizer),
    TEST(test_valid_binary_tokenizer),
    TEST(test_invalid_binary_tokenizer),
    TEST(test_valid_octal_tokenizer),
    TEST(test_invalid_octal_tokenizer),
    TEST(test_div_by_zero_validator),
    TEST(test_parens_validator)
};


/* Runner for the entire module */
int run_lexer_tests(int *total_ran, int *crashes) {
    size_t test_count = ARRAY_LEN(lexer_tests);
    int pass = run_forked_tests(lexer_tests, test_count, crashes);
    if (total_ran) { *total_ran = test_count; }
    return pass;
}


bool test_valid_arithmetic_op_tokenizer(void) {
    operation_type op;

    ASSERT_TRUE(is_operation("add", &op));
    ASSERT_TRUE(op == ADD);
    ASSERT_TRUE(is_operation("plus", &op));
    ASSERT_TRUE(op == ADD);

    ASSERT_TRUE(is_operation("sub", &op));
    ASSERT_TRUE(op == SUB);
    ASSERT_TRUE(is_operation("minus", &op));
    ASSERT_TRUE(op == SUB);

    ASSERT_TRUE(is_operation("mul", &op));
    ASSERT_TRUE(op == MUL);
    ASSERT_TRUE(is_operation("times", &op));
    ASSERT_TRUE(op == MUL);
    
    ASSERT_TRUE(is_operation("div", &op));
    ASSERT_TRUE(op == DIV);
    ASSERT_TRUE(is_operation("over", &op));
    ASSERT_TRUE(op == DIV);

    return true;
}


bool test_invalid_arithmetic_op_tokenizer(void) {
    ASSERT_TRUE(!is_operation("", NULL));
    ASSERT_TRUE(!is_operation(" ", NULL));
    ASSERT_TRUE(!is_operation("\n", NULL));
    ASSERT_TRUE(!is_operation("\\", NULL));
    ASSERT_TRUE(!is_operation("dog", NULL));
    ASSERT_TRUE(!is_operation("0x10", NULL));
    ASSERT_TRUE(!is_operation("adds", NULL));
    ASSERT_TRUE(!is_operation(NULL, NULL));

    return true;
}


bool test_valid_decimal_tokenizer(void) {
     int base;
     value_t val;

     ASSERT_TRUE(is_number("10", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);
     
     ASSERT_TRUE(is_number("  10  ", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);

     ASSERT_TRUE(is_number("123456789", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     ASSERT_TRUE(is_number("0", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)0, val);

     ASSERT_TRUE(is_number("1", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)1, val);

     errno = 0;
     ASSERT_TRUE(is_number("18446744073709551615", &base, &val));
     ASSERT_TRUE(errno == 0);
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)18446744073709551615ULL, val);
 
     return true;
}


bool test_invalid_decimal_tokenizer(void) {
    ASSERT_TRUE(!is_number("-10", NULL, NULL));
    ASSERT_TRUE(!is_number("10-", NULL, NULL));
    ASSERT_TRUE(!is_number("10 -", NULL, NULL));
    ASSERT_TRUE(!is_number("string", NULL, NULL));
    ASSERT_TRUE(!is_number("", NULL, NULL));
    ASSERT_TRUE(!is_number("-123456789", NULL, NULL));
    ASSERT_TRUE(!is_number("–123456789", NULL, NULL));
    ASSERT_TRUE(!is_number("—123456789", NULL, NULL));
    ASSERT_TRUE(!is_number("18446744073709551616", NULL, NULL));
    ASSERT_TRUE(errno == ERANGE);
    ASSERT_TRUE(!is_number("-18446744073709551614", NULL, NULL));
    ASSERT_TRUE(!is_number(NULL, NULL, NULL));

    return true;
}


bool test_valid_hexadecimal_tokenizer(void) {
     int base;
     value_t val;

     ASSERT_TRUE(is_number("0x10", &base, &val));
     ASSERT_EQ_INT(16, base);
     ASSERT_EQ_VALUE_T((value_t)0x10, val);
     
     ASSERT_TRUE(is_number("  0x10  ", &base, &val));
     ASSERT_EQ_INT(16, base);
     ASSERT_EQ_VALUE_T((value_t)0x10, val);

     ASSERT_TRUE(is_number("0x75BCD15", &base, &val));
     ASSERT_EQ_INT(16, base);
     ASSERT_EQ_VALUE_T((value_t)0x75BCD15, val);

     ASSERT_TRUE(is_number("0x0", &base, &val));
     ASSERT_EQ_INT(16, base);
     ASSERT_EQ_VALUE_T((value_t)0, val);

     ASSERT_TRUE(is_number("0x1", &base, &val));
     ASSERT_EQ_INT(16, base);
     ASSERT_EQ_VALUE_T((value_t)1, val);

     errno = 0;
     ASSERT_TRUE(is_number("0xFFFFFFFFFFFFFFFF", &base, &val));
     ASSERT_TRUE(errno == 0);
     ASSERT_EQ_INT(16, base);
     ASSERT_EQ_VALUE_T((value_t)0xFFFFFFFFFFFFFFFFULL, val);
 
     return true;
}


bool test_invalid_hexadecimal_tokenizer(void) {
    ASSERT_TRUE(!is_number("-0x10", NULL, NULL));
    ASSERT_TRUE(!is_number("0x10-", NULL, NULL));
    ASSERT_TRUE(!is_number("0x10 -", NULL, NULL));
    ASSERT_TRUE(!is_number("string", NULL, NULL));
    ASSERT_TRUE(!is_number("", NULL, NULL));
    ASSERT_TRUE(!is_number("-0x75BCD15", NULL, NULL));
    ASSERT_TRUE(!is_number("–0x75BCD15", NULL, NULL));
    ASSERT_TRUE(!is_number("—0x75BCD15", NULL, NULL));
    ASSERT_TRUE(!is_number("0x10000000000000000", NULL, NULL));
    ASSERT_TRUE(errno == ERANGE);
    ASSERT_TRUE(!is_number("-0xFFFFFFFFFFFFFFFE", NULL, NULL));
    ASSERT_TRUE(!is_number(NULL, NULL, NULL));
    ASSERT_TRUE(!is_number("0xZZ", NULL, NULL));

    return true;
}


bool test_valid_binary_tokenizer(void) {
     int base;
     value_t val;

     ASSERT_TRUE(is_number("0b1010", &base, &val));
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);
     
     ASSERT_TRUE(is_number("  0b1010  ", &base, &val));
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);

     ASSERT_TRUE(is_number("0b111010110111100110100010101", &base, &val));
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     ASSERT_TRUE(is_number("0b0", &base, &val));
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)0, val);

     ASSERT_TRUE(is_number("0b1", &base, &val));
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)1, val);

     ASSERT_TRUE(is_number("0b111010110111100110100010101", &base, &val));
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     errno = 0;
     ASSERT_TRUE(is_number("0b1111111111111111111111111111111111111111111111111111111111111111", &base, &val));
     ASSERT_TRUE(errno == 0);
     ASSERT_EQ_INT(2, base);
     ASSERT_EQ_VALUE_T((value_t)18446744073709551615ULL, val);
 
     return true;
}


bool test_invalid_binary_tokenizer(void) {
    ASSERT_TRUE(!is_number("-0b1010", NULL, NULL));
    ASSERT_TRUE(!is_number("0b1010-", NULL, NULL));
    ASSERT_TRUE(!is_number("0b1010 -", NULL, NULL));
    ASSERT_TRUE(!is_number("string", NULL, NULL));
    ASSERT_TRUE(!is_number("", NULL, NULL));
    ASSERT_TRUE(!is_number("-0b111010110111100110100010101", NULL, NULL));
    ASSERT_TRUE(!is_number("–0b111010110111100110100010101", NULL, NULL));
    ASSERT_TRUE(!is_number("—0b111010110111100110100010101", NULL, NULL));
    ASSERT_TRUE(!is_number("0b10000000000000000000000000000000000000000000000000000000000000000", NULL, NULL));
    ASSERT_TRUE(errno == ERANGE);
    ASSERT_TRUE(!is_number("-0b1111111111111111111111111111111111111111111111111111111111111110", NULL, NULL));
    ASSERT_TRUE(!is_number(NULL, NULL, NULL));
    ASSERT_TRUE(!is_number("0b33", NULL, NULL));

    return true;
}


bool test_valid_octal_tokenizer(void) {
     int base;
     value_t val;

     ASSERT_TRUE(is_number("012", &base, &val));
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);
     
     ASSERT_TRUE(is_number("  012  ", &base, &val));
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)10, val);

     ASSERT_TRUE(is_number("0726746425", &base, &val));
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     ASSERT_TRUE(is_number("00", &base, &val));
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)0, val);

     ASSERT_TRUE(is_number("01", &base, &val));
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)1, val);

     ASSERT_TRUE(is_number("0726746425", &base, &val));
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     errno = 0;
     ASSERT_TRUE(is_number("01777777777777777777777", &base, &val));
     ASSERT_TRUE(errno == 0);
     ASSERT_EQ_INT(8, base);
     ASSERT_EQ_VALUE_T((value_t)18446744073709551615ULL, val);
 
     return true;
}

bool test_invalid_octal_tokenizer(void) {
    ASSERT_TRUE(!is_number("-012", NULL, NULL));
    ASSERT_TRUE(!is_number("012-", NULL, NULL));
    ASSERT_TRUE(!is_number("012 -", NULL, NULL));
    ASSERT_TRUE(!is_number("string", NULL, NULL));
    ASSERT_TRUE(!is_number("", NULL, NULL));
    ASSERT_TRUE(!is_number("-0726746425", NULL, NULL));
    ASSERT_TRUE(!is_number("02000000000000000000000", NULL, NULL));
    ASSERT_TRUE(errno == ERANGE);
    ASSERT_TRUE(!is_number("-01777777777777777777776", NULL, NULL));
    ASSERT_TRUE(!is_number("–0726746425", NULL, NULL));
    ASSERT_TRUE(!is_number("—0726746425", NULL, NULL));
    ASSERT_TRUE(!is_number(NULL, NULL, NULL));
    ASSERT_TRUE(!is_number("08", NULL, NULL));
    ASSERT_TRUE(!is_number("09", NULL, NULL));

    return true;
}

#define dbz_expr_count 8
static char *div_by_zero_expressions[] = {
    "10 / 0",
    "0xff DIV 00",
    "1 div 0",
    "1 / 0b0",
    "0 / 0x0",
    "01 / 0000000",
    "0xff + 255 - 0 / 0",
    "0 over 0"
};


bool test_div_by_zero_validator(void) {
    for (int i = 0; i < dbz_expr_count; i++) {
        size_t len = strlen(div_by_zero_expressions[i]);
        char strbuf[len+1];
        strncpy(strbuf, div_by_zero_expressions[i], len + 1);

        size_t token_count = count_tokens(div_by_zero_expressions[i]);
        token_t tokens[token_count];
        create_tokens_from_string(strbuf, tokens, NULL);

        ASSERT_TRUE(validate_div_by_zero(tokens, token_count) == TOKENS_DIV_BY_ZERO);
    }
    return true;
}


bool test_parens_validator(void ){
    return true;
}
