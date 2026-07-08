#include <stdbool.h>

#include "lexer.h"

/*
* Calls all of the unit and integration tests for testing the lexer. 
*
* Returns the number of tests passed, and if 'total_ran' is not NULL, it writes the total
* number of tests ran there. If 'crashes' is not NULL, it writes the number of crashes 
* (i.e tests terminated by a POSIX signal) there.
*/
int run_lexer_tests(int *total_ran, int *crashes);


/*
* Tests tokenization of valid srtrings for arithmetic ops 'add', 'sub', 'mul', and 'div'.
*/
bool test_valid_arithmetic_op_tokenizer(void);


/*
* Tests tokenization of invalid srtrings for arithmetic ops 'add', 'sub', 'mul', and 'div'.
*/
bool test_invalid_arithmetic_op_tokenizer(void);


/*
* tests tokenization of valid numbers in decimal base.
*/
bool test_valid_decimal_tokenizer(void);


/*
* tests tokenization of invalid numbers in decimal base.
*/
bool test_invalid_decimal_tokenizer(void);


/*
 tests tokenization of valid numbers in hexadecimal base.
*/
bool test_valid_hexadecimal_tokenizer(void);


/*
* tests tokenization of invalid numbers in hexadecimal base.
*/
bool test_invalid_hexadecimal_tokenizer(void);


/*
* tests tokenization of valid numbers in binary base.
*/
bool test_valid_binary_tokenizer(void);


/*
* tests tokenization of invalid numbers in binary base.
*/
bool test_invalid_binary_tokenizer(void);


/*
* tests tokenization of valid numbers in octal base.
*/
bool test_valid_octal_tokenizer(void);


/*
* tests tokenization of invalid numbers in octal base.
*/
bool test_invalid_octal_tokenizer(void);


/* 
* Tests the division-by-zero validator function.
*/
bool test_div_by_zero_validator(void);


typedef struct {
    const char *expr;
    tokens_status expected;
} parens_validator_test_t;

/*
* Tests the parentheses validator function.
*/
bool test_parens_validator(void);

