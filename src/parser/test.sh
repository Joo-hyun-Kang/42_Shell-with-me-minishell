#!/bin/bash

echo "#====================================================#"
echo "#==wwwwwwwwwww=wwwwwwwwwww====wwwwww====wwwwwwwwwww==#"
echo "#======www=====www=========www======www=====www======#"
echo "#======www=====wwwwwwwwwww====wwwwww========www======#"
echo "#======www=====www=========www======www=====www======#"
echo "#======www=====wwwwwwwwwww====wwwwww========www======#"
echo "#==================================by.kanghyki=======#"
make
gcc -g -Wall -Wextra -Werror main.c libparser.a
./a.out
make fclean
rm -rf a.out.dSYM
