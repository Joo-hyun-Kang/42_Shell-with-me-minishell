#include "cmd.h"

int		ft_is_redir(enum e_token_type token)
{
	if (token == LT)
		return (true);
	else if (token == DLT)
		return (true);
	else if (token == GT)
		return (true);
	else if (token == DGT)
		return (true);
	return (false);
}

int	ft_find_next_pipe(t_argument **arg)
{
	t_argument *p;

	p = *arg;
	while (p != NULL)
	{
		if (p->next_token_type == PIPE)
			return (true);
		p = p->next;
	}
	return (false);
}

void	ft_free_redir_variable(t_redir *redir)
{
	free_arraylist(redir->list_arg);
	free_arraylist(redir->list_com);
	free_arraylist(redir->redir_arg);
}