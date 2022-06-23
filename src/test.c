/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/19 15:43:42 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 19:57:09 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include <signal.h>

#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define ENTER "\033[1;30;47m"
#define GREEN "\033[1;32m"
#define CYAN "\033[1;35m"
#define RED "\033[1;31m"
#define DEFAULT "\033[0;0m"

int	testSize = 0;
int	is_done = 0;

/*
 * GNL
 * GNL
 * GNL
 * GNL
 * GNL
 */
typedef struct s_gnlist {
	int				fd;
	char			*storage;
	struct s_gnlist	*next;
	struct s_gnlist	*back;
}	t_gnlist;

/*
 * UTILS
 * UTILS
 * UTILS
 * UTILS
 * UTILS
 */
static void		sig_handler(int sig);
static char		**getTestScript(void);
static char		ft_scan_chr(void);
static void		print_test_cmd_list(char **cmdStrDPtr);
static void		move_idx(char **cmdStrDPtr, int *idx);
static int		ft_strdplen(char **str);
static char		**ft_add_cmd(char **dp, char *str);
static void		ft_show_argument_test(t_argument *arg);
static void		*ft_system_test(char *cmd);
static void		*system_test(char *cmd);
static bool		check_work_done(int waitTime);
/* ---------------------------------------------------------------- */

int	main(int argc, char **argv, char **ori_env)
{
	const int	waitTime = 3;
	int			idx = 0;
	char		c;
	int			pid;
	char		**cmdStrDPtr;
	t_argument	*arg;
	t_env_root	*env;


	env = ft_dpenv_to_bstenv(ori_env);
	/* Set signal */
	/* Set signal */
	/* Set signal */
	signal(SIGUSR1, sig_handler);

	/* Add test */
	/* Add test */
	/* Add test */
	cmdStrDPtr = getTestScript();

	/* Testing  */
	/* Testing  */
	/* Testing  */
	while (1)
	{
		if (idx > 0)
		{
			if (cmdStrDPtr[idx] == NULL)
				printf("\n%s! This is the last test! (r:reset, u:undo, q:quit, m:move, l:list) >> %s", RED, DEFAULT);
			else
				printf("\n%sPress key... (r:reset, u:undo, q:quit, m:move, l:list) >>%s ", ENTER, DEFAULT);
			c = ft_scan_chr();
			if (c == 'r')
				idx = 0;
			else if (c == 'u')
			{
				if (idx > 1)
					idx -= 2;
				else
					idx = 0;
			}
			else if (c == 'l')
			{
				print_test_cmd_list(cmdStrDPtr);
				--idx;
			}
			else if (c == 'm')
				move_idx(cmdStrDPtr, &idx);

			if (c == 'q' || cmdStrDPtr[idx] == NULL)
				return 0;
		}
		system("clear");

		printf("#--------------------------------------------------#\n");
		printf(" %sCOMMAND%s:                                           \n", YELLOW, DEFAULT);
		printf("      %s%-45s%s \n", GREEN, cmdStrDPtr[idx], DEFAULT);
		printf("                                             # %s%-3d%s  \n", GREEN, idx, DEFAULT);
		printf("#--------------------------------------------------#\n");


//		arg = ft_parser(ft_strdup(cmdStrDPtr[idx]), env);
//		printf("\n##--------%s Argument %s--------##\n", CYAN, DEFAULT);
//		ft_show_argument_test(arg);
//		ft_free_argument(arg);

		/*
		 *
		 * ft_system
		 *
		 */
		is_done = 0;

		pid = fork();
		if (pid == 0)
			ft_system_test(cmdStrDPtr[idx]);
		else
		{
			if (check_work_done(waitTime) == false)
			{
				printf("%sft_system is blocked. Process terminated.%s\n", RED, DEFAULT);
				kill(pid, SIGKILL);
			}
			wait(&pid);
			fflush(stdin);
		}

		/*
		 *
		 * system
		 *
		 */
		is_done = 0;

		pid = fork();
		if (pid == 0)
			system_test(cmdStrDPtr[idx]);
		else
		{
			if (check_work_done(waitTime) == false)
			{
				printf("%sSystem is blocked. Process terminated.%s\n", RED, DEFAULT);
				kill(pid, SIGKILL);
			}
			wait(&pid);
			fflush(stdin);
		}
		printf("\n%s##-----------------------------##%s\n", BLUE, DEFAULT);

		++idx;
	}
	system("clear");
	printf("#----------------------------------------#\n");
	printf("|                                        |\n");
	printf("|                  %sDONE%s                  |\n", RED, DEFAULT);
	printf("|                                        |\n");
	printf("#----------------------------------------#\n");
}

/*
 *
 *
 *                 TEST UTILS
 *
 *
 *
 * */

static void	sig_handler(int sig)
{
	if (sig == SIGUSR1)
		is_done = 1;
}

static char	**getTestScript(void)
{
	int		fd;
	char	*line;
	char	**cmdStrDPtr;

	cmdStrDPtr = 0;
	fd = open("src/testscript.txt", O_RDONLY);
	if (fd < 0)
		return (NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (*line != '\0')
			cmdStrDPtr = ft_add_cmd(cmdStrDPtr, line);
		line = get_next_line(fd);
	}
	return (cmdStrDPtr);
}

static char	ft_scan_chr(void)
{
	char	c;

	scanf("%c", &c);
	fflush(stdin);
	return (c);
}

static void	print_test_cmd_list(char **cmdStrDPtr)
{
	int		i = 0;
	char	c;

	system("clear");
	while (cmdStrDPtr[i] != NULL)
	{
		printf("# %s%d%s: %s%s%s\n", CYAN, i, DEFAULT, GREEN, cmdStrDPtr[i], DEFAULT);
		++i;
	}
	printf("\n%sPress any key...%s", ENTER, DEFAULT);
	c = ft_scan_chr();
	system("clear");
}

static void	move_idx(char **cmdStrDPtr, int *idx)
{
	int		i = 0;
	int	n;

	system("clear");
	while (cmdStrDPtr[i] != NULL)
	{
		printf("# %s%d%s: %s%s%s\n", CYAN, i, DEFAULT, GREEN, cmdStrDPtr[i], DEFAULT);
		++i;
	}
	printf("\n%sEnter the index you want to move... >>%s ", ENTER, DEFAULT);
	scanf("%d", &n);
	fflush(stdin);
	if (!(n >= 0 && n < testSize))
	{
		printf("\n%sCan't do it... Press any key...%s|\n", RED, DEFAULT);
		ft_scan_chr();
		--(*idx);
	}
	else
		*idx = n;
}

static int	ft_strdplen(char **str)
{
	int	i;

	i = 0;
	while (str[i] != NULL)
		++i;
	return (i);
}

static char	**ft_add_cmd(char **dp, char *str)
{
	int			env_len;
	char		**new_env;
	int			i;

	if (dp == NULL)
		env_len = 0;
	else
		env_len = ft_strdplen(dp);
	new_env = (char **)malloc(sizeof(char *) * (env_len + 2));
	if (new_env == NULL)
		return (NULL);
	new_env[env_len + 1] = NULL;
	i = 0;
	while (i < env_len)
	{
		new_env[i] = ft_strdup(dp[i]);
		++i;
	}
	new_env[i] = ft_strdup(str);
	free(dp);
	++testSize;
	return (new_env);
}

static void	ft_show_argument_test(t_argument *arg)
{
	const char	*t_type_str_test[7] = {"ARGUMENT", "PIPE", "LT", "DLT", "GT", "DGT", "EOL"};
	char		**str;

	if (arg == 0)
	{
		printf("{ %sNULL%s }\n", RED, DEFAULT);
		return ;
	}
	printf("{ next tokenType: %s%s%s }\n", GREEN, t_type_str_test[arg->next_token_type], DEFAULT);
	str = arg->pa_argument;
	while (*str != 0)
		printf("{ arg->pa_argument: \"%s%s%s\" }\n", GREEN, *str++, DEFAULT);
	printf("  |\n");
	printf("  v\n");
	ft_show_argument_test(arg->next);
}

static void	*ft_system_test(char *cmd)
{
	char	*tmp;

	tmp = ft_strjoin("echo '", cmd);
	free(cmd);
	cmd = ft_strjoin(tmp, "' | ./minishell");
	free(tmp);
	printf("\n%s##--------  ft_system  --------##%s\n", BLUE, DEFAULT);
	system(cmd);

	kill(getppid(), SIGUSR1);
	exit(1);
}

static void	*system_test(char *cmd)
{
	char	*tmp;

	printf("\n%s##--------   System    --------##%s\n", BLUE, DEFAULT);
	tmp = ft_strjoin("echo '", cmd);
	free(cmd);
	cmd = ft_strjoin(tmp, "' | bash");
	free(tmp);

	system(cmd);

	kill(getppid(), SIGUSR1);
	exit(1);
}

static bool	check_work_done(int waitTime)
{
	int i;

	for ( i = 0; i < waitTime; i++ )
	{
		if (is_done == 1)
			return (true);
		sleep(1);
	}
	return (false);
}
