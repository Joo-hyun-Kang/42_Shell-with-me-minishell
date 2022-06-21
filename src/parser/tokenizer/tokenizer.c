/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/21 11:32:19 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_tokenizer(char *cmd_str, t_env_root *env)
{
	t_lexer	*lexer;
	t_token	*head;
	t_token	*new_token;

	head = 0;
	lexer = ft_init_lexer(cmd_str);
	while (ft_cur_char(lexer) != '\0')
	{
		while (ft_strchr_except_null(M_SPACE, ft_cur_char(lexer)) != NULL)
			ft_read_lexer(lexer);
		if (ft_cur_char(lexer) != '\0')
		{
			if (ft_strchr_except_null(M_META, ft_cur_char(lexer)) != NULL)
				new_token = ft_create_token_type_metachar(lexer);
			else
				new_token = ft_create_token_type_argument(lexer, env);
			if (new_token == NULL)
			{
				ft_free_token(head);
				return (NULL);
			}
			ft_add_token_back(&head, new_token);
		}
	}
	ft_add_token_back(&head, ft_init_token(NULL, EOL));
	ft_free_lexer(lexer);
	return (head);
}
