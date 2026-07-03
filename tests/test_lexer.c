#include "test_lexer.h"
#include "test_assert.h"

#include "token.h"
#include "lexer.h"

#include <stdbool.h>

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
    ASSERT_TRUE(!is_operation("+add", NULL));
    ASSERT_TRUE(!is_operation("add+", NULL));
    ASSERT_TRUE(!is_operation("addd", NULL));
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

     ASSERT_TRUE(is_number("123456789", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     ASSERT_TRUE(is_number("0", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)0, val);

     ASSERT_TRUE(is_number("1", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)1, val);

     ASSERT_TRUE(is_number("123456789", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)123456789, val);

     ASSERT_TRUE(is_number("18446744073709551615", &base, &val));
     ASSERT_EQ_INT(10, base);
     ASSERT_EQ_VALUE_T((value_t)18446744073709551615ULL, val);
 
     return true;
}

