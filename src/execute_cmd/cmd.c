/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/25 16:53:50 by jokang            #+#    #+#             */
/*   Updated: 2022/07/04 20:03:03 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

void	ft_system(t_argument *argument)
{
	char	*command;

	if (argument->pa_argument[COMMAND_POSITION] == NULL)
	{
		argument = argument->next;
		command = ft_strdup(argument->pa_argument[COMMAND_POSITION]);
		ft_error(EXE_NO_DIR, command, true);
		free(command);
		return ;
	}
	while (argument != NULL)
	{
		if (argument->next_token_type == EOL)
			ft_execute_single_cmd(&argument);
		else
			ft_execute_mult_cmd(&argument);
	}
}

void	ft_execute_mult_cmd(t_argument **arg)
{
	t_pipes				*pa_pipes;
	enum e_token_type	token;
	int					state;
	pid_t				child_pid;

	pa_pipes = (t_pipes *)malloc_safe(sizeof(t_pipes));
	ft_construt_pipes(*arg, pa_pipes);
	state = INIT;
	while (*arg != NULL)
	{
		token = (*arg)->next_token_type;
		if (token == PIPE)
			child_pid = ft_execute_pipe(arg, state, pa_pipes);
		else if (ft_is_redir(token))
			child_pid = ft_execute_redir(arg, state, pa_pipes);
	}
	ft_close_pipe(pa_pipes);
	ft_wait_child(child_pid);
	ft_free_pipes(&pa_pipes);
}

char	*ft_join_path_command(char *path, char *command)
{
	char	*pa_temp;
	char	*pa_path_with_command;

	pa_temp = ft_strjoin(path, "/");
	pa_path_with_command = ft_strjoin(pa_temp, command);
	free(pa_temp);
	return (pa_path_with_command);
}

void	*ft_intcpy(void *dest, const void *src, size_t n)
{
	size_t	i;

	i = 0;
	if (!dest && !src)
		return (0);
	while (i < n)
	{
		((int *)dest)[i] = ((int *)src)[i];
		++i;
	}
	return (dest);
}

void	ft_copy_char_arr_list(char **dst, char **src, int src_len)
{
	int	i;

	i = 0;
	while (i < src_len)
	{
		*dst = ft_strdup(*src);
		++dst;
		++src;
		++i;
	}
}
