#include "minishell.h"

t_data *get_data(void)
{
	static t_data d;

	return &d;
}
t_cmd *get_cmd(t_data *d, size_t index)
{
    return ((t_cmd *)vec_get(&d->vec_cmd, index));
}

t_token *get_tok(t_data *d, size_t index)
{
    return ((t_token *)vec_get(&d->vec_tok, index));
}

t_env *get_env(t_data *d, size_t index)
{
    return ((t_env *)vec_get(&d->vec_env, index));
}

bool ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void shell_init(t_data *d,char **envp)
{
	//perhaps more things to init here
	if (arena_init(&d->arena_tok, 10000) == -1)
		exit(EXIT_FAILURE);
	if (vec_new(&d->vec_tok, 1, sizeof(t_token *)) == -1)
	{
		arena_free(&d->arena_tok);
		exit(EXIT_FAILURE);
	}
	if (arena_init(&d->arena_env, 10000) == -1)
		exit(EXIT_FAILURE);
	if (vec_new(&d->vec_env, 1, sizeof(t_env *)) == -1)
	{
		arena_free(&d->arena_env);
		exit(EXIT_FAILURE);
	}
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