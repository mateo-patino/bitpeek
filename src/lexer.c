#include "lexer.h"
#include "token.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



tokens_status create_tokens_from_argv(char **argv, token_t *addr, char **invalid) {
    char **ptr = argv;
    tokens_status status;
    size_t i = 0;

    while (*ptr != NULL) {
        status = create_token_from_str(*ptr, addr + i);
        /* TODO */

    } 
}


tokens_status create_token_from_str(const char *str, token_t *addr) {
    if (str == NULL || strlen(str) == 0) {
        return TOKENS_NULL_STR;
    }

    /* Check if 'str' is a valid number */
    int base;
    value_t val;
    if (is_number(str, &base, &val)) {
        number_t *number = init_number(val, base);
        if (!number) {
            return TOKENS_MALLOC_FAILURE;
        }
        addr->type = NUMBER;
        addr->obj = number;
        return TOKENS_OK;
    }
    else {
        /* errno could also be EINVAL ('str' is not a number) but that doesn't mean the string 
         * is invalid. The string could be a non-numeric token, hence we continue checking. */
        if (errno == ERANGE) {
            return TOKENS_ULL_OVERFLOW;
        }
    }

    /* Check if 'str' is a valid operand */
    operation_type type;
    if (is_operation(str, &type)) {
        /* CONTINUE HERE: implement and call init_operand to tokenize */
    }
    

    return TOKENS_OK;

}


bool is_number(const char *str, int *base, value_t *val) {
    if (!str || strlen(str) == 0) {
        return false;
    }
    char *endptr = NULL;
    unsigned long long result;

    errno = 0;
    result = strtoull(str, &endptr, 10);
    if (endptr == str) {
        /* All 3 non-decimal bases are prefixed by a 0*, so endptr != str in such cases */ 
        return false;
    }
    else if (*endptr == '\0') {
        /* A leading 0 signals an octal (not inferred by strtoull by default, which skips the leading zero) */
        if (*str == '0') {
            errno = 0;
            result = strtoull(str, &endptr, 8);
            if (errno == 0) {
                if (base) { *base = 8; }
                if (val) { *val = (value_t)result; }
                return true;
            }
            return false;
        }
        /* Decimal */
        if (errno == 0) {
            if (base) { *base = 10; }
            if (val) { *val = (value_t)result; }
            return true;
        }
        return false;    
    }
    else if ((int)(endptr - str) == 1) {
        /* Check for bases prefixed by 0b and 0x */
        errno = 0;
        if (*endptr == 'b') {
            result = strtoull(str + 2, &endptr, 2);

            if (errno == 0 && *endptr == '\0') {
                if (base) { *base = 2; }
                if (val) { *val = (value_t)result; }
                return true;
            }
            return false;
        }
        else if (*endptr == 'x') {
            result = strtoull(str + 2, &endptr, 16);

            if (errno == 0 && *endptr == '\0') {
                if (base) { *base = 16; }
                if (val) { *val = (value_t)result; }
                return true;
            }
            return false;
        }
    }
    return false;
}


bool is_operation(const char *str, operation_type *type) {
   for (int i = 0; i < NUM_OP; i++) {
       for (int j = 0; j < SYNONYMS_PER_OP; j++) {
           if (strcmp(str, operation_labels[i][j]) == 0) {
               if (type) { *type = (operation_type)i; }
               return true;
           }
       }
   }
   return false;
}

