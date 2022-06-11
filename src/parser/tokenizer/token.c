/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/11 20:28:30 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

char	*ft_strchr_except_null(const char *str, int c)
{
	while (*str)
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

t_token	*ft_init_token(char *pa_str, enum e_token_type token_type)
{
	t_token	*new_token;

	new_token = (t_token *)malloc(sizeof(t_token));
	if (new_token == 0)
		return (0);
	ft_memset(new_token, 0, sizeof(t_token));
	new_token->pa_str = pa_str;
	new_token->token_type = token_type;
	return (new_token);
}

void	ft_add_token(t_token **head, t_token *new_token)
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

t_token	*ft_create_token_meta_char(char **str)
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
		token_type = LT;
		if (*(*str + 1) == '>')
		{
			token_type = DLT;
			++(*str);
		}
	}
	++(*str);
	return (ft_init_token(0, token_type));
}

void	ft_combine_str(char **dst, char *src)
{
	char	*new_str;

	if (*dst == 0)
		*dst = src;
	else
	{
		new_str = ft_strjoin(*dst, src);
		free(*dst);
		free(src);
		*dst = new_str;
	}
}

void	ft_replace_str_with_env(char **str, char **dst, char **env)
{
	char	*s_pos;
	char	*key;
	char	*value;

	++(*str);
	s_pos = *str;
	while (*(*str) != 0
			&& ft_strchr_except_null(WHITE_SPACE, *(*str)) == 0
			&& ft_strchr_except_null(METACHAR, *(*str)) == 0
			&& ft_strchr_except_null(QUOTE, *(*str)) == 0)
		++(*str);
	key = ft_strndup(s_pos, *str - s_pos);
	value = ft_key_to_value(env, key);
	free(key);
	ft_combine_str(dst, value);
}

void	ft_quote(char **str, char **dst, char quote, char **env)
{
	char	*s_pos;
	char	*read_line;
	char	*for_free;

	s_pos = *str;
	while (*(*str) != 0)
	{
		if (*(*str) == quote)
		{
			ft_combine_str(dst, ft_strndup(s_pos, *str - s_pos));
			++(*str);
			return ;
		}
		else if (*(*str) == '$')
			ft_replace_str_with_env(str, dst, env);
		++(*str);
	}
	ft_combine_str(dst, ft_strndup(s_pos, *str - s_pos));
	for_free = *dst;
	*dst = ft_strjoin(*dst, "\n");
	free(for_free);
	read_line = readline("> ");
	for_free = read_line;
	ft_quote(&read_line, dst, quote, env);
	free(for_free);
}

t_token	*ft_create_token_argument(char **str, char **env)
{
	char	*s_pos;
	char	*pa_str;
	char	quote;

	s_pos = *str;
	pa_str = 0;
	while (*(*str) != 0)
	{
		s_pos = *str;
		while (*(*str) != 0
				&& ft_strchr_except_null(WHITE_SPACE, *(*str)) == 0
				&& ft_strchr_except_null(METACHAR, *(*str)) == 0
				&& ft_strchr_except_null(QUOTE, *(*str)) == 0
				&& *(*str) != '$')
			++(*str);
		ft_combine_str(&pa_str, ft_strndup(s_pos, *str - s_pos));
		if (ft_strchr_except_null(QUOTE, *(*str)) != 0)
		{
			quote = *(*str);
			++(*str);
			ft_quote(str, &pa_str, quote, env);
		}
		else if (*(*str) == '$')
			ft_replace_str_with_env(str, &pa_str, env);
		else if (ft_strchr_except_null(WHITE_SPACE, *(*str)) != 0
				|| ft_strchr_except_null(METACHAR, *(*str)) != 0)
			break ;
	}
	return (ft_init_token(pa_str, ARGUMENT));
}

void	ft_print_token_test(t_token *head)
{
	const char	*t_type_str_test[8] = {"ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

	while (head)
	{
		printf("{token type:%s, token value:%s\n", t_type_str_test[head->token_type], head->pa_str);
		head = head->next;
	}
}

t_token	*ft_tokenization(char *str, char **env)
{
	t_token	*head;
	t_token	*new_token;

	head = 0;
	while (*str != 0)
	{
		while (ft_strchr_except_null(WHITE_SPACE, *str) != 0)
			++str;
		if (*str != 0)
		{
			if (ft_strchr_except_null(METACHAR, *str) != 0)
				new_token = ft_create_token_meta_char(&str);
			else
				new_token = ft_create_token_argument(&str, env);
			ft_add_token(&head, new_token);
		}
	}
	ft_add_token(&head, ft_init_token(0, EOL));
	// ft_print_token_test(head);
	return (head);
}
