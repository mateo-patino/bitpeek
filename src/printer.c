#include "printer.h"

#include <inttypes.h>
#include <limits.h>


int find_highest_exponent_2(value_t res) {
    for (int exp = CHAR_BIT * sizeof(value_t) - 1; exp >= 0; exp--) {
        if (res & ((value_t)1 << exp)) {
            return exp;
        }
    }
    return 0;
}


void pretty_print_value(FILE *stream, value_t res, int base, bool caps, bool add_newline) {
    if (!stream) {
        return;
    }
    switch(base) {
        case 2:
            pretty_print_binary(stream, res, GROUP_BINARY_BY);
            break;
        case 8:
            pretty_print_octal(stream, res, GROUP_OCTAL_BY);
            break;
        case 10:
            pretty_print_decimal(stream, res);
            break;
        case 16:
            pretty_print_hexadecimal(stream, res, GROUP_HEX_BY, caps);
            break;
        default:
            break;
    }

    if (add_newline) {
        fprintf(stream, "\n");
    }
}

void pretty_print_all_bases(FILE *stream, value_t res, bool caps) {
    if (!stream) {
        return;
    }
    fprintf(stream, "Binary:      ");
    pretty_print_value(stream, res, 2, caps, true);

    fprintf(stream, "Octal:       ");
    pretty_print_value(stream, res, 8, caps, true);

    fprintf(stream, "Decimal:     ");
    pretty_print_value(stream, res, 10, caps, true);

    fprintf(stream, "Hexadecimal: ");
    pretty_print_value(stream, res, 16, caps, true);
}


int print_group_zero_padding(FILE *stream, int digits, int group_by) {
    if (!stream) {
        return -1;
    }
    int padding = 0;
    int rem;
    if ((rem = digits % group_by) != 0) {
        padding = group_by - rem;
        for (int i = 0; i < padding; i++) {
            fputc('0', stream);
        }
    }
    return padding;
}


void pretty_print_binary(FILE *stream, value_t res, int group_by) {
    if (!stream) {
        return;
    }
    fprintf(stream, "0b ");

    int highest_exponent = find_highest_exponent_2(res); 
    int digits = highest_exponent + 1;

    /* Add padding to print whole groups if needed */
    int padding = print_group_zero_padding(stream, digits, group_by);
    
    int digits_printed = padding;
    int total_digits = digits + padding;
    for (int exp = highest_exponent; exp >= 0; exp--) {
        if (res & ((value_t)1 << exp)) {
            fputc('1', stream);
        }
        else {
            fputc('0', stream);
        }
        digits_printed++;

        if (digits_printed < total_digits && digits_printed % group_by == 0) {
            fputc(' ', stream);
        }
    }
}


void pretty_print_octal(FILE *stream, value_t res, int group_by) {
    if (!stream) {
        return;
    }
    fprintf(stream, "0 ");

    char buf[MAXLEN_OCTAL_STR];
    value_t quotient = res;
    char remainder;
    int digits = 0;

    /* Compute octal digits by recording remainders */
    do {
        remainder = quotient % 8;
        quotient = quotient / 8;
        buf[digits++] = remainder;
    } while (quotient > 0);

    /* Pad to achieve whole groups */
    int padding = print_group_zero_padding(stream, digits, group_by);
    
    int digits_printed = padding;
    int total_digits = digits + padding;
    for (int i = digits - 1; i >= 0; i--) {
        fputc('0' + buf[i], stream);
        digits_printed++;

        if (digits_printed < total_digits && digits_printed % group_by == 0) {
            fputc(' ', stream);
        }
    }

}


void pretty_print_decimal(FILE *stream, value_t res) {
    if (!stream) {
        return;
    }
    /* Compute least-significant thousand outside the recursion (to skip comma) */ 
    value_t rem = res % 1000;
    bool is_mst = pp_decimal_helper(stream, res / 1000);

    /* If this frame's 'rem' is the most-significant thousand, don't pad */
    if (!is_mst && rem < 100) {
        int padding = rem > 9 ? 1 : 2;
        for (int i = 0; i < padding; i++) {
            fputc('0', stream);
        }
    }
    fprintf(stream, "%" PRIu64, rem); /* Least significant thousand doesn't have a comma */
}


bool pp_decimal_helper(FILE *stream, value_t quotient) {
    if (!quotient) {
        /* The caller whose quotient gave zero must be the MST */
        return true;
    }
    value_t rem = quotient % 1000;
    bool is_mst = pp_decimal_helper(stream, quotient / 1000);

    /* If this frame's 'rem' is the most-significant thousand, don't pad */
    if (!is_mst && rem < 100) {
        int padding = rem > 9 ? 1 : 2;
        for (int i = 0; i < padding; i++) {
            fputc('0', stream);
        }
    }
    fprintf(stream, "%" PRIu64 ",", rem);
    return false;
}


void pretty_print_hexadecimal(FILE *stream, value_t res, int group_by, bool caps) {
    if (!stream) {
        return;
    }
    fprintf(stream, "0x ");

    char buf[MAXLEN_HEX_STR];
    char remainder;
    int quotient = res;
    int digits = 0;

    /* Record hex digits from least-significant digit to most-significant */
    do {
        remainder = quotient % 16;
        quotient = quotient / 16;
        buf[digits++] = remainder; /* Save the raw value 0-15 */
    } while (quotient > 0);
   
    /* Add padding if needed */
    int padding = print_group_zero_padding(stream, digits, group_by);

    int digits_printed = padding;
    int total_digits = digits + padding;
    for (int i = digits - 1; i >= 0; i--) {
        fputc(num_to_hex_digit(buf[i], caps), stream);
        digits_printed++;

        if (digits_printed < total_digits && digits_printed % group_by == 0) {
            fputc(' ', stream);
        }
    }
}


char num_to_hex_digit(char num, bool caps) {
    if (num < 0 || num > 15) {
        return '?';
    }

    if (num >= 0 && num <= 9) {
        return '0' + num;
    }
    return (caps ? 'A' : 'a') + (num - 10);
}
