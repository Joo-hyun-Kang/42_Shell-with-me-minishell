#include <unistd.h>
#include <sys/wait.h>
#include <stdio.h>

#include "../../library/libft/inc/libft.h"
#include "../../include/minishell.h"
#include "cmd.h"


void	ft_get_com(t_command *com, char *out_res);

void	ft_system(t_command *com)
{
	enum e_token_type		seperator;
	char					*pa_u_comm;
	int						is_bulletin;
	enum t_bulltein	bull_type;

	//만약에 멀티 커맨드가 파이프라면
	//파이프 처리
	seperator = com->next_token;

	ft_get_com(com, pa_u_comm);
	is_bulletin = is_u_comm_bulletin(pa_u_comm);

}

int	is_u_comm_bulletin(char *u_comm, t_bulletin out_type)
{
	// 문자열을 비교하나?
	//
	
}

void	ft_get_comm(t_command *comm, char *out_res)
{
	char	*p;
	int		length;

	p = comm->argument;
	length = 0;
	while (*p != '\0' && *p != ' ')
	{
		length++;
	}
	out_res = (char *)malloc(sizeof(char) * length + 1);
	ft_strlcpy(out_res, comm->argument, length);	
}

int	main(int argc, char **argv, char **environ)
{
	//자식 프로세스를 무조건 만들어주어야 하나?
	//자식 프로세스는 언제 만들어지나?
	//명령어 실행할 때 1개가 더 만들어진다 추가적으로 필요할 때 더 만들어진다
	enum e_token_type	multi_comm = 
	

	pid_t child_pid;

	child_pid = fork();
	
	// 자식 프로세스
	if (child_pid == 0)
	{
		//자식 프로세스라면 명령을 실행한다
		//만약에 불트인이라면 불트인 명령어를 실행한다  
		//불트인이 아니라면 /bin/에 있는 곳에서 실행시킨다
		//일단 이렇게 짜보고 파이프라면? 여러 개의 명령어의 경우엔 어떻게 실행할 것인가?  
		printf("%s\n", "Hi i'm child");
	}
	else if (child_pid == -1)
	{
		printf("%s\n", "it is fork Error!");
		//에러 처리
	}
	else
	{
		//부모 프로세스라면 기다린다
		wait(NULL);
		printf("%s\n", "Hi i'm perent");
	}

}


