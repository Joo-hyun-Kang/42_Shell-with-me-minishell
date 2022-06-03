#include "parser.h"
#include <stdio.h>

const char	*token_type_arr[9] = {"COMMAND", "ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

int main(void)
{
	const int	test_case = 13;
	t_lexer_lst	*l[test_case];
	t_token_lst	*iter;

	printf("\n\n\n\n\n\n\n\n");
	// Lexer Test 1---------------------------------------
	l[0] = ft_init_lexer("echo hello");
	ft_analyze_string(l[0]);
	// Lexer Test 2---------------------------------------
	l[1] = ft_init_lexer("; - >a >>a <<a <a $ ?");
	ft_analyze_string(l[1]);
	// Lexer Test 3---------------------------------------
	l[2] = ft_init_lexer("ls;");
	ft_analyze_string(l[2]);
	// Lexer Test 3---------------------------------------
	l[3] = ft_init_lexer("echo askldj aksjdl \"As adsA\"S\"");
	ft_analyze_string(l[3]);
	// Lexer Test 4---------------------------------------
	l[4] = ft_init_lexer("echo hello | ls | echo what is your name?");
	ft_analyze_string(l[4]);
	// Lexer Test 5---------------------------------------
	l[5] = ft_init_lexer("echo -n hello;ls|echo \"what        is        your           name?\"");
	ft_analyze_string(l[5]);
	// Lexer Test 6---------------------------------------
	l[6] = ft_init_lexer("echo -nhello");
	ft_analyze_string(l[6]);
	// Lexer Test 7---------------------------------------
	l[7] = ft_init_lexer("echo -nhello\"hello my frie\"nd!!\"@@@");
	ft_analyze_string(l[7]);
	// Lexer Test 8---------------------------------------
	l[8] = ft_init_lexer("echo \"-nhello\"hello my friend!!\" @@@");
	ft_analyze_string(l[8]);
	// Lexer Test 9---------------------------------------
	l[9] = ft_init_lexer("echo $env_variable");
	ft_analyze_string(l[9]);
	// Lexer Test 10---------------------------------------
	l[10] = ft_init_lexer("echo askldj aksjdl \"As adsA\"S");
	ft_analyze_string(l[10]);
	// Lexer Test 11---------------------------------------
	l[11] = ft_init_lexer("echo \"zzzz''\"");
	ft_analyze_string(l[11]);
	// Lexer Test 10---------------------------------------
	l[12] = ft_init_lexer("echo 'zzzz\"\"'");
	ft_analyze_string(l[12]);
	
	
	for (int i = 0; i < test_case; i++)
	{
		printf("Lexer Test %d------------------------------------\n", i);
		printf("Test string >> \t%s\n", l[i]->str);
		iter = l[i]->head_token;
		while (iter->next != 0)
		{
			printf("{ tokenType: %-13s, tokenString: \"%s\" }\n", token_type_arr[iter->t_type], iter->str);
			iter = iter->next;
		}
		printf("{ tokenType: %-13s, tokenString: \"%s\" }\n", token_type_arr[iter->t_type], iter->str);
	}
	printf("\n\n\n\n\n\n\n\n");

	return (0);
}
