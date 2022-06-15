/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 17:15:52 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_tokenizer(char *cmd_str, char **env)
{
	t_token	*head;
	t_token	*new_token;

	head = 0;
	while (*cmd_str != '\0')
	{
		while (ft_strchr_except_null(M_SPACE, *cmd_str) != 0)
			++cmd_str;
		if (*cmd_str != '\0')
		{
			if (ft_strchr_except_null(M_META, *cmd_str) != 0)
				new_token = ft_create_token_type_metachar(&cmd_str);
			else
				new_token = \
					ft_create_token_type_argument(&cmd_str, env);
			ft_add_token_back(&head, new_token);
		}
	}
	return (head);
}
