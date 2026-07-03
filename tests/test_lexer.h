#include <stdbool.h>

/*
* Calls all of the unit and integration tests for testing the lexer. 
*
* Returns the number of tests passed, and if 'total_ran' is not NULL, it writes the total
* number of tests ran there.
*/
int run_lexer_tests(int *total_ran);


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


