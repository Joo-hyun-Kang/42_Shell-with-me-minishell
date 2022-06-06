/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/06 15:55:52 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/minishell.h"

t_token_lst	*ft_new_token(enum e_token_type type, char *str)
{
	t_token_lst	*t;

	t = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (t == 0)
		return (0);
	t->token_type = type;
	t->pa_str_or_null = str;
	t->next = 0;
	return (t);
}

void	ft_read_char(t_lexer *l)
{
	if (l->read_pos >= ft_strlen(l->pa_str))
		l->ch = 0;
	else
	{
		++l->read_pos;
		l->ch = l->pa_str[l->read_pos];
	}
}

char	ft_lookup_char(t_lexer *l)
{
	if (l->read_pos >= ft_strlen(l->pa_str))
		return (0);
	return (l->pa_str[l->read_pos]);
}

char	ft_lookup_next_char(t_lexer *l)
{
	if (l->read_pos + 1 >= ft_strlen(l->pa_str))
		return (0);
	return (l->pa_str[l->read_pos + 1]);
}

void	ft_skip_whitespace(t_lexer *l)
{
	while (ft_strchr(" \r\v\f\n\t", ft_lookup_char(l)))
		ft_read_char(l);
}

char	*ft_strnjoin(char *dst, char *src, int n)
{
	char	*tmp;

	tmp = ft_strndup(src, n);
	dst = ft_strjoin(dst, tmp);
	free(tmp);
	return (dst);
}

// FIXME: Memory allocation
char	*ft_quote(char *str, t_lexer *l, char quote)
{
	int			start_pos;
	int			flag = 0;

	ft_read_char(l);
	start_pos = l->read_pos;
	while (ft_lookup_char(l) != 0)
	{
		if (flag == 1 && ft_lookup_char(l) == ' ')
		{
			str = ft_strnjoin(str, &l->pa_str[start_pos], l->read_pos - start_pos);
			break ;
		}
		else if (ft_lookup_char(l) == quote)
		{
			flag ^= 1;
			str = ft_strnjoin(str, &l->pa_str[start_pos], l->read_pos - start_pos);
			ft_read_char(l);
			start_pos = l->read_pos;
		}
		else
			ft_read_char(l);
	}
	if (ft_lookup_char(l) == 0)
		str = ft_strnjoin(str, &l->pa_str[start_pos], l->read_pos - start_pos);
	else
		ft_read_char(l);
	if (flag == 0)
		str = ft_strnjoin(str, &l->pa_str[start_pos], l->read_pos - start_pos);
	return (str);
}

char	*ft_read_argument(t_lexer *l)
{
	int			start_pos;
	char		*rst;

	start_pos = l->read_pos;
	while (ft_strchr(TOKEN_SEPARATOR, ft_lookup_char(l)) == 0
		&& ft_strchr(" \r\v\f\n\t", ft_lookup_char(l)) == 0
		&& ft_strchr("\"'", ft_lookup_char(l)) == 0
		&& ft_lookup_char(l) != 0)
		ft_read_char(l);
	rst = ft_strndup(&l->pa_str[start_pos], l->read_pos - start_pos);
	if (ft_strchr("\"'", ft_lookup_char(l))) 
		rst = ft_quote(rst, l, ft_lookup_char(l));
	return (rst);
}

char	*ft_get_env(t_lexer *l)
{
	(void)l;
	return(ft_strndup("env", 3));
}

t_token_lst *ft_create_next_token(t_lexer *l)
{
	t_token_lst		*t;

	t = 0;
	ft_skip_whitespace(l);
	if (ft_strchr(TOKEN_SEPARATOR, ft_lookup_char(l)))
	{
		if (ft_lookup_char(l) == '|')
			t = ft_new_token(PIPE, 0);
		else if (ft_lookup_char(l) == ';')
			t = ft_new_token(SEMICOLON, 0);
		else if (ft_lookup_char(l) == '>')
		{
			if (ft_lookup_next_char(l) != '>')
				t = ft_new_token(LT, 0);
			else
			{
				t = ft_new_token(DLT, 0);
				ft_read_char(l);
			}
		}
		else if (ft_lookup_char(l) == '<')
		{
			if (ft_lookup_next_char(l) != '<')
				t = ft_new_token(GT, 0);
			else
			{
				t = ft_new_token(DGT, 0);
				ft_read_char(l);
			}
		}
		ft_read_char(l);
	}
	else if (ft_lookup_char(l) == '$')
		t = ft_new_token(ARGUMENT, ft_get_env(l));
	else if (ft_lookup_char(l) != 0)
		t = ft_new_token(ARGUMENT, ft_read_argument(l));
	else
		t = ft_new_token(EOL, 0);
	return (t);
}
