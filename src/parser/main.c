#include "parser.h"
#include "token/token.h"
#include <stdio.h>

const char	*t_type_str_test[9] = {"COMMAND", "ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

void	delete_lexer_test(t_lexer_lst *l)
{
	t_token_lst	*iter;

	iter = l->head_token;
	while (iter)
	{
		if (iter->prev)
		{
			free(iter->prev->str);
			free(iter->prev);
		}
		if (iter->next == 0)
			free(iter);
		iter = iter->next;
	}
	free(l);
}

void	tokenization_test(t_lexer_lst *l)
{
	t_token_lst	*iter;
	iter = l->head_token;
	while (iter != 0)
	{
		printf("{ tokenType: %-13s, tokenString: \"%s\" }\n", t_type_str_test[iter->t_type], iter->str);
		iter = iter->next;
	}
	delete_lexer_test(l);
}

int main(void)
{
	t_lexer_lst	*l;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo hello");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("ls;");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"As adsA\"S\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo hello; | ls");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo A B C D");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo what  is  your  name");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"what  is  your  name\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo -n hello");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo -nhello");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"Double Quote\"and\"Double Quote\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"Double Quote\"and NormalString");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"Double Quote\"and \"UnclosedQuote");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'Single Quote'and'Single Quote'");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'Single Quote'and NormalString");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'Single Quote'and 'UnclosedQuote");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo $env_variable");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo $env_variable $env_variable2");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo $");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"zzzz''\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'zzzz\"\"'");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo ABC\"DEF\"'");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	tokenization_test(l);

	system("leaks a.out");
	
	return (0);
}
