/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: kanghyki <kanghyki@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/06/03 15:55:58 by kanghyki          #+#    #+#             */
/*   Updated: 2022/06/04 02:13:12 by kanghyki         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "parser.h"
#include "token/token.h"

t_command	*ft_init_command(void)
{
	t_command	*c;

	c= (t_command *)malloc(sizeof(t_command));
	if (c== 0)
		return (0);
	c->cmd = 0;
	c->argument = 0;
	c->option = 0;
	return (c);
}

t_parser	*ft_init_parser(t_lexer_lst *l)
{
	t_parser	*p;

	p = (t_parser *)malloc(sizeof(t_parser));
	if (p == 0)
		return (0);
	p->l = l;
	p->cur_t = l->head_token;
	return (p);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

enum e_command	lookup_correct_command(t_token_lst *t)
{
	if (ft_strcmp("echo", t->str))
		return (ECHO);
	if (ft_strcmp("cd", t->str))
		return (CD);
	if (ft_strcmp("pwd", t->str))
		return (PWD);
	if (ft_strcmp("export", t->str))
		return (EXPORT);
	if (ft_strcmp("unset", t->str))
		return (UNSET);
	if (ft_strcmp("env", t->str))
		return (ENV);
	if (ft_strcmp("exit", t->str))
		return (EXIT);
	return (ILLEGAL);
}

t_token_lst	*ft_peek_token(t_parser *p)
{
	return (p->cur_t);
}

void	ft_read_token(t_parser *p)
{
	if (p->cur_t == 0)
		return ;
	p->cur_t = p->cur_t->next;
}

// Recursive-Top_Down-parser
t_command	*ft_recursive_top_down_parser(t_parser *p)
{
	t_command		*c;
	char			*tmp;

	c = ft_init_command();
	c->cmd = lookup_correct_command(ft_peek_token(p));
	if (c->cmd == ILLEGAL)
		return (0);
	ft_read_token(p);
	if (ft_strcmp("-n", ft_peek_token(p)->str))
	{
		c->option = "-n";
		ft_read_token(p);
	}
	if (ft_peek_token(p)->t_type != EOL
		|| ft_peek_token(p)->t_type != PIPE
		|| ft_peek_token(p)->t_type != SEMICOLON)
	{
		if (c->argument != 0)
			ft_strjoin(c->argument, " ");
		tmp = ft_peek_token(p)->str;
		ft_strjoin(c->argument, tmp);
		ft_read_token(p);
		return 0;
	}
	return (c);
}
