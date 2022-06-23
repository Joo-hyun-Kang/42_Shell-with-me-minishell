/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:35:47 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/23 05:21:55 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_argument	*ft_init_argument(t_token *cur_tok, t_env_root *env)
{
	t_argument	*argument;

	argument = (t_argument *)malloc(sizeof(t_argument));
	if (argument == NULL)
		return (NULL);
	ft_memset(argument, 0, sizeof(t_argument));
	argument->pa_argument = ft_init_pa_argument(cur_tok);
	argument->env = env;
	return (argument);
}

char	**ft_init_pa_argument(t_token *cur_tok)
{
	char	**pa_argument;
	int		i;

	i = 0;
	while (cur_tok->next != NULL)
	{
		if (cur_tok->token_type != ARGUMENT)
			break ;
		++i;
		cur_tok = cur_tok->next;
	}
	pa_argument = (char **)malloc(sizeof(char *) * (i + 1));
	if (pa_argument == NULL)
		return (NULL);
	ft_memset(pa_argument, 0, sizeof(char *) * (i + 1));
	return (pa_argument);
}

void	ft_add_argument_back(t_argument **head, t_argument *arg)
{
	t_argument	*iter;

	if (*head == NULL)
		*head = arg;
	else
	{
		iter = *head;
		while (iter->next)
			iter = iter->next;
		iter->next = arg;
	}
}

void	ft_free_argument(t_argument *arg)
{
	t_argument	*prev;
	int			i;

	prev = arg;
	while (arg != NULL)
	{
		i = 0;
		while (arg->pa_argument[i] != NULL)
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

t_argument	*p_all_free(t_argument *head_arg, t_token *head_tok)
{
	ft_free_argument(head_arg);
	tk_free(head_tok);
	return (NULL);
}
