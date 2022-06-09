/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/09 04:01:52 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

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

t_token_lst	*ft_init_token(char *pa_str, enum e_token_type token_type)
{
	t_token_lst	*new_token;

	new_token = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (new_token == 0)
		return (0);
	ft_memset(new_token, 0, sizeof(t_token_lst));
	new_token->pa_str = pa_str;
	new_token->token_type = token_type;
	return (new_token);
}

void	ft_add_token(t_token_lst **head, t_token_lst *new_token)
{
	t_token_lst	*iter;

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

t_token_lst	*ft_create_token_meta_char(char **str)
{
	enum e_token_type	token_type;

	token_type = 0;
	if (*(*str) == ';')
		token_type = SEMICOLON;
	else if (*(*str) == '|')
		token_type = PIPE;
	else if (*(*str) == '<')
	{
		token_type = GT;
		if (*(*str + 1) == '<')
		{
			token_type = DGT;
			++(*str);
		}
	}
	else if (*(*str) == '>')
	{
		token_type = GT;
		if (*(*str + 1) == '>')
		{
			token_type = DGT;
			++(*str);
		}
	}
	++(*str);
	return (ft_init_token(0, token_type));
}

t_token_lst	*ft_create_token_argument(char **str)
{
	char	*s_pos;
	char	*duplicated;

	s_pos = *str;
	while (ft_strchr(WHITE_SPACE, *(*str)) == 0)
		++(*str);
	duplicated = ft_strndup(s_pos, *str - s_pos);
	return (ft_init_token(duplicated, ARGUMENT));
}

t_token_lst	*ft_tokenization(char *str)
{
	t_token_lst	*head;
	t_token_lst	*new_token;

	head = 0;
	while (*str != 0)
	{
		while (ft_strchr(WHITE_SPACE, *str) != 0)
			++str;
		if (ft_strchr(METACHAR, *str) != 0)
			new_token = ft_create_token_meta_char(&str);
		else if (*str != 0)
			new_token = ft_create_token_argument(&str);
		ft_add_token(&head, new_token);
	}
	ft_add_token(&head, ft_init_token(0, EOL));
	return (head);
}
