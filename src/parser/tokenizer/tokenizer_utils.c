/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/12 04:03:15 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/13 00:47:34 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*ft_strchr_except_null(const char *str, int c)
{
	while (*str != 0)
	{
		if (*str == (char)c)
			return ((char *)str);
		++str;
	}
	return (0);
}

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	size_t	src_size;
	char	*dup;

	i = 0;
	src_size = ft_strlen(src);
	if (src_size < n)
		n = src_size;
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (dup == 0)
		return (0);
	while (i < n)
	{
		dup[i] = src[i];
		++i;
	}
	dup[i] = 0;
	return (dup);
}

t_token	*ft_init_token(char *str, enum e_token_type token_type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token == 0)
		return (0);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->pa_str = str;
	new_token->token_type = token_type;
	return (new_token);
}

void	ft_add_token_back(t_token **head, t_token *new_token)
{
	t_token	*iter;

	if (*head == 0)
		*head = new_token;
	else
	{
		iter = *head;
		while (iter->next != 0)
			iter = iter->next;
		iter->next = new_token;
	}
}
