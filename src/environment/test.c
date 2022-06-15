#include "../../include/minishell.h"

char	*ft_strndup(const char *src, size_t n)
{
	size_t	i;
	size_t	src_size;
	char	*dup;

	i = 0;
	src_size = ft_strlen(src);
	if (src_size < n)
		n = src_size;
	dup = (char *)malloc(sizeof(char) * (n + 1));
	if (dup == NULL)
		return (NULL);
	while (i < n)
	{
		dup[i] = src[i];
		++i;
	}
	dup[i] = 0;
	return (dup);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] != '\0' && s2[i] != '\0')
	{
		if (s1[i] != s2[i])
			break ;
		++i;
	}
	return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int main(int argc, char **argv, char **env)
{
	t_env_root	*root;

	root = ft_dpenv_to_bstenv(env);
	ft_env_delete(root, "PATH");
	ft_env_insert(root, "USER", "테스트입니당 값 수정되나 보려구용");
	ft_env_insert(root, "USER123", "테스트입니당 값 추가중");
	ft_print_env_in_order(root->root);
}
