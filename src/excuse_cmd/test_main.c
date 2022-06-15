#include "cmd.h"

// int	main(int argc, char **argv, char **environ)
// {
// 	//Test Code
// 	t_argument *pa_arg;
// 	t_argument *p;
	
// 	/*
// 	//echo
// 	{
// 		printf("Echo TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = SEMICOLON;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("echo");
// 		p->argument->pa_argument[1] = ft_strdup("1234");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = SEMICOLON;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 3 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("echo");
// 		p->argument->pa_argument[1] = ft_strdup("-n");
// 		p->argument->pa_argument[2] = ft_strdup("5678");
// 		p->argument->pa_argument[3] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = SEMICOLON;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("echo");
// 		p->argument->pa_argument[1] = ft_strdup("-n");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = SEMICOLON;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("echo");
// 		p->argument->pa_argument[1] = NULL;

// 		ft_system(pa_arg);

// 		printf("\n");
// 	}

// 	//Pwd
// 	{
// 		printf("Pwd TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = ft_strdup("213213");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 3 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = ft_strdup("213213");
// 		p->argument->pa_argument[2] = ft_strdup("213213");
// 		p->argument->pa_argument[3] = NULL;

// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("\n");
// 	}

// 	//Cd
// 	{
// 		printf("Cd TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("cd");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("cd");
// 		p->argument->pa_argument[1] = ft_strdup("bin");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("cd");
// 		p->argument->pa_argument[1] = ft_strdup("~");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 1 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 2 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("cd");
// 		p->argument->pa_argument[1] = ft_strdup("2131313");
// 		p->argument->pa_argument[2] = NULL;
// 		p->next = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * 3 + 1);
// 		p->argument->pa_argument[0] = ft_strdup("cd");
// 		p->argument->pa_argument[1] = ft_strdup("342424");
// 		p->argument->pa_argument[2] = ft_strdup("32131");
// 		p->argument->pa_argument[3] = NULL;

// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("\n");
// 	}

	
// 	//execuse
// 	{
// 		printf("execuse TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("ls");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("cd");
// 		p->argument->pa_argument[1] = ft_strdup("../");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("pwd");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("./a.out");
// 		p->argument->pa_argument[1] = NULL;

// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("ls");
// 		p->argument->pa_argument[1] = ft_strdup("a");
// 		p->argument->pa_argument[2] = NULL;

// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("END!\n");
// 	}
// 	*/

	
// 	{
// 		printf("execuse TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("../main");
// 		p->argument->pa_argument[1] = NULL;
		
// 		p->next = NULL;

// 		ft_system(pa_arg);

// 		printf("END!\n");
// 	}

// 	{
// 		printf("MULT PIPE TEST\n");

// 		pa_arg = (t_argument *)malloc(sizeof(t_argument));
// 		p = pa_arg;

// 		printf("my output is \n");

// 		p->next_token_type = PIPE;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("sort");
// 		p->argument->pa_argument[1] = NULL;
		
// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = PIPE;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("ls");
// 		p->argument->pa_argument[1] = NULL;
		
// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = PIPE;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("sort");
// 		p->argument->pa_argument[1] = NULL;
		
// 		p->next = (t_argument *)malloc(sizeof(t_argument));
// 		p = p->next;
// 		p->next_token_type = EOL;
// 		p->argument->pa_argument = (char **)malloc(sizeof(char *) * (1 + 1));
// 		p->argument->pa_argument[0] = ft_strdup("ls");
// 		p->argument->pa_argument[1] = NULL;
// 		p->next = NULL;
		
// 		ft_system(pa_arg);
		
// 		printf("END!\n");
		
// 	}
// }
