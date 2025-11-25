#include "minishell.h"

bool ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void shell_init(t_data *d,t_shell *shell, char **envp)
{
	d->heredoc_skip = 0;
	shell->data = d;
    shell->envp = create_envp_from_data(d);

	if (arena_init(&d->arena_tok, 100000) == -1)
		destroy_and_exit(d, "Arena init fail\n", 1);
	if (vec_new(&d->vec_tok, 1, sizeof(t_token *)) == -1)
		destroy_and_exit(d, "Vec init fail\n", 1);
	if (arena_init(&d->arena_env, 100000) == -1)
		destroy_and_exit(d, "Arena init fail\n", 1);
	if (vec_new(&d->vec_env, 1, sizeof(t_env *)) == -1)
		destroy_and_exit(d, "Vec init fail\n", 1);
	if (vec_new(&d->vec_cmds, 1, sizeof(t_cmd *)) == -1)
		destroy_and_exit(d, "Arena init fail\n", 1);
	envp_init(d, envp);
}

bool str_cmp(char *s1, char *s2)
{
	size_t	i;

	i = 0;
	while (s1[i] || s2[i])
	{
		if (s1[i] != s2[i])
			return (false);
		i++;
	}
	return (true);
}