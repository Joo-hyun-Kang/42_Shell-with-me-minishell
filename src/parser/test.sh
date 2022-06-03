#!/bin/bash

echo testing
make
gcc -g -Wall -Wextra -Werror main.c libparser.a
./a.out
make fclean
rm -rf a.out.dSYM
