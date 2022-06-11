/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/12 03:22:36 by kanghyki         ###   ########.fr       */
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

t_token	*ft_create_token_type_metachar(char **str)
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

void	ft_merge_string(char **dst, char *src)
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

void	ft_merge_environment(char **str, char **dst, char **env)
{
	char	*s_pos;
	char	*key;
	char	*value;

	++(*str);
	s_pos = *str;
	while (ft_strchr(SKIPCHAR, *(*str)) == 0)
		++(*str);
	key = ft_strndup(s_pos, *str - s_pos);
	value = ft_key_to_value(env, key);
	free(key);
	ft_merge_string(dst, value);
}

void	ft_quote(char **input_command, char **dst, char quote, char **env)
{
	char	*s_pos;
	char	*read_line;
	char	*for_free;

	s_pos = *input_command;
	while (*(*input_command) != 0)
	{
		if (*(*input_command) == quote)
		{
			ft_merge_string(dst, ft_strndup(s_pos, *input_command - s_pos));
			++(*input_command);
			if (*(*input_command) != 0)
				ft_merge_string(dst, ft_strdup(*input_command));
			return ;
		}
		else if (quote == '"' && *(*input_command) == '$')
			ft_merge_environment(input_command, dst, env);
		++(*input_command);
	}
	ft_merge_string(dst, ft_strndup(s_pos, *input_command - s_pos));
	for_free = *dst;
	*dst = ft_strjoin(*dst, "\n");
	free(for_free);
	read_line = readline("> ");
	for_free = read_line;
	ft_quote(&read_line, dst, quote, env);
	free(for_free);
}

t_token	*ft_create_token_type_argument(char **input_command, char **environment)
{
	char	*s_pos;
	char	*new_string;

	new_string = 0;
	s_pos = *input_command;
	while (*(*input_command) != 0)
	{
		s_pos = *input_command;
		while (ft_strchr(SKIPCHAR, *(*input_command)) == 0)
			++(*input_command);
		ft_merge_string(&new_string, ft_strndup(s_pos, *input_command - s_pos));
		if (ft_strchr_except_null(QUOTE, *(*input_command)) != 0)
		{
			++(*input_command);
			ft_quote(input_command, &new_string, *((*input_command) - 1), environment);
		}
		else if (*(*input_command) == '$')
			ft_merge_environment(input_command, &new_string, environment);
		else if (ft_strchr_except_null(WHITE_SPACE, *(*input_command)) != 0
				|| ft_strchr_except_null(METACHAR, *(*input_command)) != 0)
			break ;
	}
	return (ft_init_token(new_string, ARGUMENT));
}

/* Interface */
t_token	*ft_tokenizer(char *input_command, char **environment)
{
	t_token	*head;
	t_token	*new_token;

	head = 0;
	while (*input_command != 0)
	{
		while (ft_strchr_except_null(WHITE_SPACE, *input_command) != 0)
			++input_command;
		if (*input_command != 0)
		{
			if (ft_strchr_except_null(METACHAR, *input_command) != 0)
				new_token = ft_create_token_type_metachar(&input_command);
			else
				new_token = ft_create_token_type_argument(&input_command, environment);
			ft_add_token_back(&head, new_token);
		}
	}
	ft_add_token_back(&head, ft_init_token(0, EOL));
	return (head);
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
