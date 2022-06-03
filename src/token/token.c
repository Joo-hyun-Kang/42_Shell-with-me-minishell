/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/02 20:30:32 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/03 03:02:48 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "token.h"

/*
 * 문자열 하나씩 읽으면서 white space를 구분자로 순서대로 토큰 생성 후 리스트에 담음
 * ft_next_token이 해당 기능을 수행
 */

t_token_lst	*ft_new_token(enum e_token_type type, char *str)
{
	t_token_lst	*token;

	token = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!token)
		return (0);
	token->t_type = type;
	token->token = str;
	token->next = 0;
	token->prev = 0;
	return (token);
}

void	ft_read_char(t_cmd_lst *cl)
{
	if (cl->r_pos >= ft_strlen(cl->cmd))
		cl->ch = 0;
	else
	{
		++cl->r_pos;
		cl->ch = cl->cmd[cl->r_pos];
	}
}

char	ft_peek_char(t_cmd_lst *cl)
{
	if (cl->r_pos >= ft_strlen(cl->cmd))
		return (0);
	return (cl->ch);
}

void	ft_skip_whitespace(t_cmd_lst *cl)
{
	while (cl->ch == '\t' || cl->ch == '\n' || cl->ch == '\v'
		|| cl->ch == '\f' || cl->ch == '\r' || cl->ch == ' ')
		ft_read_char(cl);
}

t_token_lst *ft_next_token(t_cmd_lst *cl)
{
	int				s_pos;
	t_token_lst		*token = 0;

	ft_skip_whitespace(cl);
	if (cl->ch == 0)
		return (0);
	if (ft_strchr("-|;'\"$/><?", cl->ch))
	{
		if (cl->ch == '-')
			token = ft_new_token(MINUS_SIGN, "-");
		else if (cl->ch == '|')
			token = ft_new_token(PIPE, "|");
		else if (cl->ch == ';')
			token = ft_new_token(SEMICOLON, ";");
		else if (cl->ch == '\'')
			token = ft_new_token(SINGLE_QUOTE, "'");
		else if (cl->ch == '"')
			token = ft_new_token(DOUBLE_QUOTE, "\"");
		else if (cl->ch == '$')
			token = ft_new_token(DOLLER_SIGN, "$");
		else if (cl->ch == '/')
			token = ft_new_token(SLASH, "/");
		else if (cl->ch == '?')
			token = ft_new_token(QUESTION_MAKR, "?");
		else if (cl->ch == '>')
		{
			ft_read_char(cl);
			if (ft_peek_char(cl) != '>')
				token = ft_new_token(LT, ">");
			else
				token = ft_new_token(DLT, ">>");
		}
		else if (cl->ch == '<')
		{
			ft_read_char(cl);
			if (ft_peek_char(cl) != '<')
				token = ft_new_token(GT, "<");
			else
				token = ft_new_token(DGT, "<<");
		}
		ft_read_char(cl);
	}
	else if (cl->head_token == 0)
	{
		s_pos = cl->r_pos;
		while (ft_is_letter(cl->ch))
			ft_read_char(cl);
		token = ft_new_token(FUNCTION, ft_strndup(&cl->cmd[s_pos], cl->r_pos - s_pos));
	}
	else
	{
		s_pos = cl->r_pos;
		while (ft_is_letter(cl->ch))
			ft_read_char(cl);
		token = ft_new_token(ARGUMENT, ft_strndup(&cl->cmd[s_pos], cl->r_pos - s_pos));
	}
	return (token);
}
