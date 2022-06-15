#include "../../include/minishell.h"

int main(int argc, char **argv, char **env)
{
	t_env_root	*root;

	root = ft_dpenv_to_bstenv(env);
	ft_env_delete(root, "PATH");
	ft_env_insert(root, "USER", "테스트입니당 값 수정되나 보려구용");
	ft_env_insert(root, "USER123", "테스트입니당 값 추가중");
	ft_print_env_in_order(root->root);
}
