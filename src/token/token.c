/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/03 14:34:22 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/*
 * 문자열 하나씩 읽으면서 white space를 구분자로 순서대로 토큰 생성 후 리스트에 담음
 * ft_next_token이 해당 기능을 수행
 */

t_token_lst	*ft_new_token(enum e_token_type type, char *str)
{
	t_token_lst	*t;

	t = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!t )
		return (0);
	t ->t_type = type;
	t ->token = str;
	t ->next = 0;
	t ->prev = 0;
	return (t );
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

char	ft_peek_char(t_lexer_lst *l)
{
	if (l->r_pos >= ft_strlen(l->str))
		return (0);
	return (l->ch);
}

void	ft_skip_whitespace(t_lexer_lst *l)
{
	while (l->ch == '\t' || l->ch == '\n' || l->ch == '\v'
		|| l->ch == '\f' || l->ch == '\r' || l->ch == ' ')
		ft_read_char(l);
}

// TODO: Refactoring
t_token_lst *ft_next_token(t_lexer_lst *l)
{
	int				s_pos;
	t_token_lst		*t;

	t = 0;
	ft_skip_whitespace(l);
	if (l->ch == 0)
		return (0);
	if (ft_strchr("-|;'\"$/?", l->ch))
	{
		if (l->ch == '-')
			t = ft_new_token(MINUS_SIGN, "-");
		else if (l->ch == '|')
			t = ft_new_token(PIPE, "|");
		else if (l->ch == ';')
			t = ft_new_token(SEMICOLON, ";");
		else if (l->ch == '\'')
			t = ft_new_token(SINGLE_QUOTE, "'");
		else if (l->ch == '"')
			t = ft_new_token(DOUBLE_QUOTE, "\"");
		else if (l->ch == '$')
			t = ft_new_token(DOLLER_SIGN, "$");
		else if (l->ch == '/')
			t = ft_new_token(SLASH, "/");
		else if (l->ch == '?')
			t = ft_new_token(QUESTION_MAKR, "?");
		ft_read_char(l);
	}
	else if (l->ch == '>')
	{
		ft_read_char(l);
		if (ft_peek_char(l) != '>')
			t = ft_new_token(LT, ">");
		else
		{
			ft_read_char(l);
			t = ft_new_token(DLT, ">>");
		}
	}
	else if (l->ch == '<')
	{
		ft_read_char(l);
		if (ft_peek_char(l) != '<')
			t = ft_new_token(GT, "<");
		else
		{
			ft_read_char(l);
			t = ft_new_token(DGT, "<<");
		}
	}
	else if (l->head_token == 0)
	{
		s_pos = l->r_pos;
		while (ft_is_letter(l->ch))
			ft_read_char(l);
		t = ft_new_token(FUNCTION, ft_strndup(&l->str[s_pos], l->r_pos - s_pos));
	}
	else
	{
		s_pos = l->r_pos;
		while (ft_is_letter(l->ch))
			ft_read_char(l);
		t = ft_new_token(ARGUMENT, ft_strndup(&l->str[s_pos], l->r_pos - s_pos));
	}
	return (t );
}
