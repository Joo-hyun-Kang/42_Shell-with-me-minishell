long    ft_atol(char *str)
{
    long        ret;
    int         minus;
    int         i;
    ret = 0;
    minus = 1;
    i = 0;
    while ((str[i] >= 9 && str[i] <= 13) || str[i] == 32)
        i++;
    if (str[i] == '+' || str[i] == '-')
    {
        if (str[i] == '-')
            minus = -1;
        i++;
    }
    while (str[i])
    {
        ret = ret * 10 + (str[i] - '0');
        i++;
    }
    if (minus == -1)
        ret = ret * minus;
    return (ret);
}
void func_exit(int argc,char **argv)
{
    int i = -1;
    long long first_argv;
    first_argv = ft_atol(argv[1]);
    if (argc == 1)
    {
        printf("exit\n");
        exit(g_state.exit_status);
    }
    else if (argc == 2 && first_argv >= -2147483648 && ft_atoi(argv[1]) <= 2147483647)
    {
        printf("exit\n");
        g_state.exit_status = ft_atoi(argv[1]);
        if (g_state.exit_status >= 0)
            g_state.exit_status = g_state.exit_status % 256;
        else
        {
            g_state.exit_status *= -1;
            g_state.exit_status = 256 - (g_state.exit_status % 256);
        }
        exit(g_state.exit_status);
    }
    else if (argc > 2  && first_argv >= -2147483648 && ft_atoi(argv[1]) <= 2147483647)
    {
        printf("exit\n");
        printf("bash: exit: too many arguments\n");
        g_state.exit_status = 1;
        return ;
    }
    else
    {
        printf("exit\n");
        printf("bash: exit: %s: numeric argument required\n",argv[1]);
        g_state.exit_status = 255;
        exit(g_state.exit_status);
    }
}