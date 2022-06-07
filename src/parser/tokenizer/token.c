/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/07 17:13:47 by kanghyki         ###   ########.fr       */
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

char	*gnl_strjoin(char *s1, char *s2)
{
	int		index;
	char	*str;
	char	*loc_s1;

	index = 0;
	loc_s1 = s1;
	if (!s1 && !s2)
		return (0);
	else if (!s1 || !s2)
	{
		if (!s1)
			return (ft_strndup(s2, ft_strlen(s2)));
		else
			return (ft_strndup(s1, ft_strlen(s1)));
	}
	str = (char *)malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (0);
	while (*s1)
		str[index++] = *(s1++);
	while (*s2)
		str[index++] = *(s2++);
	str[index] = 0;
	free(loc_s1);
	return (str);
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

void	ft_read_char(t_lexer *out_lexer)
{
	if (out_lexer->read_pos < ft_strlen(out_lexer->pa_str))
		out_lexer->read_pos += 1;
}

char	ft_get_char(t_lexer *out_lexer)
{
	return (out_lexer->pa_str[out_lexer->read_pos]);
}

char	ft_peek_char(t_lexer *out_lexer)
{
	if (out_lexer->read_pos < ft_strlen(out_lexer->pa_str))
		return (out_lexer->pa_str[out_lexer->read_pos + 1]);
	return (0);
}

void	ft_skip_space(t_lexer *out_lexer)
{
	while (ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer)) != 0)
		ft_read_char(out_lexer);
}

int	ft_get_pos(t_lexer *lexer)
{
	return (lexer->read_pos);
}

void	ft_add_token(t_lexer *out_lexer, t_token_lst *new_token)
{
	t_token_lst	*iter;

	if (out_lexer->head == 0)
		out_lexer->head = new_token;
	else
	{
		iter = out_lexer->head;
		while (iter->next != 0)
			iter = iter->next;
		iter->next = new_token;
	}
}

void	ft_tokenization_reserved(t_lexer *out_lexer)
{
	enum e_token_type	token_type;

	token_type = 0;
	if (ft_get_char(out_lexer) == ';')
		token_type = SEMICOLON;
	else if (ft_get_char(out_lexer) == '|')
		token_type = PIPE;
	else if (ft_get_char(out_lexer) == '<' && ft_peek_char(out_lexer) == '<')
	{
		token_type = DGT;
		ft_read_char(out_lexer);
	}
	else if (ft_get_char(out_lexer) == '<')
		token_type = GT;
	else if (ft_get_char(out_lexer) == '>' && ft_peek_char(out_lexer) == '>')
	{
		token_type = DLT;
		ft_read_char(out_lexer);
	}
	else if (ft_get_char(out_lexer) == '>')
		token_type = LT;
	ft_add_token(out_lexer, ft_init_token(0, token_type));
	ft_read_char(out_lexer);
}

char	*ft_dquote(t_lexer *out_lexer, char *pa_str)
{
	int		start_pos;
	int		is_closed;
	char	*tmp;

	is_closed = 0;
	ft_read_char(out_lexer);
	start_pos = ft_get_pos(out_lexer);
	while (ft_get_char(out_lexer) != 0)
	{
		if (is_closed == 1 && ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer) != 0))
			break ;
		if (ft_get_char(out_lexer) == '"')
		{
			is_closed ^= 1;
			tmp = ft_strndup(&out_lexer->pa_str[start_pos], out_lexer->read_pos - start_pos);
			pa_str = gnl_strjoin(pa_str, tmp);
			free(tmp);
			ft_read_char(out_lexer);
			start_pos = ft_get_pos(out_lexer);
		}
		else if (ft_get_char(out_lexer) == '$')
		{
			tmp = ft_get_env(out_lexer);
			pa_str = gnl_strjoin(pa_str, tmp);
			free(tmp);
			start_pos = ft_get_pos(out_lexer);
		}
		else
			ft_read_char(out_lexer);
	}
	if (is_closed == 1 && ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer) != 0))
	{
		tmp = ft_strndup(&out_lexer->pa_str[start_pos], out_lexer->read_pos - start_pos);
		pa_str = gnl_strjoin(pa_str, tmp);
		free(tmp);
		ft_read_char(out_lexer);
		return (pa_str);
	}
	if (is_closed == 0)
		printf("dquote>\n");
	return (pa_str);
}

char	*ft_squote(t_lexer *out_lexer, char *pa_str)
{
	int		start_pos;
	int		is_closed;
	char	*tmp;

	is_closed = 0;
	ft_read_char(out_lexer);
	start_pos = ft_get_pos(out_lexer);
	while (ft_get_char(out_lexer) != 0)
	{
		if (is_closed == 1 && ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer) != 0))
			break ;
		if (ft_get_char(out_lexer) == '\'')
		{
			is_closed ^= 1;
			tmp = ft_strndup(&out_lexer->pa_str[start_pos], out_lexer->read_pos - start_pos);
			pa_str = gnl_strjoin(pa_str, tmp);
			free(tmp);
			ft_read_char(out_lexer);
			start_pos = ft_get_pos(out_lexer);
		}
		else
			ft_read_char(out_lexer);
	}
	if (is_closed == 1 && ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer) != 0))
	{
		tmp = ft_strndup(&out_lexer->pa_str[start_pos], out_lexer->read_pos - start_pos);
		pa_str = gnl_strjoin(pa_str, tmp);
		free(tmp);
		ft_read_char(out_lexer);
		return (pa_str);
	}
	if (is_closed == 0)
		printf("dquote>\n");
	return (pa_str);
}

void	ft_tokenization_argument(t_lexer *out_lexer)
{
	int		start_pos;
	char	*pa_dup_str;

	pa_dup_str = 0;
	start_pos = ft_get_pos(out_lexer);
	while (ft_strchr(TOKEN_RESERVED, ft_get_char(out_lexer)) == 0
			&& ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer)) == 0
			&& ft_strchr(TOKEN_QUOTE, ft_get_char(out_lexer)) == 0)
		ft_read_char(out_lexer);
	if (ft_get_pos(out_lexer) - start_pos != 0)
		pa_dup_str = ft_strndup(&out_lexer->pa_str[start_pos], out_lexer->read_pos - start_pos);
	if (ft_get_char(out_lexer) == '"')
		pa_dup_str = ft_dquote(out_lexer, pa_dup_str);
	else if (ft_get_char(out_lexer) == '\'')
		pa_dup_str = ft_squote(out_lexer, pa_dup_str);
	else if (ft_get_char(out_lexer) == '$')
		pa_dup_str = ft_get_env(out_lexer);
	ft_read_char(out_lexer);
	ft_add_token(out_lexer, ft_init_token(pa_dup_str, ARGUMENT));
}

char	*ft_get_env(t_lexer *out_lexer)
{
	int		start_pos;
	// char	*env;
	// char	*env_key;

	ft_read_char(out_lexer);
	start_pos = ft_get_pos(out_lexer);
	while (ft_strchr(TOKEN_RESERVED, ft_get_char(out_lexer)) == 0
			&& ft_strchr(TOKEN_SPACE, ft_get_char(out_lexer)) == 0
			&& ft_strchr(TOKEN_QUOTE, ft_get_char(out_lexer)) == 0)
		ft_read_char(out_lexer);
	/*
	 * env = getenv(env_key);
	 * free(env_key);
	 * ft_add_token(out_lexer, ft_init_token(env, ARGUMENT));
	 */
	printf("%s\n", &out_lexer->pa_str[out_lexer->read_pos]);
	if (ft_get_pos(out_lexer) - start_pos != 0)
		return (ft_strndup(&out_lexer->pa_str[start_pos], out_lexer->read_pos - start_pos));
	else
		return (ft_strndup("$", 1));
}

void	ft_tokenization(t_lexer *out_lexer)
{
	while (ft_get_char(out_lexer) != 0)
	{
		ft_skip_space(out_lexer);
		if (ft_strchr(TOKEN_RESERVED, ft_get_char(out_lexer)) != 0)
			ft_tokenization_reserved(out_lexer);
		else if (ft_get_char(out_lexer) != 0)
			ft_tokenization_argument(out_lexer);
	}
	ft_add_token(out_lexer, ft_init_token(0, EOL));
}
