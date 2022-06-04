/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/04 11:53:44 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"
#include <stdio.h>

/*
 * 문자열 하나씩 읽으면서 white space를 구분자로 순서대로 토큰 생성 후 리스트에 담음
 * ft_next_token이 해당 기능을 수행
 */

t_token_lst	*ft_new_token(enum e_token_type type, char *str)
{
	t_token_lst	*t;

	t = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (t == 0)
		return (0);
	t->t_type = type;
	t->str = str;
	t->next = 0;
	t->prev = 0;
	return (t);
}

void	ft_read_char(t_lexer_lst *l)
{
	if (l->r_pos >= ft_strlen(l->str))
		l->ch = 0;
	else
	{
		++l->r_pos;
		l->ch = l->str[l->r_pos];
	}
}

void	ft_skip_meanless_word(t_lexer_lst *l)
{
	while (ft_strchr(WHITE_SPACE, l->ch))
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

char	*ft_quote(char *str, t_lexer_lst *l, char quote)
{
	int			s_pos;
	int			flag = 0;

	ft_read_char(l);
	s_pos = l->r_pos;
	while (l->ch != 0)
	{
		if (l->ch == quote)
		{
			flag ^= 1;
			str = ft_strnjoin(str, &l->str[s_pos], l->r_pos - s_pos);
			ft_read_char(l);
			s_pos = l->r_pos;
		}
		else if (flag == 1 && l->ch == ' ')
		{
			str = ft_strnjoin(str, &l->str[s_pos], l->r_pos - s_pos);
			break ;
		}
		else
			ft_read_char(l);
	}
	ft_read_char(l);
	if (flag == 0)
	{
		str = ft_strnjoin(str, &l->str[s_pos], l->r_pos - s_pos);
		printf("quote> \n");
	}
	return (str);
}

char	*ft_read_argument(t_lexer_lst *l)
{
	int			s_pos;
	char		*rst;

	s_pos = l->r_pos;
	while (ft_strchr(TOKEN_SET, l->ch) == 0
		&& ft_strchr(WHITE_SPACE, l->ch) == 0
		&& ft_strchr(QUOTE, l->ch) == 0
		&& l->ch != 0)
		ft_read_char(l);
	rst = ft_strndup(&l->str[s_pos], l->r_pos - s_pos);
	if (ft_strchr(QUOTE, l->ch)) 
		rst = ft_quote(rst, l, l->ch);
	return (rst);
}

char	*ft_get_env(t_lexer_lst *l)
{
	ft_read_char(l);
	if (l->ch == 0 || l->ch == ' ')
		return (ft_strndup("$", 1));
	while (ft_strchr(TOKEN_SET, l->ch) == 0
		&& ft_strchr(WHITE_SPACE, l->ch) == 0
		&& ft_strchr(QUOTE, l->ch) == 0
		&& l->ch != 0)
		ft_read_char(l);
	return(ft_strndup("env_testing", 11));
}

t_token_lst *ft_create_next_token(t_lexer_lst *l, int *out_seperate)
{
	t_token_lst		*t;

	t = 0;
	ft_skip_meanless_word(l);
	if (ft_strchr(TOKEN_SET, l->ch))
	{
		if (l->ch == '|')
		{
			t = ft_new_token(PIPE, ft_strndup("|", 1));
			ft_read_char(l);
		}
		else if (l->ch == ';')
		{
			t = ft_new_token(SEMICOLON, ft_strndup(";", 1));
			ft_read_char(l);
		}
		else if (l->ch == '>')
		{
			ft_read_char(l);
			if (l->ch != '>')
				t = ft_new_token(LT, ft_strndup(">", 1));
			else
			{
				ft_read_char(l);
				t = ft_new_token(DLT, ft_strndup(">>", 2));
			}
		}
		else if (l->ch == '<')
		{
			ft_read_char(l);
			if (l->ch != '<')
				t = ft_new_token(GT, ft_strndup("<", 1));
			else
			{
				ft_read_char(l);
				t = ft_new_token(DGT, ft_strndup("<<", 2));
			}
		}
	}
	else if (l->ch == '$')
			t = ft_new_token(ARGUMENT, ft_get_env(l));
	else if (l->ch == '\'' || l->ch == '"')
		t = ft_new_token(ARGUMENT, ft_quote(0, l, l->ch));
	// If not exist before token or that token's meaning is endpoint assign command type to current token.
	else if (l->ch != 0)
	{
		if (*out_seperate == 1)
		{
			t = ft_new_token(COMMAND, ft_read_argument(l));
			*out_seperate = 0;
		}
		else
			t = ft_new_token(ARGUMENT, ft_read_argument(l));
	}
	else
		t = ft_new_token(EOL, 0);
	return (t);
}
