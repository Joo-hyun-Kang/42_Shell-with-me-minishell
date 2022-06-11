/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/11 15:42:56y kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_argument	*ft_init_argument(void)
{
	t_argument	*argument;

	argument = (t_argument *)malloc(sizeof(t_argument));
	if (argument == 0)
		return (0);
	ft_memset(argument, 0, sizeof(t_argument));
	return (argument);
}

char	**ft_malloc_pa_argument(t_token *cur_token)
{
	char	**pa_argument;
	int		i;

	i = 0;
	while (cur_token->next)
	{
		if (cur_token->token_type != ARGUMENT)
			break ;
		++i;
		cur_token = cur_token->next;
	}
	pa_argument = (char **)malloc(sizeof(char *) * (i + 1));
	if (pa_argument == 0)
		return (0);
	ft_memset(pa_argument, 0, sizeof(char *) * (i + 1));
	return (pa_argument);
}

void	ft_add_argument(t_argument **head, t_argument *arg)
{
	t_argument	*iter;

	if (*head == 0)
		*head = arg;
	else
	{
		iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = arg;
	}
}

char	*ft_return_type_char(enum e_token_type token_type)
{
	if (token_type == SEMICOLON)
		return (";");
	else if (token_type == LT)
		return (">");
	else if (token_type == DLT)
		return (">>");
	else if (token_type == GT)
		return ("<");
	else if (token_type == DGT)
		return ("<<");
	else
		return ("\\n");
}

void	ft_combine_pipe_str(t_token *cur_token, char **env)
{
	char	*read_line;
	t_token	*add;

	read_line = readline("> ");
	add = ft_tokenization(read_line, env);
	free(read_line);
	cur_token->next = add;
}

void	ft_replace_heredoc(t_token *cur_token, char **env)
{
	char	*read_line;
	char	*new_pa_str;
	char	*for_free;
	t_token	*new_token;

	new_pa_str = ft_strdup("");
	read_line = readline("> ");
	while (ft_strcmp(cur_token->next->pa_str, read_line) != 0)
	{
		ft_combine_str(&new_pa_str, read_line);
		for_free = new_pa_str;
		new_pa_str = ft_strjoin(new_pa_str, "\n");
		free(for_free);
		read_line = readline("> ");
	}
	free(read_line);
	t_token	*next_next_token = cur_token->next->next;
	free(cur_token->next->pa_str);
	free(cur_token->next);
	new_token = ft_tokenization(new_pa_str, env);
	free(new_pa_str);
	cur_token->next = new_token;
	while (new_token->next != 0)
		new_token = new_token->next;
	new_token->next = next_next_token;
}

t_token	*ft_read_token(t_token *cur_token, t_argument *out_arg, int index)
{
	if (cur_token->token_type == ARGUMENT)
	{
		out_arg->pa_argument[index] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, out_arg, index + 1));
	}
	else
	{
		out_arg->next_token_type = cur_token->token_type;
		if (cur_token->token_type == EOL)
			return (cur_token);
		else if (cur_token->next->token_type != ARGUMENT)
		{
			if (cur_token->token_type == PIPE)
			{
				ft_combine_pipe_str(cur_token, *(out_arg->env));
				return (cur_token->next);
			}
			else
			{
				printf("minishell: parse error near `%s'\n", \
					ft_return_type_char(cur_token->next->token_type));
				return (0);
			}
		}
		else if (cur_token->token_type == DGT)
			ft_replace_heredoc(cur_token, *(out_arg->env));
		return (cur_token->next);
	}
}

/* Never in EOL */
t_token	*ft_read_token_state_only_argument(t_token *cur_token, t_argument *out_arg, int index)
{
	if (cur_token->token_type != ARGUMENT)
	{
		if (cur_token->token_type == PIPE)
		{
			printf("minishell: parse error near `|'\n");
			return (0);
		}
		else
			return (ft_read_token(cur_token, out_arg, -1));
	}
	else
	{
		out_arg->pa_argument[index] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, out_arg, index + 1));
	}
}

t_argument	*ft_create_argument(char *str, char ***env)
{
	t_argument	*head_arg;
	t_argument	*cur_arg;
	t_token	*head_token;
	t_token	*cur_token;

	if (*str == 0)
		return (0);
	head_token = ft_tokenization(str, *env);
	cur_token = head_token;
	head_arg = 0;
	while (cur_token->token_type != EOL)
	{
		cur_arg = ft_init_argument();
		cur_arg->pa_argument = ft_malloc_pa_argument(cur_token);
		cur_arg->env = env;
		cur_token = ft_read_token_state_only_argument(cur_token, cur_arg, 0);
		if (cur_token == 0)
		{
			ft_delete_argument(head_arg);
			break ;
		}
		ft_add_argument(&head_arg, cur_arg);
	}
	ft_delete_token(head_token);
	return (head_arg);
}

void	ft_delete_token(t_token *token)
{
	t_token *prev;

	prev = token;
	while (token != 0)
	{
		token = token->next;
		free(prev);
		prev = token;
	}
}

void	ft_delete_argument(t_argument *arg)
{
	t_argument	*prev;
	int			i;

	prev = arg;
	while (arg != 0)
	{
		i = 0;
		while (arg->pa_argument[i] != 0)
		{
			free(arg->pa_argument[i]);
			++i;
		}
		free(arg->pa_argument);
		arg = arg->next;
		free(prev);
		prev = arg;
	}
}
