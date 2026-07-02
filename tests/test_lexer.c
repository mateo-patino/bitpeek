#include "test_lexer.h"
#include "test_assert.h"

#include "token.h"
#include "lexer.h"

#include <stdbool.h>

bool test_valid_arithmetic_op_tokenizer(void) {
    operation_type op;
    bool retval;

    retval = is_operation("add", &op);
    ASSERT_TRUE(retval);
    ASSERT_TRUE(op == ADD);

    retval = is_operation("sub", &op);
    ASSERT_TRUE(retval);
    ASSERT_TRUE(op == SUB);

    retval = is_operation("mul", &op);
    ASSERT_TRUE(retval);
    ASSERT_TRUE(op == MUL);

    retval = is_operation("div", &op);
    ASSERT_TRUE(retval);
    ASSERT_TRUE(op == DIV);

    return true;
}
