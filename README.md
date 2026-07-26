# BitPeek

Online calculators often don't provide a useful representation of numbers in bases other than 10. I made this program to see numbers in **binary, octal, and hexadecimal** easily with convenient features that online calculators do not generally support. 

It is especially designed for students learning about number systems and people working in systems programming.

## Installation
Clone the repository and build using `make`.

```sh

git clone https://github.com/yourname/bitpeek.git
cd bitpeek
make
````
Optionally, add it to your local binaries directory to run it from anywhere in your system,
```sh
sudo cp bitpeek /usr/local/bin
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

`bitpeek` supports the following arithmetic and bitwise operators, which strictly follow the precedence and associativity rules of the **C programming language**. All operators have **aliases that can be used interchangeably** in the command-line.

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

### Convert a number to a different base

```sh
$ ./bitpeek "0xFF"
    Base 2:        0b1111 1111
    Base 8:        0377
    Base 10:       255
    Base 16:       0xff
```



