#include "test_lexer.h"
#include "test_assert.h"

#include "token.h"
#include "lexer.h"

bool test_valid_arithmetic_op_tokenizer(void) {
    operation_type op;
    bool result;

    ASSERT_TRUE(is_operation("add", &op) == ADD);
    
    ASSERT_TRUE(is_operation("sub", &op) == SUB);

    ASSERT_TRUE(is_operation("mul", &op) == MUL);

    ASSERT_TRUE(is_operation("div", &op) == DIV);

}
