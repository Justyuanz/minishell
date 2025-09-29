#include "minishell.h"

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