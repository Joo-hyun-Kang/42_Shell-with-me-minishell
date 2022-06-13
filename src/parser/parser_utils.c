/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:35:47 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/13 00:46:42 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

t_argument	*ft_init_argument(t_token *cur_token, char ***env)
{
	t_argument	*argument;

	argument = (t_argument *)malloc(sizeof(t_argument));
	if (argument == 0)
		return (0);
	ft_memset(argument, 0, sizeof(t_argument));
	argument->pa_argument = ft_init_pa_argument(cur_token);
	argument->env = env;
	return (argument);
}

char	**ft_init_pa_argument(t_token *cur_token)
{
	char	**pa_argument;
	int		i;

	i = 0;
	while (cur_token->next != 0)
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

void	ft_add_argument_back(t_argument **head, t_argument *arg)
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
