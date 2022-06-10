/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/11 02:20:24 by kanghyki         ###   ########.fr       */
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

t_token	*ft_read_token(t_token *cur_token, t_argument *out_arg, int index)
{
	if (cur_token->token_type == ARGUMENT)
	{
		out_arg->pa_argument[index] = cur_token->pa_str;
		return (ft_read_token(cur_token->next, out_arg, index + 1));
	}
	else
	{
		printf("%s\n", cur_token->pa_str);
		out_arg->next_token_type = cur_token->token_type;
		if (cur_token->token_type == EOL)
			return (cur_token);
		return (cur_token->next);
	}
}

t_token	*ft_read_token_state_only_argument(t_token *cur_token, t_argument *out_arg, int index)
{
	if (cur_token->token_type != ARGUMENT)
	{
		printf("Error: parsing error\n");
		return (0);
	}
	out_arg->pa_argument[index] = cur_token->pa_str;
	return (ft_read_token(cur_token->next, out_arg, index + 1));
}

t_argument	*ft_create_argument(char *str, char ***env)
{
	t_argument	*head_arg;
	t_argument	*cur_arg;
	t_token	*head_token;
	t_token	*cur_token;

	if (*str == 0)
		return (0);
	head_token = ft_tokenization(str);
	cur_token = head_token;
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
