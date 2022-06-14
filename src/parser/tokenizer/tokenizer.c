/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenizer.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/14 21:34:56 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token	*ft_create_token_type_metachar(char **str)
{
	enum e_token_type	token_type;

	token_type = -1;
	if (*(*str) == ';')
		token_type = SEMICOLON;
	else if (*(*str) == '|')
		token_type = PIPE;
	else if (*(*str) == '<')
	{
		token_type = GT;
		if (*(*str + 1) == '<')
			token_type = DGT;
	}
	else if (*(*str) == '>')
	{
		token_type = LT;
		if (*(*str + 1) == '>')
			token_type = DLT;
	}
	if (token_type == DLT || token_type == DGT)
		++(*str);
	++(*str);
	return (ft_init_token(NULL, token_type));
}

void	ft_merge_environment(char **str, char **dst, char **env)
{
	char	*s_pos;
	char	*key;
	char	*value;

	++(*str);
	s_pos = *str;
	while (ft_strchr(SKIPCHAR, *(*str)) == 0 && *(*str) != '\n')
		++(*str);
	key = ft_strndup(s_pos, *str - s_pos);
	value = ft_get_value_from_env(env, key);
	free(key);
	ft_merge_string(dst, value);
}

void	ft_quote(char **input_command, char **dst, char quote, char **env)
{
	char	*s_pos;
	char	*read_line;
	char	*tmp;

	s_pos = *input_command;
	while (*(*input_command) != '\0')
	{
		if (*(*input_command) == quote)
		{
			ft_merge_string(dst, ft_strndup(s_pos, *input_command - s_pos));
			if (*(++(*input_command)) != '\0')
				ft_merge_string(dst, ft_strdup(*input_command));
			return ;
		}
		else if (quote == '"' && *(*input_command) == '$')
			ft_merge_environment(input_command, dst, env);
		++(*input_command);
	}
	tmp = ft_strndup(s_pos, *input_command - s_pos);
	ft_merge_string(dst, ft_strjoin(tmp, "\n"));
	free(tmp);
	read_line = readline("> ");
	tmp = read_line;
	ft_quote(&read_line, dst, quote, env);
	free(tmp);
}

t_token	*ft_create_token_type_argument(char **input_command, char **environment)
{
	char	*s_pos;
	char	*new_string;
	char	quote;

	new_string = 0;
	s_pos = *input_command;
	while (*(*input_command) != '\0')
	{
		s_pos = *input_command;
		while (ft_strchr(SKIPCHAR, *(*input_command)) == 0)
			++(*input_command);
		ft_merge_string(&new_string, ft_strndup(s_pos, *input_command - s_pos));
		if (ft_strchr_except_null(QUOTE, *(*input_command)) != 0)
		{
			quote = *(*input_command);
			++(*input_command);
			ft_quote(input_command, &new_string, quote, environment);
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
	while (*input_command != '\0')
	{
		while (ft_strchr_except_null(WHITE_SPACE, *input_command) != 0)
			++input_command;
		if (*input_command != '\0')
		{
			if (ft_strchr_except_null(METACHAR, *input_command) != 0)
				new_token = ft_create_token_type_metachar(&input_command);
			else
				new_token = \
					ft_create_token_type_argument(&input_command, environment);
			ft_add_token_back(&head, new_token);
		}
	}
	ft_add_token_back(&head, ft_init_token(NULL, EOL));
	return (head);
}
