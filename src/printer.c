#include "printer.h"

#include <inttypes.h>


void pretty_print_value(FILE *stream, value_t res, int base, bool caps, bool add_newline) {
    if (!stream) {
        return;
    }
    switch(base) {
        case 2:
            print_binary(stream, res, caps);
            break;
        case 8:
            fprintf(stream, "0%" PRIo64, res);
            break;
        case 10:
            fprintf(stream, "%" PRIu64, res);
            break;
        case 16:
            if (caps) {
                fprintf(stream, "0x%" PRIX64, res);
            }
            else {
                fprintf(stream, "0x%" PRIx64, res);
            }
            break;
        default:
            break;
    }

    if (add_newline) {
        fprintf(stream, "\n");
    }
}

void print_all_bases(FILE *stream, value_t res, bool caps) {
    fprintf(stream, "Binary:      ");
    pretty_print_value(stream, res, 2, caps, true);

    fprintf(stream, "Octal:       ");
    pretty_print_value(stream, res, 8, caps, true);

    fprintf(stream, "Decimal:     ");
    pretty_print_value(stream, res, 10, caps, true);

    fprintf(stream, "Hexadecimal: ");
    pretty_print_value(stream, res, 16, caps, true);
}


void print_binary(FILE *stream, value_t res, bool caps) {
    /* TODO: write a compiler-independent printer (i.e. don't use %b) */
    fprintf(stream, "Unavailable");
    (void)res;
    (void)caps;
}
