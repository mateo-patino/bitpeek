#include "lexer.h"
#include "token.h"

#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


tokens_status create_tokens_from_string(char *str, token_t *addr, char **invalid) {
    tokens_status status;
    size_t i = 0;
    const char *sep = SPACE_CHARS;
    char *tok = strtok(str, sep);

    while (tok) {
        status = create_token_from_str(tok, addr + i);
        if (status != TOKENS_OK) {
            if (invalid) {
                *invalid = tok;
            }
            addr[i].type = INVALID_TOKEN;
            addr[i].obj = NULL;
            for (size_t t = 0; t < i; t++) {
                free_token_obj(addr + t);
            }
            return status;
        }
        i++;
        tok = strtok(NULL, sep);
    }

    return TOKENS_OK;
}


tokens_status create_tokens_from_argv(char **argv, token_t *addr, char **invalid) {
    char **ptr = argv;
    tokens_status status;
    size_t i = 0;

    while (*ptr) {
        status = create_token_from_str(*ptr, addr + i);
        if (status != TOKENS_OK) {
            if (invalid) {
                *invalid = *ptr;
            }
            addr[i].type = INVALID_TOKEN;
            addr[i].obj = NULL;
            for (size_t t = 0; t < i; t++) {
                free_token_obj(addr + t);
            }
            return status;
        }
        i++;
        ptr++;
    } 

    return TOKENS_OK;
}


tokens_status create_token_from_str(const char *str, token_t *addr) {
    if (str == NULL || *str == '\0') {
        return TOKENS_NULL_STR;
    }
    size_t len = strlen(str);
    char s[len + 1];
    for (size_t i = 0; i <= len; i++) {
        s[i] = tolower((unsigned char)str[i]);
    }

    /* Check if 'str' is a valid number */
    int base;
    value_t val;
    if (is_number(s, &base, &val)) {
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
    if (is_operation(s, &type)) {
        operand_t *op = init_operand(type);
        if (!op) {
            return TOKENS_MALLOC_FAILURE;
        }
        addr->type = OPERAND;
        addr->obj = op;

        return TOKENS_OK;
    }
    
    /* Check if 'str' is a parenthesis */
    if (strcmp(s, "(") == 0) {
        addr->type = LPAREN;
        addr->obj = NULL;
        return TOKENS_OK;
    }
    else if (strcmp(s, ")") == 0) {
        addr->type = RPAREN;
        addr->obj = NULL;
        return TOKENS_OK;
    }
    
    /* At this point, 'str' is neither a number, operand, or parenthesis */
    return TOKENS_INVALID_ARG;
}


int get_base_from_str(const char *str) {
    if (!str || *str == '\0') {
        return -1;
    }

    /* Ignore leading whitespace */
    while (isspace(*str)) { str++; }

    /* Check if first digit is '0' */
    if (*str == '0') {
        /* Zero */
        if (str[1] == '\0') {
            return 10;
        }
        else if (str[1] == 'b') {
            return 2;
        }
        else if (str[1] == 'x') {
            return 16;
        }
        /* Octal: check second digit is valid octal digit */
        else if (str[1] >= '0' && str[1] <= '7') {
            return 8;
        }
        
        return -1;
    }
    /* Check if first digit is [1, 9] */
    else if (*str >= 49 && *str <= 57) {
        return 10;
    }
    
    return -1;
}


bool get_number_from_str(const char *str, const int base, value_t *val) {
    if (!str || *str == '\0') {
        return false;
    }
    char *end;
    errno = 0;
    unsigned long long num = strtoull(str, &end, base);

    /* Consume trailing whitespace */
    while (isspace(*end)) { end++; }

    if (*end == '\0' && errno == 0) {
        if (val) { *val = (value_t)num; }
        return true;
        
    }

    return false;
}

bool has_dash(const char *str) {
    if (!str || *str == '\0') {
        return false;
    }
    size_t len = strlen(str);
    for (size_t i = 0; i < len; i++) {
        if (str[i] == '-') {
            return true;
        }
    }

    return false;
}


void print_token_error(tokens_status status, const char *msg) {
    if (!msg) {
        msg = "";   
    }
    switch (status) {
        case TOKENS_OK:
            fprintf(stdout, "Valid tokens. %s\n", msg);
            break;
        case TOKENS_INVALID_ARG:
            fprintf(stderr, "Invalid argument. %s\n", msg);
            break;
        case TOKENS_ULL_OVERFLOW:
            fprintf(stderr, "Number is too large. %s\n", msg);
            break;
        case TOKENS_NULL_STR:
            fprintf(stderr, "NULL string was unexpectedly passed. %s\n", msg);
            break;
        case TOKENS_MALLOC_FAILURE:
            fprintf(stderr, "malloc() failed. %s\n", msg);
            break;
        default:
            fprintf(stderr, "%i is not a registered status code.\n", status);
    }
}


bool is_number(const char *str, int *base, value_t *val) {
    if (!str || *str == '\0' || has_dash(str)) {
        return false;
    }

    while (isspace(*str)) { str++; }

    int str_base;
    if ((str_base = get_base_from_str(str)) == -1) {
        return false;
    }

    /* Skip the prefix */
    switch (str_base) {
        case 2:
            str += 2;
            break;
        case 8:
            str++;
            break;
        case 10:
            /* No prefix to skip for decimal */
            break;
        case 16:
            str += 2;
            break;
        default:
            /* Only the bases above are valid, so return failure otherwise */
            return false;
    }

    if (!get_number_from_str(str, str_base, val)) {
        return false;
    }

    /* get_number_from_str writes the value to 'val' */
    if (base) { *base = str_base; }

    return true;
}


bool is_operation(const char *str, operation_type *type) {
    if (!str || *str == '\0') {
        return false;
    } 
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

