#include <stdio.h>
#include "token.h"

int main(void)
{
	t_token_lst	*iter;

	// Lexer Test 1---------------------------------------
	printf("Test string \"echo hello\"\n");
	t_lexer_lst	*l = ft_init_lexer("echo hello");
	ft_set_lexer(l);
	iter = l->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);

	// Lexer Test 2---------------------------------------
	printf("Test string \"; - >a >>a <<a <a $ ?\"\n");
	t_lexer_lst	*l2 = ft_init_lexer("; - >a >>a <<a <a $ ?");
	ft_set_lexer(l2);
	iter = l2->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);

	// Lexer Test 3---------------------------------------
	printf("Test string \"ls;\"\n");
	t_lexer_lst	*l3 = ft_init_lexer("ls;");
	ft_set_lexer(l3);
	iter = l3->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);
	// Lexer Test 3---------------------------------------
	printf("Test string \"echo askldj aksjdl \"As adsA\"S\"\n");
	t_lexer_lst	*l4 = ft_init_lexer("echo askldj aksjdl \"As adsA\"S\"");
	ft_set_lexer(l4);
	iter = l4->head_token;
	while (iter->next != 0)
	{
		printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);
		iter = iter->next;
	}
	printf("{ tokenType: %d, token: \"%s\" }\n", iter->t_type, iter->token);
	

	return (0);
}
