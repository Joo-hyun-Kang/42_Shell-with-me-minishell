#!/bin/bash

echo testing
make
gcc -Wall -Wextra -Werror main.c libtoken.a
./a.out
rm a.out
make fclean
