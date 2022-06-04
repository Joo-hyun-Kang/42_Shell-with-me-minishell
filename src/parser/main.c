#include "parser.h"
#include "token/token.h"
#include <stdio.h>

const char	*t_type_str_test[9] = {"COMMAND", "ARGUMENT", "PIPE", "SEMICOLON", "LT", "DLT", "GT", "DGT", "EOL"};

void	delete_lexer_test(t_lexer_lst *l)
{
	t_token_lst	*iter;

	iter = l->head_token;
	while (iter->next)
	{
		if (iter->prev)
		{
			free(iter->prev->str);
			free(iter->prev);
		}
		iter = iter->next;
	}
	free(iter);
	free(l);
}

void	print_token_test(t_lexer_lst *l)
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

void	tokenization_test(void)
{
	t_lexer_lst	*l;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo hello");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("ls;");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"As adsA\"S\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo hello; | ls");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo A B C D");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo what  is  your  name");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"what  is  your  name\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo -n hello");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo -nhello");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"Double Quote\"and\"Double Quote\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"Double Quote\"and NormalString");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"Double Quote\"and \"UnclosedQuote");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'Single Quote'and'Single Quote'");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'Single Quote'and NormalString");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'Single Quote'and 'UnclosedQuote");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo $env_variable");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo $env_variable $env_variable2");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo $");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"zzzz''\"");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo 'zzzz\"\"'");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo ABC\"DEF\"'");
	printf("%s\n", l->str);
	ft_analyze_string(l);
	print_token_test(l);
}

void	add_expected_token(t_lexer_lst *l, enum e_token_type t_type, char *str)
{
	t_token_lst	*t;
	t_token_lst	*iter;

	t = (t_token_lst *)malloc(sizeof(t_token_lst));
	if (!t)
		return ;
	t->t_type = t_type;
	t->str = str;
	t->next = 0;
	t->prev = 0;
	if (!l->head_token)
	{
		l->head_token = t;
		return ;
	}
	iter = l->head_token;
	while (iter->next)
		iter = iter->next;
	iter->next = t;
	return ;
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	if (!s1 && !s2)
		return (0);
	while (s1[i] && s2[i])
	{
		if (s1[i] != s2[i])
			break ;
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

void	compare_tokens(t_lexer_lst *l, t_lexer_lst *expected)
{
	int			i = 0;
	t_token_lst	*iter;
	t_token_lst	*expected_iter;

	printf("\033[0;32m>> %s\033[0m\n", l->str);
	iter = l->head_token;
	expected_iter = expected->head_token;
	printf("%3c%10s%3c%20s%3c%10s%3c%20s%3c\n", ' ', "Return", ' ', "", ' ', "Origin", ' ', "", ' ');
	printf("%3c%10s%3c%20s%3c%10s%3c%20s%3c\n", ' ', "[tokenType]", ' ', "\"tokenStr\"", ' ', "[tokenType]", ' ', "\"tokenStr\"", ' ');
	while (iter != 0 && expected_iter != 0)
	{
		printf("%-3d%10s%3c\"%20s\"%3c%10s%3c\"%20s\"%3c\n", i + 1, t_type_str_test[iter->t_type], ' ', iter->str, ' ', t_type_str_test[expected_iter->t_type], ' ', expected_iter->str, ' ');
		if (ft_strcmp(iter->str, expected_iter->str) != 0
			|| iter->t_type != expected_iter->t_type)
		{
			printf("ERROR!!!!!!!\n");
			return ;
		}
		iter = iter->next;
		expected_iter = expected_iter->next;
		++i;
	}
	printf("\n\n");
	delete_lexer_test(l);
	delete_lexer_test(expected);
}

char	*ft_strdup(const char *s)
{
	size_t	i;
	char	*s_dup;

	i = 0;
	s_dup = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1));
	if (!s_dup)
		return (0);
	while (s[i])
	{
		s_dup[i] = s[i];
		++i;
	}
	s_dup[i] = 0;
	return (s_dup);
}

t_lexer_lst	*init_lexer_test(void)
{
	t_lexer_lst	*l;

	l = (t_lexer_lst *)malloc(sizeof(t_lexer_lst));
	if (!l)
		return (0);
	l->str = 0;
	l->r_pos = 0;
	l->ch = 0;
	l->head_token = 0;
	return (l);
}

//	- [echo \\t\\t sdjklqw]           -> [		sdjklqw] + \n
//	- [echo        asd]               -> [asd] + \n
//	- [echo " asd "]                  -> [ asd ] + \n
//	- [echo askldj aksjdl "As adsA"S] -> [askldj aksjdl As adsAS] + \n
//	- [echo \" sdasd \"]              -> [" sdasd "]
//	- [echo          " asd a asd"112] -> [ asd a asd112] + \n
//	- ["echo" asdf]                   -> [asdf] + \n
//	- ["echo " asdf]                  -> [command not found: echo ] + \n
//	- [echo "-n"asdf]                 -> [-nasdf]
//	- ["echo"123]			  -> [command not found: echo123] + \n
void	dirty_case_echo(void)
{
	t_lexer_lst	*l;
	t_lexer_lst	*expected;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \\t\\t sdjklqw");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup("\\t\\t"));
	add_expected_token(expected, ARGUMENT, ft_strdup("sdjklqw"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo        asd");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup("asd"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \" asd \"");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup(" asd "));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo askldj aksjdl \"As adsA\"S");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup("askldj"));
	add_expected_token(expected, ARGUMENT, ft_strdup("aksjdl"));
	add_expected_token(expected, ARGUMENT, ft_strdup("As adsAS"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \" sdasd \"");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup(" sdasd "));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo          \" asd a asd\"112");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup(" asd a asd112"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("\"echo\" asdf");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup("asdf"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("\"echo \" asdf");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo "));
	add_expected_token(expected, ARGUMENT, ft_strdup("asdf"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	
	// Lexer Test---------------------------------------
	l = ft_init_lexer("echo \"-n\"asdf");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo"));
	add_expected_token(expected, ARGUMENT, ft_strdup("-nasdf"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("\"echo\"123");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("echo123"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
}

//	ls -l;ls
//	ls -l;;ls
//	` ls -l ; ls`
//	’;’
//	’;;’
//	’; ;’
void	dirty_case_semicolon(void)
{
	t_lexer_lst	*l;
	t_lexer_lst	*expected;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("ls -l;ls");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("ls"));
	add_expected_token(expected, ARGUMENT, ft_strdup("-l"));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup("ls"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("ls -l;;ls");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("ls"));
	add_expected_token(expected, ARGUMENT, ft_strdup("-l"));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup("ls"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("` ls -l ; ls`");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup(" ls -l ; ls"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer(";");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer(";;");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("; ;");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup(""));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
}

//	cat '
//	cat ''
//	cat '$PATH'
//	cat ' '
//	cat ';'
//	"...;..."
void	dirty_case_cat(void)
{
	t_lexer_lst	*l;
	t_lexer_lst	*expected;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat '");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup(""));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat ''");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup(""));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat $PATH");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup("env_testing"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat ' '");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup(" "));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat ';'");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup(";"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("...;...");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("..."));
	add_expected_token(expected, SEMICOLON, ft_strdup(";"));
	add_expected_token(expected, COMMAND, ft_strdup("..."));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
}

//	cat tmp.c > test.txt
//	cat < test.txt
void	dirty_case_redirection(void)
{
	t_lexer_lst	*l;
	t_lexer_lst	*expected;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat tmp.c > test.txt");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup("tmp.c"));
	add_expected_token(expected, LT, ft_strdup(">"));
	add_expected_token(expected, ARGUMENT, ft_strdup("test.txt"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat < test.txt");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, GT, ft_strdup(">"));
	add_expected_token(expected, ARGUMENT, ft_strdup("test.txt"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
}


//	cat test | grep int | more
//	ls filethatdoesntexist | grep bla | more
//	ls | grep .c | grep cmd > test
void	dirty_case_pipe(void)
{
	t_lexer_lst	*l;
	t_lexer_lst	*expected;

	// Lexer Test---------------------------------------
	l = ft_init_lexer("cat test | grep int | more");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("cat"));
	add_expected_token(expected, ARGUMENT, ft_strdup("test"));
	add_expected_token(expected, PIPE, ft_strdup("|"));
	add_expected_token(expected, COMMAND, ft_strdup("grep"));
	add_expected_token(expected, ARGUMENT, ft_strdup("int"));
	add_expected_token(expected, PIPE, ft_strdup("|"));
	add_expected_token(expected, COMMAND, ft_strdup("more"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("ls filethatdoesntexist | grep bla | more");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("ls"));
	add_expected_token(expected, ARGUMENT, ft_strdup("filethatdoesntexist"));
	add_expected_token(expected, PIPE, ft_strdup("|"));
	add_expected_token(expected, COMMAND, ft_strdup("grep"));
	add_expected_token(expected, ARGUMENT, ft_strdup("bla"));
	add_expected_token(expected, PIPE, ft_strdup("|"));
	add_expected_token(expected, COMMAND, ft_strdup("more"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
	// Lexer Test---------------------------------------
	l = ft_init_lexer("ls | grep .c | grep cmd > test");
	expected = init_lexer_test();
	add_expected_token(expected, COMMAND, ft_strdup("ls"));
	add_expected_token(expected, PIPE, ft_strdup("|"));
	add_expected_token(expected, COMMAND, ft_strdup("grep"));
	add_expected_token(expected, ARGUMENT, ft_strdup(".c"));
	add_expected_token(expected, PIPE, ft_strdup("|"));
	add_expected_token(expected, COMMAND, ft_strdup("grep"));
	add_expected_token(expected, ARGUMENT, ft_strdup("cmd"));
	add_expected_token(expected, LT, ft_strdup(">"));
	add_expected_token(expected, ARGUMENT, ft_strdup("test"));
	add_expected_token(expected, EOL, 0);
	ft_analyze_string(l);
	compare_tokens(l, expected);
}

int main(void)
{
	tokenization_test();
	dirty_case_echo();
	dirty_case_semicolon();
	dirty_case_cat();
	dirty_case_redirection();
	dirty_case_pipe();
	// system("leaks a.out");
	
	return (0);
}
