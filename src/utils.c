#include "minishell.h"

bool ft_isspace(char c)
{
	return (c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r'
		|| c == ' ');
}

void shell_init(t_data *d)
{
	//perhaps more things to init here
	if (arena_init(&d->arena, 10000) == -1)
		exit(EXIT_FAILURE);
	if (vec_new(&d->vec_tok, 1, sizeof(t_token *)) == -1)
	{
		arena_free(&d->arena);
		exit(EXIT_FAILURE);
	}
}