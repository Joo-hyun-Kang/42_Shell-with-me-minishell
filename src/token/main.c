#include <stdio.h>
#include "token.h"

int main(void)
{
	t_cmd_lst	*cl;
	t_token_lst	*iter;

	printf("echo hello\n");
	cl = ft_get_cmd_lst("echo hello");
	iter = cl->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: %s }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: %s }\n", iter->t_type, iter->token);

	// ---------------------------------------
	printf("; - > >> << < $ ?\n");
	t_cmd_lst	*cl2 = ft_get_cmd_lst("; - > >> << < $ ?");
	iter = cl2->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: %s }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: %s }\n", iter->t_type, iter->token);

	// ---------------------------------------
	printf("ls;\n");
	t_cmd_lst	*cl3 = ft_get_cmd_lst("ls;");
	iter = cl3->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: %s }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: %s }\n", iter->t_type, iter->token);
	return (0);
}
