/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/15 23:30:31 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_tokenizer(char *cmd_str, t_env_root *root_env)
{
	t_lexer	*lexer;
	t_token	*head;
	t_token	*new_token;

	head = 0;
	lexer = ft_init_lexer(cmd_str);
	while (ft_cur_char(lexer) != '\0')
	{
		while (ft_strchr_except_null(M_SPACE, ft_cur_char(lexer)) != 0)
			ft_read_lexer(lexer);
		if (ft_cur_char(lexer) != '\0')
		{
			if (ft_strchr_except_null(M_META, ft_cur_char(lexer)) != 0)
				new_token = ft_create_token_type_metachar(lexer);
			else
				new_token = ft_create_token_type_argument(lexer, root_env);
			ft_add_token_back(&head, new_token);
		}
	}
	ft_free_lexer(lexer);
	return (head);
}
