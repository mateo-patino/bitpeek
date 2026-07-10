#ifndef PRINTER_H
#define PRINTER_H

#include "token.h"

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
* Prints 'res' in binary, octal, decimal, and hexadecimal base to 'stream'.
*/
void print_all_bases(FILE *stream, value_t res, bool caps);


/*
* Prints 'res' in binary format to 'stream'. If 'caps' is true, the number is prefixed with
* OB and with 0b otherwise.
*/
void print_binary(FILE *stream, value_t res, bool caps);


/* 
* Pretty prints a value 'res' to 'stream' in the 'base' number system.
* If 'caps' is true, it prints the value in caps. If 'add_newline' is true,
* it prints a newline after the number.
*/
void pretty_print_value(FILE* stream, value_t res, int base, bool caps, bool add_newline);

#endif
