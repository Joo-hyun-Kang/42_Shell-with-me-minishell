make -C ../../library/libft
gcc env_bst.c test.c ../../library/libft/libft.a -o envtest
./envtest
rm envtest
make fclean -C ../../library/libft
