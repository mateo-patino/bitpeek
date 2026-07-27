# BitPeek

BitPeek is a command-line tool for inspecting integers in binary, octal, decimal, and hexadecimal. It was designed for students learning computer systems and programmers who frequently work with low-level integer representations.

## Installation
Clone the repository and build using `make`.

```sh

git clone https://github.com/mateo-patino/bitpeek.git
cd bitpeek
make
````
Optionally, add it to your local binaries directory to run it from anywhere in your system,
```sh
sudo cp bitpeek /usr/local/bin/
```

## Usage

`bitpeek` is a command-line program that evaluates expressions containing binary, octal, decimal, and hexadecimal numbers and prints the result in four bases.

```sh
$ ./bitpeek "0xABC + 67 + 013 + 0b1010"
    Base 2:        0b101100010100
    Base 8:        05424
    Base 10:       2,836
    Base 16:       0xb14
``` 
Use the `0b` prefix for writing binary numbers, a leading `0` for octal numbers, and the `0x` prefix for hexadecimal numbers. Note that **every number, operator, and parenthesis must be surrounded by whitespaces**. In other words, "2 + ( 1 + 1 )" is a valid expression but "2+(1+1)" is not.

## Supported Operators

`bitpeek` supports the following arithmetic and bitwise operators, which strictly follow the precedence and associativity rules of the **C programming language**.

| Operator | Aliases | Precedence | Associativity |
| --- | --- | --- | --- |
| NOT | `not`, `~`, `bitnot` | 13 | Right |
| Multiplication | `mul`, `times`, `*` | 12 | Left |
| Division | `div`, `over`, `/` | 12 | Left |
| Addition | `add`, `plus`, `+` | 11 | Left |
| Subtraction | `sub`, `minus`, `-` | 11 | Left |
| Left shift | `lshift`, `<<`, `leftshift` | 10 | Left |
| Right shift | `rshift`, `>>`, `rightshift` | 10 | Left |
| AND | `and`, `&`, `bitand` | 7 | Left |
| XOR | `xor`, `^`, `bitxor` | 6 | Left |
| OR | `or`, `\|`, `bitor` | 5 | Left |

Note that some operators are **reserved shell symbols**, e.g., `*`,`<<`, `&`, etc, so **surround expressions in quotes** or replace these symbols with aliases (listed above). 


## Digit Grouping

A fundamental idea in computer science is that **four binary digits represent one hexadecimal digit**. Hence, it is conventional to write binary numbers in **groups of four bits**. `bitpeek` lets you do this via command-line arguments. For example, to group binary digits in sets of four bits, pass `-b 4` to the program.

```sh
$ ./bitpeek -b 4 "1023"
    Base 2:        0b 0011 1111 1111
    Base 8:        01777
    Base 10:       1,023
    Base 16:       0x3ff
```

To group octal and hexadecimal digits, use the options `-o` and `-x`, respectively. Below we group hexadecimal by 2 digits and octal by 3 digits because these groupings map to 8 and 9 bits, respectively, and most programmers visualize non-decimal numbers this way.

```sh
$ ./bitpeek -b 4 -x 2 -o 3 "1024 - 1"
    Base 2:        0b 0011 1111 1111
    Base 8:        0 001 777
    Base 10:       1,023
    Base 16:       0x 03 ff
```

## "Real" Examples

Here are examples of problems in computer science and systems where `bitpeek` is a useful visualization tool.

### Convert a number to a different base

```sh
$ ./bitpeek "0xFF"
    Base 2:        0b1111 1111
    Base 8:        0377
    Base 10:       255
    Base 16:       0xff
```

### Align a memory address

When allocating aligned memory or working with memory-mapped hardware, it is common to round an address up to the next 16-byte boundary. Add 15 and clear the lowest four bits to do so.

```sh
$ ./bitpeek -b 4 -x 2 "( 0x1234 + 0x0f ) & ~ 0x0f"
    Base 2:        0b 0001 0010 0100 0000
    Base 8:        011100
    Base 10:       4,672
    Base 16:       0x 12 40
```

### Extract a color channel

An RGB color is often stored as three bytes. The expression below shifts the green byte into place and masks out the other channels from the color `0x5a3c7f`.

```sh
$ ./bitpeek -b 8 -x 2 "( 0x5a3c7f >> 8 ) & 0xff"
    Base 2:        0b 00111100
    Base 8:        074
    Base 10:       60
    Base 16:       0x 3c
```

### Swap the byte order of a number

When moving binary data between systems with different endianness, each byte in a 32-bit number may need to be moved into its opposite position. Mask each byte, then shift it by 8 or 24 bits before combining the pieces.

```sh
$ ./bitpeek -b 8 -x 2 "( ( 0x12345678 & 0xff ) << 24 ) | ( ( 0x12345678 & 0xff00 ) << 8 ) | ( ( 0x12345678 & 0xff0000 ) >> 8 ) | ( ( 0x12345678 & 0xff000000 ) >> 24 )"
    Base 2:        0b 01111000 01010110 00110100 00010010
    Base 8:        017025432022
    Base 10:       2,018,915,346
    Base 16:       0x 78 56 34 12
```

### Find a memory page offset

With 4 KiB pages, the lowest 12 bits of an address identify its offset within the page. A mask of `0xfff` keeps exactly those bits.

```sh
$ ./bitpeek -b 4 -x 2 "0x7fff1234 & 0xfff"
    Base 2:        0b 0010 0011 0100
    Base 8:        01064
    Base 10:       564
    Base 16:       0x 02 34
```

### Check a Unix permission bit

Unix file modes are naturally written in octal. This expression isolates the group-read bit from mode `0755`.

```sh
$ ./bitpeek -b 3 -o 3 -x 2 "0755 & 0040"
    Base 2:        0b 100 000
    Base 8:        0 040
    Base 10:       32
    Base 16:       0x 20
```


