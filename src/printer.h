#ifndef PRINTER_H
#define PRINTER_H

#include "token.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


/* 
* Digits from binary, octal, and hex numbers are printed in groups.
* The constants below are conventional for most programmers, but feel free 
* to change them if you wish to format pretty-print in a different output.
*/
#define GROUP_BINARY_BY 4
#define GROUP_OCTAL_BY 3
#define GROUP_HEX_BY 2


/*
* Returns the highest power of 2 needed to represent 'res' in binary form.
*/
int find_highest_exponent_2(value_t res);

/*
* Prints 'res' in binary, octal, decimal, and hexadecimal base to 'stream'.
*/
void pretty_print_all_bases(FILE *stream, value_t res, bool caps);


/*
* Prints group_by - (digits % group_by) zeroes to stream, adding padding zeroes
* to number whose digits are to be grouped by 'group_by' digits.
*
* Upon success, it returns the number of padding zeroes printed which may be 0 or more.
* It returns -1 if 'stream' is NULL.
*/
int print_group_zero_padding(FILE *stream, int digits, int group_by);


/*
* Prints 'res' in binary format to 'stream'. 
*
* The digits of the printer number are arranged in groups of 'group_by' digits and 
* prefixed by 0b.
*/
void pretty_print_binary(FILE *stream, value_t res, int group_by);


/* Find the number of digits in the largest octal number. Recall one octal digit corresponds to 3 bits */
#define MAXLEN_OCTAL_STR (int)((CHAR_BIT * sizeof(value_t) + 2 ) / 3) /* Ceiling of n / d = ( n + d - 1 ) / d */

/*
* Prints 'res' in octal format to 'stream'.
* The digits are arranged in groups of 'group_by' digits.
*/
void pretty_print_octal(FILE *stream, value_t res, int group_by);


/*
* Prints in 'res' in decimal format using commas to separate thousands.
*/
void pretty_print_decimal(FILE *stream, value_t res);


/*
* Recursively grabs the last 3 digits of an integer and prints them only after
* all the more significant digits have been printed.
*
* It prints a comma after every 3-digit value. Hence, to pretty-print (comma-print)
* a decimal number, the last 3 digits of the number should be processed before 
* calling this function.
*
* It returns true to a caller frame whose 'rem' is the most signifcant thousand in the number.
* This is done to disable the zero padding should 'rem' be less than 3 digits long.
*/
bool pp_decimal_helper(FILE *stream, value_t quotient);

/* 4 bits equal one hex digit */
#define MAXLEN_HEX_STR (int)(CHAR_BIT * sizeof(value_t) / 4)

/*
* Pretty prints 'res' to 'stream' in hexadecimal.
*/
void pretty_print_hexadecimal(FILE *stream, value_t res, int group_by, bool caps);


/*
* Maps a number 0-15 to a hexadecimal digit 0-F or 0-f depending on 'caps'.
*/
char num_to_hex_digit(char num, bool caps);


/* 
* Pretty prints a value 'res' to 'stream' in the 'base' number system.
* If 'caps' is true, it prints the value in caps. If 'add_newline' is true,
* it prints a newline after the number.
*/
void pretty_print_value(FILE* stream, value_t res, int base, bool caps, bool add_newline);


/*
* Prints 'res' in binary to 'stream' without any prefixes or grouping. 
*/
void raw_print_binary(FILE *stream, value_t res, bool add_newline);


/*
* Prints 'res' in octal to 'stream' without any prefixes or grouping. 
*/
void raw_print_octal(FILE *stream, value_t res, bool add_newline);

/*
* Prints 'res' in decimal to 'stream' without any commas. 
*/
void raw_print_decimal(FILE *stream, value_t res, bool add_newline);


/*
* Prints 'res' in hexadecimal to 'stream' without any prefixes or grouping.. 
*/
void raw_print_hexadecimal(FILE *stream, value_t res, bool add_newline);

#endif

